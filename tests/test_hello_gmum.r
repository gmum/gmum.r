library('testthat')
library('gmum.r')

test_that('Hello gmum working', {
  expect_that(hello_gmum(), equals(c('hello', 'gmum')))

})