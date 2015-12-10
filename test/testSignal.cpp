#include "SignalSlot.hpp"
#include <gtest/gtest.h>

TEST(SignalTest, SlotLabbda) {
  bool wascalled1 = false;
  Slot<> slot1([&wascalled1]() { wascalled1 = true; });
  slot1();
  EXPECT_TRUE(wascalled1);
}

void setTrue(bool &wasCalled) { wasCalled = true; };

TEST(SignalTest, SlotFunction) {
  bool wascalled1 = false;

  Slot<bool &> slot1(setTrue);
  slot1(wascalled1);

  EXPECT_TRUE(wascalled1);
}

TEST(SignalTest, SlotFunctor) {
  struct ExpectEqFunctor {
    bool wasCalled = false;
    void operator()() { wasCalled = true; };
  };

  Slot<> slot1(ExpectEqFunctor{});
  slot1();
  EXPECT_TRUE(slot1.target<ExpectEqFunctor>()->wasCalled);
}

TEST(SignalTest, SlotObserver) {
  struct Observer {
    Observer() : x(0){};
    int getX() { return x; };
    Slot<int> setX = [this](int x_) { this->x = x_; };

  private:
    int x;
  };

  Observer observer{};
  observer.setX(19);

  EXPECT_EQ(19, observer.getX());
}

TEST(SignalTest, SignalConnect) {
  bool wasCalled = false;
  Slot<> slot1([&wasCalled]() { wasCalled = true; });
  Signal<> signal1;
  signal1.connect(slot1);
  signal1();

  EXPECT_TRUE(wasCalled);
}

TEST(SignalTest, SignalCall) {
  bool wasCalled = false;
  Slot<> slot1([&wasCalled]() { wasCalled = true; });
  slot1();

  EXPECT_TRUE(wasCalled);
}

TEST(SignalTest, SignalDisconnect) {
  bool wasCalled = false;
  Slot<> slot1([&wasCalled]() { wasCalled = true; });
  Signal<> signal1;
  signal1.connect(slot1);
  signal1.disconnect(slot1);
  signal1();

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SignalDoubleDisconnect) {
  bool wasCalled = false;
  Slot<> slot1([&wasCalled]() { wasCalled = true; });
  Signal<> signal1;
  signal1.connect(slot1);
  signal1.disconnect(slot1);
  signal1.disconnect(slot1);
  signal1();

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SignalReconnect) {
  bool wasCalled = false;
  Slot<> slot1([&wasCalled]() { wasCalled = true; });
  Signal<> signal1;
  signal1.connect(slot1);
  signal1.disconnect(slot1);
  signal1.connect(slot1);

  signal1();

  EXPECT_TRUE(wasCalled);
}

TEST(SignalTest, SlotConnect) {

  bool wasCalled = false;
  Slot<> slot1([&wasCalled]() { wasCalled = true; });
  Signal<> signal1;
  slot1.connect(signal1);
  signal1();

  EXPECT_TRUE(wasCalled);
}

TEST(SignalTest, SlotDisconnect) {
  bool wasCalled = false;
  Slot<> slot1([&wasCalled]() { wasCalled = true; });
  Signal<> signal1;
  signal1.connect(slot1);
  slot1.disconnect();
  signal1();

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SlotDoubleDisconnect) {
  bool wasCalled = false;
  Slot<> slot1([&wasCalled]() { wasCalled = true; });
  Signal<> signal1;
  signal1.connect(slot1);
  slot1.disconnect();
  slot1.disconnect();
  signal1();

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SignalSlotDoubleDisconnect) {
  bool wasCalled = false;
  Slot<> slot1([&wasCalled]() { wasCalled = true; });
  Signal<> signal1;
  signal1.connect(slot1);
  slot1.disconnect();
  signal1.disconnect(slot1);
  signal1();

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SlotSignalDoubleDisconnect) {
  bool wasCalled = false;
  Slot<> slot1([&wasCalled]() { wasCalled = true; });
  Signal<> signal1;
  signal1.connect(slot1);
  signal1.disconnect(slot1);
  slot1.disconnect();
  signal1();

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SlotScopedDisconnect) {
  bool wasCalled = false;
  Signal<> signal1;
  {
    Slot<> slot1([&wasCalled]() { wasCalled = true; });
    signal1.connect(slot1);
  }
  signal1();

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SignalScopedDisconnect) {
  bool wasCalled = false;
  Slot<> slot1([&wasCalled]() { wasCalled = true; });
  {
    Signal<> signal1;
    signal1.connect(slot1);
  }
  slot1.disconnect(); // no doluble free/corruption error should be thrown
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
