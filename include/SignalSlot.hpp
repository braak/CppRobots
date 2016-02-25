/**
*   \copyright Copyright 2016 Hochschule Emden/Leer. This project is released
* under
* the MIT License, see the file
* LICENSE.md for rights and limitations.
*   \file SignalSlot.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __SIGNAL_SLOT__
#define __SIGNAL_SLOT__

#include <map>
#include <functional>

template <class... Args> class Signal;

/**
  \brief The endpoint of a Signal-Slot connection.

  A Signal-Slot   system is a variation of the Observer pattern, the Signal is
  the 'Subject' and the Slot is the 'Observer'. The main difference is, that
  no internal state is managed (at least not by default, see test/testSignal.cpp
  for an example how to do that).

  A Slot will be called when its connected Signal is called.

  A Slot can be connected to exactly one Signal.

  A Slot can also be called on its own without invoking the Signal.

  \tparam Args... Arguments of the underliying function.
*/
template <class... Args> class Slot : public std::function<void(Args...)> {
public:
  /**
    Inherited Constructors from std::function.
  */
  using std::function<void(Args...)>::function;

  /**
  \brief Destructor.
  The Destructor automaticaly disconnects the Slot from its Signal.
  */
  ~Slot() { disconnect(); }

  /**
  \brief Connects the Slot to a Signal.
  The old Signal will be disconnected.
  \param newSignal the new Signal.
  */
  void connect(Signal<Args...> &newSignal) { newSignal.connect(*this); };

  /**
    \brief  Disconnects the Slot from its Signal
  */
  void disconnect() {
    if (signal) {
      signal->disconnect(*this);
    }
  };

private:
  Signal<Args...> *signal = nullptr;
  std::size_t id = 0;
  friend class Signal<Args...>; //!< Acces used to connect Signal and Slot
};

/**
\brief The startpoint of a Signal-Slot connection.

A Signal-Slot   system is a variation of the Observer pattern, the Signal is
the 'Subject' and   the Slot is the 'Observer'. The main difference is, that
Slots does not store an internal state.

When a Signal is called all connected Slots will be called.

A Signal can be connected to many Slots.

\tparam Args Arguments of the underliying function.
*/
template <class... Args> class Signal {
public:
  /**
  \brief Destructor.
  All Slots will be automaticaly disconnected.
  */
  ~Signal() {
    for (auto &slot : slots) {
      slot.second->signal = nullptr;
    }
  }

  /**
  \brief  Connect the Signal to a new Slot.
  \param slot The new Slot.
  */
  void connect(Slot<Args...> &slot) {
    if (slot.signal == this) {
      // we are already connected
      return;
    }
    slot.disconnect(); // disconnect from the previous Signal

    auto id = nextID++;

    // update the slot
    slot.id = id;
    slot.signal = this;

    // save the slot in slots
    slots[id] = &slot;
  }

  /**
  \brief Disconnect a Slot from the Signal.

  \param slot The Slot to disconnect.
  */
  void disconnect(Slot<Args...> &slot) {
    if (slot.signal == this) { // if we are the owner of the slot
      slots.erase(slot.id);
      slot.signal = nullptr;
    }
  }

  /**
    \brief Raises the Signal.

    Calls all connectd Slots with the given parameters.
    \param args The Function arguments of the connected Slot.
  */
  void operator()(Args... args) {
    // call all slots with the given arguments.
    for (auto &slot : slots) {
      if (slot.second) {
        (*slot.second)(args...);
      }
    }
  }

private:
  std::map<std::size_t, Slot<Args...> *> slots;
  std::size_t nextID = 1;
  /*NOTE: unfortunatly std::functions are not comparable (neither less than nor
   * equality). If they were we could use a set (or unordered_set) to store
   * them, instead of having to rely on nextID.
   */
};

#endif /* end of include guard: __SIGNAL_SLOT__ */
