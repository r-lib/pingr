
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
