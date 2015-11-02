#include "Collision.hpp"
#include "Rectangle.hpp"
#include <gtest/gtest.h>

TEST(CollisionTest, Overlap) {
  Rectangle rect1({10, 10}, {2, 2});
  Rectangle rect2({10, 10}, {0, 0});
  Rectangle rect3({10, 10}, {2, 2}, M_PI / 4.0);

  Collision collision1(rect1, rect2);
  Collision collision2(rect1, rect3);

  EXPECT_TRUE((bool)collision1);
  EXPECT_TRUE((bool)collision2);
}

TEST(CollisionTest, NoOverlap) {
  Rectangle rect1({10, 10}, {20, 20});
  Rectangle rect2({1, 1}, {0, 0});
  Rectangle rect3({1, 1}, {0, 0}, M_PI / 4.0);

  Collision collision1(rect1, rect2);
  Collision collision2(rect1, rect3);

  EXPECT_FALSE((bool)collision1);
  EXPECT_FALSE((bool)collision2);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
