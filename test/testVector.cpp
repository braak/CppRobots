#include "Vector.hpp"
#include <gtest/gtest.h>

TEST(VectorTest, DefaultConstructor) {
  Vector_i p;
  EXPECT_EQ(0, p.x);
  EXPECT_EQ(0, p.y);
}

TEST(VectorTest, Construction) {
  Vector_i p(1, 2);
  EXPECT_EQ(1, p.x);
  EXPECT_EQ(2, p.y);
}

TEST(VectorTest, Alias) {
  Vector_i p(1, 2);
  EXPECT_EQ(p.width, p.x);
  EXPECT_EQ(p.height, p.y);
}

TEST(VectorTest, Addition) {
  Vector_i p1(1, 1);
  Vector_i p2(2, 2);
  Vector_i p3 = p1 + p2;

  EXPECT_EQ(3, p3.x);
  EXPECT_EQ(3, p3.y);
}

TEST(VectorTest, Subtraction) {
  Vector_i p1(1, 1);
  Vector_i p2(2, 2);
  Vector_i p3 = p1 - p2;

  EXPECT_EQ(-1, p3.x);
  EXPECT_EQ(-1, p3.y);
}
TEST(VectorTest, LeftScalarMultiplication) {
  Vector_i p1(1, 1);
  Vector_i p2 = 2 * p1;

  EXPECT_EQ(2, p2.x);
  EXPECT_EQ(2, p2.y);
}
TEST(VectorTest, RightScalarMultiplication) {
  Vector_i p1(1, 1);
  Vector_i p2 = p1 * 2;

  EXPECT_EQ(2, p2.x);
  EXPECT_EQ(2, p2.y);
}

TEST(VectorTest, ScalarDivisionn) {
  Vector_i p1(4, 4);
  Vector_i p2 = p1 / 2;

  EXPECT_EQ(2, p2.x);
  EXPECT_EQ(2, p2.y);
}

TEST(VectorTest, Equality) {
  Vector_i p1(4, 4);
  Vector_i p2(4, 4);

  EXPECT_TRUE(p1 == p2);
  EXPECT_FALSE(p1 != p2);
}

TEST(VectorTest, Inequality) {
  Vector_i p1(4, 4);
  Vector_i p2(1, 4);

  EXPECT_TRUE(p1 != p2);
  EXPECT_FALSE(p1 == p2);
}

TEST(VectorTest, Cast) {
  Vector_i p1(4, 4);
  Vector_f p2(4.1, 4.2);

  EXPECT_EQ(p1, static_cast<Vector<int>>(p2));
}

TEST(VectorTest, DotProduct) {
  Vector_i p1(4, 3);
  Vector_i p2(1, 2);

  EXPECT_EQ(10, p1.dot(p2));
}

TEST(VectorTest, Zero) {
  Vector_i p1(0, 0);
  Vector_i p2 = Vector_i::zero();

  EXPECT_EQ(p1, p2);
}

TEST(VectorTest, UnitX) {
  Vector_i p1(1, 0);
  Vector_i p2 = Vector_i::unit_x();

  EXPECT_EQ(p1, p2);
}

TEST(VectorTest, UnitY) {
  Vector_i p1(0, 1);
  Vector_i p2 = Vector_i::unit_y();

  EXPECT_EQ(p1, p2);
}

TEST(VectorTest, PolarUnit) {
  const double angle = 0.15;
  Vector_i p1(cos(angle), sin(angle));
  Vector_i p2 = Vector_i::polar(angle);

  EXPECT_EQ(p1, p2);
}

TEST(VectorTest, Polar) {
  const double angle = -0.15;
  const double magnitude = 13;
  Vector_i p1(13 * cos(angle), 13 * sin(angle));
  Vector_i p2 = Vector_i::polar(angle, magnitude);

  EXPECT_EQ(p1, p2);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
