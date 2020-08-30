#include "gtest/gtest.h"

namespace {

TEST(Test, ThatPasses) {
  EXPECT_TRUE(true);
}

TEST(Test, ThatFails) {
  EXPECT_TRUE(false);
}

}  // namespace
