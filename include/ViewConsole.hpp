/**
*   \copyright Copyright 2015 Jan-Niklas Braak. This project is released under
* the MIT License, see the file LICENSE.md for rights and limitations.
*   \file View.hpp
*   \author Jan-Niklas Braak
*/

#ifndef __CPPROBOTS_VIEW_CONSOLE__
#define __CPPROBOTS_VIEW_CONSOLE__

class ViewConsole : public View {
  std::shared_ptr<Simulation> simulation;

public:
  void setSimulation(std::shared_ptr<Simulation> sim) override {
    simulation = sim;
  }

  virtual void input() override{};
  virtual void output() override {
    if (!simulation) {
      throw std::runtime_error("No Simulation was set for this View.");
    }
    std::cout << simulation->runtimeString() << std::endl;
    for (auto const &player : simulation->getPlayers()) {
      std::cout << "\t" << player.first << " at " << player.second.getPosition()
                << ", " << player.second.getHealth() << " health" << std::endl;
    }
  };
  virtual void finish() override { std::cin.get(); };
  virtual bool isRunning() const override { return true; };
  void log(std::string text) override {
    if (!simulation) {
      throw std::runtime_error("No Simulation was set for this View.");
    }
    std::cout << simulation->runtimeString() << ": " << text << std::endl;
  }
};

#endif /* end of include guard: __CPPROBOTS_VIEW_CONSOLE__ */
