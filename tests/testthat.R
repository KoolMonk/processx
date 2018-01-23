library(testthat)
library(processx3)

Sys.setenv("R_TESTS" = "")
test_check("processx3", reporter = "summary")
