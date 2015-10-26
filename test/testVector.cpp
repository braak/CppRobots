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

TEST(VectorTest, Equality) {
  Vector_i p1(4, 4);

  EXPECT_TRUE(Vector_i(4, 4) == p1);
  EXPECT_FALSE(Vector_i(4, 4) != p1);
}

TEST(VectorTest, Inequality) {
  Vector_i p1(4, 4);

  EXPECT_FALSE(Vector_i(1, 4) == p1);
  EXPECT_TRUE(Vector_i(1, 4) != p1);
}

TEST(VectorTest, Addition) {
  Vector_i p1(1, 2);
  Vector_i p2(4, 4);

  EXPECT_EQ(Vector_i(5, 6), p1 + p2);
}

TEST(VectorTest, Subtraction) {
  Vector_i p1(1, 2);
  Vector_i p2(4, 4);

  EXPECT_EQ(Vector_i(-3, -2), p1 - p2);
}
TEST(VectorTest, LeftScalarMultiplication) {
  Vector_i p1(1, 2);
  EXPECT_EQ(Vector_i(2, 4), 2 * p1);
}
TEST(VectorTest, RightScalarMultiplication) {
  Vector_i p1(1, 2);
  EXPECT_EQ(Vector_i(2, 4), p1 * 2);
}

TEST(VectorTest, ScalarDivisionn) {
  Vector_i p1(4, 4);
  EXPECT_EQ(Vector_i(2, 2), p1 / 2);
}

TEST(VectorTest, Cast) {
  Vector_i p1(4, 4);
  Vector_f p2(4.122332, 4.423232);

  EXPECT_EQ(p1, static_cast<Vector<int>>(p2));
}

TEST(VectorTest, Rotation) {
  Vector_i p(4, 0);
  p.rotate(M_PI);

  EXPECT_EQ(Vector_i(-4, 0), p);

  p.rotate(M_PI / 2);
  EXPECT_EQ(Vector_i(0, -4), p);
}

TEST(VectorTest, DotProduct) {
  Vector_i p1(4, 3);
  Vector_i p2(1, 2);

  EXPECT_EQ(10, p1.dot(p2));

  EXPECT_EQ(10, dot(p1, p2));
}

TEST(VectorTest, Magnitude) {
  Vector_d p1(4, 3);

  EXPECT_EQ(5, p1.magnitude());
}

TEST(VectorTest, Angle) {
  Vector_d p1(4, 4);

  EXPECT_DOUBLE_EQ(M_PI / 4.0, p1.angle());
}

TEST(VectorTest, One) { EXPECT_EQ(Vector_i(1, 1), Vector_i::one()); }

TEST(VectorTest, Zero) { EXPECT_EQ(Vector_i(0, 0), Vector_i::zero()); }

TEST(VectorTest, UnitX) { EXPECT_EQ(Vector_i(1, 0), Vector_i::unit_x()); }

TEST(VectorTest, UnitY) { EXPECT_EQ(Vector_i(0, 1), Vector_i::unit_y()); }

TEST(VectorTest, PolarUnit) {
  const double angle = 0.15;
  Vector_d p1(cos(angle), sin(angle));
  Vector_d p2 = Vector_d::polar(angle);

  EXPECT_EQ(p1, p2);
}

TEST(VectorTest, Polar) {
  const double angle = -0.15;
  const double magnitude = 13;
  Vector_d p1(13 * cos(angle), 13 * sin(angle));
  Vector_d p2 = Vector_d::polar(angle, magnitude);

  EXPECT_EQ(p1, p2);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
