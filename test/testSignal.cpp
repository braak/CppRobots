#include "SignalSlot.hpp"
#include <gtest/gtest.h>

TEST(SignalTest, SlotLabbda) {

  Slot<int, int> slot1([](int i, int j) { EXPECT_EQ(i, j); });
  slot1(1, 1);

  Slot<int, int> slot2([](int i, int j) { EXPECT_NE(i, j); });
  slot2(1, 2);
}

void expectEqFunction(int i, int j) { EXPECT_EQ(i, j); };
void expectNeFunction(int i, int j) { EXPECT_NE(i, j); };
TEST(SignalTest, SlotFunction) {

  Slot<int, int> slot1(expectEqFunction);
  slot1(1, 1);

  Slot<int, int> slot2(expectNeFunction);
  slot2(1, 2);
}

TEST(SignalTest, SlotFunctor) {
  struct ExpectEqFunctor {
    void operator()(int i, int j) { EXPECT_EQ(i, j); };
  };

  struct expectNeFunctor {
    void operator()(int i, int j) { EXPECT_NE(i, j); };
  };

  Slot<int, int> slot1(ExpectEqFunctor{});
  slot1(1, 1);

  Slot<int, int> slot2(expectNeFunctor{});
  slot2(1, 2);
}

TEST(SignalTest, SlotObserver) {
  struct Observer {
    Observer() : setX([this](int x_) { this->x = x_; }){};
    Slot<int> setX;
    int getX() { return x; };

  private:
    int x;
  };

  Observer observer;

  Signal<int> setObserverX;

  observer.setX.connect(setObserverX);

  setObserverX(19);

  EXPECT_EQ(19, observer.getX());
}

TEST(SignalTest, SignalConnect) {
  bool wasCalled = false;
  Slot<int, int> slot1([&wasCalled](int i, int j) { wasCalled = true; });
  Signal<int, int> signal1;
  signal1.connect(slot1);
  signal1(1, 1);

  EXPECT_TRUE(wasCalled);
}

TEST(SignalTest, SignalCall) {
  bool wasCalled = false;
  Slot<int, int> slot1([&wasCalled](int i, int j) { wasCalled = true; });
  slot1(1, 1);

  EXPECT_TRUE(wasCalled);
}

TEST(SignalTest, SignalDisconnect) {
  bool wasCalled = false;
  Slot<int, int> slot1([&wasCalled](int i, int j) { wasCalled = true; });
  Signal<int, int> signal1;
  signal1.connect(slot1);
  signal1.disconnect(slot1);
  signal1(1, 1);

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SignalDoubleDisconnect) {
  bool wasCalled = false;
  Slot<int, int> slot1([&wasCalled](int i, int j) { wasCalled = true; });
  Signal<int, int> signal1;
  signal1.connect(slot1);
  signal1.disconnect(slot1);
  signal1.disconnect(slot1);
  signal1(1, 1);

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SignalReconnect) {
  bool wasCalled = false;
  Slot<int, int> slot1([&wasCalled](int i, int j) { wasCalled = true; });
  Signal<int, int> signal1;
  signal1.connect(slot1);
  signal1.disconnect(slot1);
  signal1.connect(slot1);

  signal1(1, 1);

  EXPECT_TRUE(wasCalled);
}

TEST(SignalTest, SlotConnect) {

  bool wasCalled = false;
  Slot<int, int> slot1([&wasCalled](int i, int j) { wasCalled = true; });
  Signal<int, int> signal1;
  slot1.connect(signal1);
  signal1(1, 1);

  EXPECT_TRUE(wasCalled);
}

TEST(SignalTest, SlotDisconnect) {
  bool wasCalled = false;
  Slot<int, int> slot1([&wasCalled](int i, int j) { wasCalled = true; });
  Signal<int, int> signal1;
  signal1.connect(slot1);
  slot1.disconnect();
  signal1(1, 1);

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SlotDoubleDisconnect) {
  bool wasCalled = false;
  Slot<int, int> slot1([&wasCalled](int i, int j) { wasCalled = true; });
  Signal<int, int> signal1;
  signal1.connect(slot1);
  slot1.disconnect();
  slot1.disconnect();
  signal1(1, 1);

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SignalSlotDoubleDisconnect) {
  bool wasCalled = false;
  Slot<int, int> slot1([&wasCalled](int i, int j) { wasCalled = true; });
  Signal<int, int> signal1;
  signal1.connect(slot1);
  slot1.disconnect();
  signal1.disconnect(slot1);
  signal1(1, 1);

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SlotSignalDoubleDisconnect) {
  bool wasCalled = false;
  Slot<int, int> slot1([&wasCalled](int i, int j) { wasCalled = true; });
  Signal<int, int> signal1;
  signal1.connect(slot1);
  signal1.disconnect(slot1);
  slot1.disconnect();
  signal1(1, 1);

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SlotScopedDisconnect) {
  bool wasCalled = false;
  Signal<int, int> signal1;
  {
    Slot<int, int> slot1([&wasCalled](int i, int j) { wasCalled = true; });
    signal1.connect(slot1);
  }
  signal1(1, 1);

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SignalScopedDisconnect) {
  bool wasCalled = false;
  Slot<int, int> slot1([&wasCalled](int i, int j) { wasCalled = true; });
  {
    Signal<int, int> signal1;
    signal1.connect(slot1);
  }
  slot1.disconnect(); // no doluble free/corruption error should be thrown
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
