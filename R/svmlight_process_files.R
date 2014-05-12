#' Description
#' Exemplary execution of SVMLightRunner.
#' Details
#' On success it should print out created model.
#' @export
svmlight_process_files <- function(
  training_file,
  test_file
  ){
  .Call(
    "svmlight_process_files",
    training_file,
    test_file,
    PACKAGE = "gmum.r"
    )
}
