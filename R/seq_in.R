#' Find a (the first) sequence in a vector
#'
#' Returns the starting position for the first occurrence of `pattern_vector`
#' in `source_vector`. Works on all atomic vectors including raw vectors,
#' making it possible to find a sequence
#'
#' @param source_vector vector to search in
#' @param pattern_vector vector that has the sequence to search for
#' @export
#' @examples
#' seq_in(c(1,5,4,3,2,1,5), c(4,3, 2))
#'
#' # raw
# # HTTP/1.1 302 Moved Temporarily\nServer: nginx
# src <- c(0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e, 0x31, 0x20,
# 0x33, 0x30, 0x32, 0x20, 0x4d, 0x6f, 0x76, 0x65, 0x64, 0x20, 0x54,
# 0x65, 0x6d, 0x70, 0x6f, 0x72, 0x61, 0x72, 0x69, 0x6c, 0x79, 0x0a,
# 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x3a, 0x20, 0x6e, 0x67, 0x69,
# 0x6e, 0x78)
#
# seq_in(src, charToRaw("HTTP/1.1 302"))
#'
seq_in <- function(source_vector, pattern_vector) {

  which(
    Reduce(
      '+',
      lapply(
        seq_along(y <- lapply(pattern_vector, '==', source_vector)),
        function(x) {
          y[[x]][x:(length(source_vector) - length(pattern_vector) + x)]
        }
      )
    ) == length(pattern_vector)
  )

}
