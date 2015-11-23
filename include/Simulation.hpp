/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Simulation.cpp
*   \author Jan-Niklas Braak
*/
#ifndef __SIMULATION__
#define __SIMULATION__

#include "Robot.hpp"
#include "Agent.hpp"
#include "Angles.hpp"
#include "Collision.hpp"
#include "Rules.hpp"
#include "Projectile.hpp"
#include "SignalSlot.hpp"
#include "pathUtility.hpp"

#include <memory>
#include <iostream>
#include <random>
#include <list>
#include <map>
#include <iomanip>
// #include <time.h>

class Simulation {
public:
  const Rules rules;

protected:
  using KeyValuePair = std::pair<std::string, Robot>;
  std::map<std::string, Robot> players;
  std::list<Projectile> projectiles;

  std::default_random_engine generator;

  double runTime = 0;

public:
  Simulation(const Rules &rules, unsigned int seed);
  virtual void newPlayer(std::string name, Agent *agent);
  virtual void newPlayer(std::string name, Agent *agent, Vector_d position,
                         double rotation);
  virtual void update();

  virtual double getRuntime() const;
  virtual int getNumPlayers() const;
  virtual bool isRunning() const;
  virtual void exit(){};
  virtual void log(std::string text) { (void)text; };
  std::string runtimeString() {

    // time_t seconds(runTime);
    // tm *p = gmtime(&seconds);

    std::stringstream rt;

    // int runTimeInt = runTime;

    // int days = runTimeInt / 60 / 60 / 24;
    // int hours = int(runTime / 60 / 60) % 24;
    int minutes = int(runTime / 60);
    // int seconds = runTimeInt % 60;
    float seconds = fmod(runTime, 60);

    // rt << std::setfill('0') << std::setw(2) << hours << ":";
    rt << minutes << ":";
    rt << std::setfill('0') << std::setw(6) << std::fixed
       << std::setprecision(3) << seconds;
    // rt << std::setfill('0') << std::setw(3) << milliseconds;
    return rt.str();
  }

  Signal<std::string> deathSignal;
  Signal<std::string> newPlayerSignal;
  Signal<std::string, std::string> collisionSignal;
  Signal<std::string, std::string> hitSignal;
  Signal<std::string> outOfBoundsSignal;

private:
  void updatePlayers();
  void updateProjectiles();

  void check_scan(Robot &robot);
  bool inScanArea(Vector_d const &p1, double rotation,
                  Vector_d const &p2) const;
};

#endif /* end of include guard: __SIMULATION__ */
