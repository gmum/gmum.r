#include "gtest/gtest.h"

TEST(FirstTest,ShouldPass) {
  ASSERT_EQ(0,0);
}

TEST(FirstTest,ShouldFail) {
  ASSERT_EQ(0,1);
}
