#include "SignalSlot.hpp"
#include <gtest/gtest.h>

TEST(SignalTest, SlotLabbda) {
  bool wascalled = false;
  Slot<> slot([&wascalled]() { wascalled = true; });
  slot();
  EXPECT_TRUE(wascalled);
}

void setTrue(bool &wasCalled) { wasCalled = true; };

TEST(SignalTest, SlotFunction) {
  bool wascalled = false;

  Slot<bool &> slot(setTrue);
  slot(wascalled);

  EXPECT_TRUE(wascalled);
}

TEST(SignalTest, SlotFunctor) {
  struct SetTrueFunctor {
    bool wasCalled = false;
    void operator()() { wasCalled = true; };
  };

  Slot<> slot(SetTrueFunctor{});
  slot();
  EXPECT_TRUE(slot.target<SetTrueFunctor>()->wasCalled);
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
TEST(SignalTest, SlotMultipleParameter) {
  int val = 0;
  Slot<int, int> slot([&val](int x, int y) { val = x + y; });
  slot(42, 61);
  EXPECT_EQ(103, val);
}

TEST(SignalTest, SignalConnect) {
  bool wasCalled = false;
  Slot<> slot([&wasCalled]() { wasCalled = true; });
  Signal<> signal;
  signal.connect(slot);
  signal();

  EXPECT_TRUE(wasCalled);
}
TEST(SignalTest, SignalMultipleParameter) {
  int val = 0;
  Slot<int, int> slot([&val](int x, int y) { val = x + y; });
  Signal<int, int> signal;
  signal.connect(slot);
  signal(42, 61);
  EXPECT_EQ(103, val);
}

TEST(SignalTest, SignalDisconnect) {
  bool wasCalled = false;
  Slot<> slot([&wasCalled]() { wasCalled = true; });
  Signal<> signal;
  signal.connect(slot);
  signal.disconnect(slot);
  signal();

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SignalDoubleDisconnect) {
  bool wasCalled = false;
  Slot<> slot([&wasCalled]() { wasCalled = true; });
  Signal<> signal;
  signal.connect(slot);
  signal.disconnect(slot);
  signal.disconnect(slot);
  signal();

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SignalReconnect) {
  bool wasCalled = false;
  Slot<> slot([&wasCalled]() { wasCalled = true; });
  Signal<> signal;
  signal.connect(slot);
  signal.disconnect(slot);
  signal.connect(slot);

  signal();

  EXPECT_TRUE(wasCalled);
}

TEST(SignalTest, SlotConnect) {
  bool wasCalled = false;
  Slot<> slot([&wasCalled]() { wasCalled = true; });
  Signal<> signal;
  slot.connect(signal);
  signal();

  EXPECT_TRUE(wasCalled);
}

TEST(SignalTest, SlotDisconnect) {
  bool wasCalled = false;
  Slot<> slot([&wasCalled]() { wasCalled = true; });
  Signal<> signal;
  signal.connect(slot);
  slot.disconnect();
  signal();

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SlotScopedDisconnect) {
  bool wasCalled = false;
  Signal<> signal;
  {
    Slot<> slot([&wasCalled]() { wasCalled = true; });
    signal.connect(slot);
  }
  signal();

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SignalScopedDisconnect) {
  bool wasCalled = false;
  Slot<> slot([&wasCalled]() { wasCalled = true; });
  {
    Signal<> signal;
    signal.connect(slot);
  }
  slot.disconnect(); // no double free/corruption error should be thrown
}

TEST(SignalTest, SlotDoubleDisconnect) {
  bool wasCalled = false;
  Slot<> slot([&wasCalled]() { wasCalled = true; });
  Signal<> signal;
  signal.connect(slot);
  slot.disconnect();
  slot.disconnect();
  signal();

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SignalSlotDoubleDisconnect) {
  bool wasCalled = false;
  Slot<> slot([&wasCalled]() { wasCalled = true; });
  Signal<> signal;
  signal.connect(slot);
  slot.disconnect();
  signal.disconnect(slot);
  signal();

  EXPECT_FALSE(wasCalled);
}

TEST(SignalTest, SlotSignalDoubleDisconnect) {
  bool wasCalled = false;
  Slot<> slot([&wasCalled]() { wasCalled = true; });
  Signal<> signal;
  signal.connect(slot);
  signal.disconnect(slot);
  slot.disconnect();
  signal();

  EXPECT_FALSE(wasCalled);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
