
process__exists <- function(pid) {
  .Call(c_processx3__process_exists, pid)
}
