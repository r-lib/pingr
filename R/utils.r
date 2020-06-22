
int <- as.integer

is_string <- function(x) {
  is.character(x) && length(x) == 1 && !is.na(x)
}

is_string_or_null <- function(x) {
  is.null(x) || is_string(x)
}

is_count <- function(x) {
  is.integer(x) && length(x) == 1 && !is.na(x)
}

is_type <- function(x) {
  is_count(x)
}

is_class <- function(x) {
  is_count(x)
}

safe_examples <- function() {
  !is_cran_check() && is_online()
}

is_cran_check <- function () {
  if (identical(Sys.getenv("NOT_CRAN"), "true")) {
    FALSE
  } else {
    Sys.getenv("_R_CHECK_PACKAGE_NAME_", "") != ""
  }
}
