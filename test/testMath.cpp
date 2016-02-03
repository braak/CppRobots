#include <gtest/gtest.h>

#include "mathUtility.hpp"

TEST(MathTest, wrap) {
  EXPECT_DOUBLE_EQ(5.0, wrap(1.0, 6.0, 2.0));
  EXPECT_DOUBLE_EQ(4.0, wrap(4.0, 6.0, 2.0));
  EXPECT_DOUBLE_EQ(3.0, wrap(7.0, 6.0, 2.0));
}

TEST(MathTest, clamp) {
  EXPECT_DOUBLE_EQ(2.0, clamp(1.0, 6.0, 2.0));
  EXPECT_DOUBLE_EQ(4.0, clamp(4.0, 6.0, 2.0));
  EXPECT_DOUBLE_EQ(6.0, clamp(7.0, 6.0, 2.0));
}

TEST(MathTest, lerp) {
  EXPECT_DOUBLE_EQ(1.0, lerp(1.0, 6.0, 0.0));
  EXPECT_DOUBLE_EQ(2.25, lerp(1.0, 6.0, 0.25));
  EXPECT_DOUBLE_EQ(3.5, lerp(1.0, 6.0, 0.5));
  EXPECT_DOUBLE_EQ(4.75, lerp(1.0, 6.0, 0.75));
  EXPECT_DOUBLE_EQ(6.0, lerp(1.0, 6.0, 1.0));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
