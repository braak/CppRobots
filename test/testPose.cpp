#include "Pose.cpp"
#include <gtest/gtest.h>
#include <sstream>

TEST(PoseTest, Construction) {
  Pose p(1,2,3);
  EXPECT_EQ(1, p.x);
  EXPECT_EQ(2, p.y);
  EXPECT_EQ(3, p.theta);
}

TEST(PoseTest, Addition) {
  Pose p1(1,1,1);
  Pose p2(2,2,2);
  Pose p3 = p1 + p2;

  EXPECT_EQ(3, p3.x);
  EXPECT_EQ(3, p3.y);
  EXPECT_EQ(3, p3.theta);
}
TEST(PoseTest, Subtraction) {
  Pose p1(1,1,1);
  Pose p2(2,2,2);
  Pose p3 = p1 - p2;

  EXPECT_EQ(-1, p3.x);
  EXPECT_EQ(-1, p3.y);
  EXPECT_EQ(-1, p3.theta);
}
TEST(PoseTest, LeftScalarMultiplication) {
  Pose p1(1,1,1);
  Pose p2 = 2 * p1;

  EXPECT_EQ(2, p2.x);
  EXPECT_EQ(2, p2.y);
  EXPECT_EQ(2, p2.theta);
}
TEST(PoseTest, RightScalarMultiplication) {
  Pose p1(1,1,1);
  Pose p2 = p1 * 2;

  EXPECT_EQ(2, p2.x);
  EXPECT_EQ(2, p2.y);
  EXPECT_EQ(2, p2.theta);
}

TEST(PoseTest, LeftScalarDivision) {
  Pose p1(2, 2, 2);
  Pose p2 = 4 / p1;

  EXPECT_EQ(2, p2.x);
  EXPECT_EQ(2, p2.y);
  EXPECT_EQ(2, p2.theta);
}
TEST(PoseTest, RightScalarDivisionn) {
  Pose p1(4,4,4);
  Pose p2 = p1 / 2;

  EXPECT_EQ(2, p2.x);
  EXPECT_EQ(2, p2.y);
  EXPECT_EQ(2, p2.theta);
}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
