
#ifdef _WIN32
#include <Winsock2.h>
#endif

#include "pingr.h"
#include "errors.h"

#ifdef _WIN32

#include <windows.h>
#include <windns.h>

#define AF_INET6 23

SEXP r_nsl(SEXP hostname, SEXP server, SEXP class, SEXP type) {

  PDNS_RECORD response, ptr;
  DNS_STATUS ret;
  PIP4_ARRAY pSrvList = NULL;
  int cnt = 0;
  const char *resnames[] = { "answer", "flags", "" };
  const char *recnames[] = { "name", "class", "type", "ttl", "data", "" };
  const char *flagnames[] = { "aa", "tc", "rd", "ra", "ad", "cd", "" };
  SEXP result = PROTECT(mkNamed(VECSXP, resnames));
  SEXP records = PROTECT(mkNamed(VECSXP, recnames));
  SEXP row_names = PROTECT(Rf_allocVector(INTSXP, 2));
  Rf_setAttrib(records, R_ClassSymbol, mkString("data.frame"));

  SET_VECTOR_ELT(result, 0, records);
  SET_VECTOR_ELT(result, 1, mkNamed(LGLSXP, flagnames));

  if (!isNull(server)) {
    pSrvList = (PIP4_ARRAY) LocalAlloc(LPTR,sizeof(IP4_ARRAY));
    if (!pSrvList) R_THROW_ERROR("DNS query failed, out of memory");
    pSrvList->AddrCount = 1;
    pSrvList->AddrArray[0] = inet_addr(CHAR(STRING_ELT(server, 0)));
  }

  ret = DnsQuery_A(
    CHAR(STRING_ELT(hostname, 0)),
    INTEGER(type)[0],
    DNS_QUERY_STANDARD,
    pSrvList,
    &response,
    NULL
  );

  if (ret) R_THROW_SYSTEM_ERROR_CODE(ret, "DNS query failed");

  ptr = response;
  while (ptr) {
    cnt ++;
    ptr = ptr->pNext;
  }

  SET_VECTOR_ELT(records, 0, Rf_allocVector(STRSXP, cnt));
  SET_VECTOR_ELT(records, 1, Rf_allocVector(INTSXP, cnt));
  SET_VECTOR_ELT(records, 2, Rf_allocVector(INTSXP, cnt));
  SET_VECTOR_ELT(records, 3, Rf_allocVector(INTSXP, cnt));
  SET_VECTOR_ELT(records, 4, Rf_allocVector(VECSXP, cnt));
  INTEGER(row_names)[0] = NA_INTEGER;
  INTEGER(row_names)[1] = -cnt;
  Rf_setAttrib(records, R_RowNamesSymbol, row_names);

  LOGICAL(VECTOR_ELT(result, 1))[0] = NA_LOGICAL;
  LOGICAL(VECTOR_ELT(result, 1))[1] = NA_LOGICAL;
  LOGICAL(VECTOR_ELT(result, 1))[2] = NA_LOGICAL;
  LOGICAL(VECTOR_ELT(result, 1))[3] = NA_LOGICAL;
  LOGICAL(VECTOR_ELT(result, 1))[4] = NA_LOGICAL;
  LOGICAL(VECTOR_ELT(result, 1))[5] = NA_LOGICAL;

  ptr = response; cnt = 0;
  while (ptr) {
    char buf[1025];
    int raw = 0;
    SEXP rawdata;

    SET_STRING_ELT(VECTOR_ELT(records, 0), cnt, mkChar(ptr->pName));
    INTEGER(VECTOR_ELT(records, 1))[cnt] = 1L;
    INTEGER(VECTOR_ELT(records, 2))[cnt] = (int) ptr->wType;
    INTEGER(VECTOR_ELT(records, 3))[cnt] = (int) ptr->dwTtl;

    switch(ptr->wType) {
    case DNS_TYPE_A:
      inet_ntoa(AF_INET, &(ptr->Data.A.IpAddress), buf, sizeof buf);
      break;

    case DNS_TYPE_AAAA:
      inet_ntoa(AF_INET6, &(ptr->Data.AAAA.Ip6Address), buf, sizeof buf);
      break;

    case DNS_TYPE_NS:
    case DNS_TYPE_PTR:
    case DNS_TYPE_CNAME:
      snprintf(buf, sizeof buf, "%s", ptr->Data.PTR.pNameHost);
      break;

    case DNS_TYPE_TEXT:
      snprintf(buf, sizeof buf, "%s", ptr->Data.TXT.pStringArray[0]);
      break;

    case DNS_TYPE_MX:
      snprintf(buf, sizeof buf, "%s", ptr->Data.MX.pNameExchange);
      break;

    case DNS_TYPE_SOA:
      snprintf(buf, sizeof buf, "%s. %s. %u %u %u %u %u",
	       ptr->Data.SOA.pNamePrimaryServer,
	       ptr->Data.SOA.pNameAdministrator,
	       ptr->Data.SOA.dwSerialNo,
	       ptr->Data.SOA.dwRefresh,
	       ptr->Data.SOA.dwRetry,
	       ptr->Data.SOA.dwExpire,
	       ptr->Data.SOA.dwDefaultTtl);
      break;

    default:
      raw = 1;
      rawdata = PROTECT(Rf_allocVector(RAWSXP, ptr->wDataLength));
      SET_VECTOR_ELT(VECTOR_ELT(records, 4), cnt, rawdata);
      UNPROTECT(1);
      memcpy(RAW(rawdata), &(ptr->Data.A), ptr->wDataLength);
      break;
    }

    if (!raw) SET_VECTOR_ELT(VECTOR_ELT(records, 4), cnt, mkString(buf));

    cnt++;
    ptr = ptr->pNext;
  }

  /* TODO: these leak on error, we would need to use cleancall */
  LocalFree(pSrvList);
  DnsRecordListFree(response, DnsFreeRecordList);

  UNPROTECT(3);
  return result;
}

#else

#include <resolv.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <arpa/nameser.h>

#ifdef __sun
#define u_int16_t uint16_t
#define u_int32_t uint32_t
#endif

// See https://docstore.mik.ua/orelly/networking_2ndEd/dns/ch15_02.htm
// for the documentation of the ns_* functions, because these are
// otherwise undocumented.

SEXP r_nsl(SEXP hostname, SEXP server, SEXP class, SEXP type) {
  int ret;
  unsigned char answer[64 * 1024];
  ns_msg msg;
  u_int16_t i, cnt;
  const char *resnames[] = { "answer", "flags", "" };
  const char *recnames[] = { "name", "class", "type", "ttl", "data", "" };
  const char *flagnames[] = { "aa", "tc", "rd", "ra", "ad", "cd", "" };
  SEXP result = PROTECT(mkNamed(VECSXP, resnames));
  SEXP records = PROTECT(mkNamed(VECSXP, recnames));
  SEXP row_names = PROTECT(Rf_allocVector(INTSXP, 2));
  Rf_setAttrib(records, R_ClassSymbol, mkString("data.frame"));

  SET_VECTOR_ELT(result, 0, records);
  SET_VECTOR_ELT(result, 1, mkNamed(LGLSXP, flagnames));

  ret = res_init();
  if (ret) R_THROW_SYSTEM_ERROR("Failed to initialize resolver library");

  if (!isNull(server)) {
    struct in_addr addr;
    ret = inet_pton(AF_INET, CHAR(STRING_ELT(server, 0)), &addr);
    _res.options &= ~(RES_DNSRCH | RES_DEFNAMES);
    _res.nscount = LENGTH(server);
    _res.nsaddr_list[0].sin_addr = addr;
  }

  ret = res_query(
    CHAR(STRING_ELT(hostname, 0)),
    INTEGER(class)[0],
    INTEGER(type)[0],
    answer,
    sizeof answer);
  if (ret == -1) R_THROW_SYSTEM_ERROR("DNS query failed");

  ret = ns_initparse(answer, ret, &msg);
  if (ret == -1) R_THROW_SYSTEM_ERROR("Cannot parse DNS answer");

  LOGICAL(VECTOR_ELT(result, 1))[0] = ns_msg_getflag(msg, ns_f_aa);
  LOGICAL(VECTOR_ELT(result, 1))[1] = ns_msg_getflag(msg, ns_f_tc);
  LOGICAL(VECTOR_ELT(result, 1))[2] = ns_msg_getflag(msg, ns_f_rd);
  LOGICAL(VECTOR_ELT(result, 1))[3] = ns_msg_getflag(msg, ns_f_ra);
  LOGICAL(VECTOR_ELT(result, 1))[4] = ns_msg_getflag(msg, ns_f_ad);
  LOGICAL(VECTOR_ELT(result, 1))[5] = ns_msg_getflag(msg, ns_f_cd);

  cnt = ns_msg_count(msg, ns_s_an);
  SET_VECTOR_ELT(records, 0, Rf_allocVector(STRSXP, cnt));
  SET_VECTOR_ELT(records, 1, Rf_allocVector(INTSXP, cnt));
  SET_VECTOR_ELT(records, 2, Rf_allocVector(INTSXP, cnt));
  SET_VECTOR_ELT(records, 3, Rf_allocVector(INTSXP, cnt));
  SET_VECTOR_ELT(records, 4, Rf_allocVector(VECSXP, cnt));
  INTEGER(row_names)[0] = NA_INTEGER;
  INTEGER(row_names)[1] = -cnt;
  Rf_setAttrib(records, R_RowNamesSymbol, row_names);

  for (i = 0; i < cnt; i++) {
    ns_rr rec;
    u_int16_t class, type;
    u_int16_t mx;
    u_int32_t soa[5];
    const u_char *data;
    char buf[NS_MAXDNAME];
    int raw = 0;
    SEXP rawdata;

    ret = ns_parserr(&msg, ns_s_an, i, &rec);
    if (ret == -1) R_THROW_SYSTEM_ERROR("Cannot parse DNS record");
    class = ns_rr_class(rec);
    type = ns_rr_type(rec);
    data = ns_rr_rdata(rec);

    SET_STRING_ELT(VECTOR_ELT(records, 0), i, mkChar(ns_rr_name(rec)));
    INTEGER(VECTOR_ELT(records, 1))[i] = (int) class;
    INTEGER(VECTOR_ELT(records, 2))[i] = (int) type;
    INTEGER(VECTOR_ELT(records, 3))[i] = (int) ns_rr_ttl(rec);

    ret = 0;

    switch (type) {

    case ns_t_a:
      inet_ntop(AF_INET, data, buf, sizeof buf);
      break;

    case ns_t_aaaa:
      inet_ntop(AF_INET6, data, buf, sizeof buf);
      break;

    case ns_t_ns:
    case ns_t_ptr:
    case ns_t_cname:
      ret = ns_name_uncompress(ns_msg_base(msg), ns_msg_end(msg),
                               data, buf, sizeof buf);
      break;

    case ns_t_txt:
      snprintf(buf, (size_t) data[0]+1, "%s", data + 1);
      break;

    case ns_t_mx:
      NS_GET16(mx, data);
      ret = ns_name_uncompress(ns_msg_base(msg), ns_msg_end(msg),
                               data, buf, sizeof buf);
      break;

    case ns_t_soa: {
      char *buf2 = buf;
      size_t bufsize = sizeof buf;
      int len, j;
      ret = ns_name_uncompress(ns_msg_base(msg), ns_msg_end(msg),
                               data, buf, sizeof buf);
      if (ret < 0) R_THROW_SYSTEM_ERROR("Cannot parse SOA DNS record");

      data += ret; len = strlen(buf2); buf2 += len; bufsize -= len;
      if (bufsize > 2) {
        *buf2 = '.'; buf2++; bufsize--; *buf2 = ' '; buf2++; bufsize--;
      }

      ret = ns_name_uncompress(ns_msg_base(msg), ns_msg_end(msg),
                               data, buf2, bufsize);
      if (ret < 0) R_THROW_SYSTEM_ERROR("Cannot parse SOA DNS record");

      data += ret; len = strlen(buf2); buf2 += len; bufsize -= len;
      if (bufsize > 2) {
        *buf2 = '.'; buf2++; bufsize--; *buf2 = ' '; buf2++; bufsize--;
      }

      if (ns_msg_end(msg) - data < 5*NS_INT32SZ) {
        R_THROW_ERROR("Cannot parse SOA DNS record");
      }
      for (j = 0; j < 5; j++) NS_GET32(soa[j], data);
      snprintf(buf2, bufsize, "%u %u %u %u %u",
               soa[0], soa[1], soa[2], soa[3], soa[4]);
      break; }

    default:
      raw = 1;
      rawdata = PROTECT(Rf_allocVector(RAWSXP, ns_rr_rdlen(rec)));
      SET_VECTOR_ELT(VECTOR_ELT(records, 4), i, rawdata);
      UNPROTECT(1);
      memcpy(RAW(rawdata), ns_rr_rdata(rec), ns_rr_rdlen(rec));
      break;
    }

    if (ret < 0) {
      R_THROW_SYSTEM_ERROR("Cannot parse NS/PTR/CNAME DNS record");
    }

    if (!raw) SET_VECTOR_ELT(VECTOR_ELT(records, 4), i, mkString(buf));
  }

  UNPROTECT(3);
  return result;
}

#endif
