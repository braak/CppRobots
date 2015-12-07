/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file Simulation.hpp
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

/**
  A class that encapsulates the simulation logic.
*/
class Simulation {
public:
  /**
    The Rules used in the Simulation.
  */
  const Rules rules;

protected:
  /**
    Storage of the players of the game and their names.
  */
  std::map<std::string, Robot> players;
  /**
    List of Projectiles, that are currently active.
  */
  std::list<Projectile> projectiles;

  /**
    Runtime of the Simulation, in simulation time, not realtime
  */
  double runTime = 0;

  /**
    Engine used for ramdomisation.
  */
  std::default_random_engine generator;

public:
  /**
    Constructor.

    \param rules Rules used in the Simulation.
    \param seed Seed used for random number generation.
  */
  Simulation(const Rules &rules, unsigned int seed);

  /**
    Get a list of players and their names.

    \return a list of players and their names.
  */
  const std::map<std::string, Robot> &getPlayers();

  const std::list<Projectile> &getProjectiles();

  /**
    Add a new player.

    \param name The name of the player.
    \param agent The Agent used to controll the player.
    \param position The starting position of the player.
    \param rotation The starting rotation of the player.
  */
  virtual void newPlayer(std::string name, Agent *agent, Vector_d position,
                         double rotation);

  /**
    Add a new player with a random position and rotation.

    \param name The name of the player.
    \param agent The Agent used to controll the player.
  */
  virtual void newPlayer(std::string name, Agent *agent);

  /**
    Run one step of the simmulation.
  */
  virtual void update();

  /**
    Return whether the Simulation is currently running.
    \return whether the Simulation is currently running.
  */
  virtual bool isRunning() const;

  /**
    Get the runtime as a string.
    \return The runtime as a string.
  */
  std::string runtimeString() const;
  /**
    Get the runtime as a number.
    \return The runtime as a number.
  */
  virtual double getRuntime() const;

  /**
    Get the number of player in the game.

    \return the number of player in the game.
  */
  virtual int getNumPlayers() const;

  /**
    Signal raised when a player dies.
    \param (1) the name of the player that died
  */
  Signal<std::string> deathSignal;
  /**
    Signal raised when a new player enters the game.
    \param (1) the name of the new player
  */
  Signal<std::string> newPlayerSignal;
  /**
    Signal raised when two players collide
    \param (1) the name of the first player
    \param (2) the name of the second player
  */
  Signal<std::string, std::string> collisionSignal;
  /**
    Signal raised when a player gets hit by a projectile
    \param (1) the name of the player that got hit.
    \param (2) the name of owner of the projectile.
  */
  Signal<std::string, std::string> hitSignal;
  /**
    Signal raised when a player is out of bounds
    \param (1) the name of the player that  is out of bounds.
  */
  Signal<std::string> outOfBoundsSignal;
  /**
    Signal raised once per simulation step.
  */
  Signal<> simulationStepSignal;

private:
  void updatePlayers();
  void updateProjectiles();

  void check_scan(Robot &robot);
  bool inScanArea(Vector_d const &p1, double rotation,
                  Vector_d const &p2) const;
};

#endif /* end of include guard: __SIMULATION__ */
