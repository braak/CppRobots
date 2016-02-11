# Creating an Agent #

**Warning: This Text is incomplete and may contain errors.**

Before trying to create your own Agent compile an run the Program at least once. See [README](README.md) for instructions on how to do that.

## The short version

* create a Class that inherits from Agent
* place the header file in `include/Agents`
* place the source file in `src/Agents`
* overload the `update` method and return a valid `Action`.
* modify `robots.cpp` so it uses your Agent.
* rerun cmake
* rebuild the project

It's important that you run `cmake` not just `make`, because make doesn't know about your Agent yet.

## Robots ##

Your Agent gets a reference to the Robot it controls as the parameter of `update`. Your Agent can use the information given by the Robots public interface to make its decisions.

For developing an Agent it's important understand how the Simulation handles time. The Simulation updates everything at a constant rate. Instead of moving smoothly the Robots skip forward every step of the simulation. From the outside it looks smooth because the time between steps is small, in most cases 33.3 milliseconds or less. An Agent gets to make one Devision every time step.


<!-- Time step -->

### Components of the virtual Robot ###

* Body
* Turret


## Movement ##

Lets look a Robot movement and how an Agent can influence it.
First, what does it mean for a Robot to move? An Object in 2D-Space, like the Robot, can perform two types of movement, translation and rotation.[[1]](https://en.wikipedia.org/wiki/Rigid_transformation) It can change its position and its rotation. Our Robot has an additional restriction, it can not move (translate) sidewards. Different types of vehicles have different restrictions. A car, for example, can't move sidewards and additionally can not turn on a point.<!-- [...](https://en.wikipedia.org/wiki/Parallel_parking) -->
A train is even more constrained, it can only follow a set path. Our Robot is modeled after a differential drive robots, like a autonomous robotic vacuum cleaner or the [Turtle] educational robot.

Additionally the speed and turning rate of the Robot are restricted. The Robot has a maximal forward velocity \f$ v_{max}\f$ a minimal forward velocity \f$ v_{min}\f$ (usualy negativ) and a maximal angular velocity \f$ \omega_{max} \f$. These variables depend on your configuration and are stored in Simulation::rules, Robots have  references to the Rules. This document uses 'speed' and to refer to 'forward velocity' and 'turn rate' to refer to 'angular velocity'.

| name                                 | variable               | default value|
|--------------------------------------|--------------------------------|------|
| Maximal velocity \f$ v_{max}\f$      | [v_max](\ref Rules::v_max)     | 100  |
| Minimal velocity \f$ v_{min}\f$      | [v_min](\ref Rules::v_min)     | -30  |
| Maximal turning rate \f$ \omega_{max}\f$ | [w_max](\ref Rules::w_max) | 0.6  |

Acceleration is not restricted in the Simulation.

<!-- TODO: Image of position and movement.  -->

The following code is from the Orbiter class, it demonstrates how to drive with a constant speed and turn rate. Driving with constant seed and turn rate makes the Robot drive in a circle.

~~~{.cpp}
Action Orbiter::update(Robot const &r) {
  (void)r; // surpress unused parameter warning
  return {v, w, 0, false};
}
~~~

The Orbiter overloads the `update` method of Agent. The speed `v` and the turn rate `w` are members of the Orbiter class. They are constant doubles and are set in the constructor of Orbiter. The `update` method returns an Action. In the Orbiter the Action is constructed with the constant speed and turn rate, a `turretAngle` of 0 and `shooting` set to `false`. We will see what `turretAngle` and `shooting` does in future examples.

Driving in a circle is not very interesting. By modifying the speed and/or turn rate over time we can do something smarter. The following code is taken from the Wanderer Agent. It makes the Robot randomly change direction over time. The speed is still constant, but the turn rate depend on a random function.

~~~{.cpp}
Action Wanderer::update(Robot const &r) {
  // integrate a random number
  w += random();
  // prevent windup by limiting the turning rate to the maximal output.
  w = clamp(w, -r.rules.w_max, r.rules.w_max);
  return {v, w, 0, false};
}
~~~

Here `v`, `w` and the `random` function are members of the Wanderer class. The speed `v` is a constant. The `random` function is created using the [pseudo-random number generation library](http://en.cppreference.com/w/cpp/numeric/random).

Each time Step a random value is added to the turn rate `w`, that way `w` changes gradually over time. In addition the value of `w` is [clamped](\ref clamp) to the maximal turn rate to avoid the integral windup problem.

## Targeting ##

Now that we can move lets try interacting with our environment in a more meaningful way. To do that we will make our Robot 'see' other Robots. First it is important to know how vision works in the simulation.

Each Robot has a arc of vision centered on its turret. That means the Robot always looks in the direction of the turret. The Vision of the Robot is limited by the scan range and scan angle.
In addition the Robot senses targets in its immediate proximity, independent of the angle.
These variables depend on your configuration and are stored in Simulation::rules. The following table shows their default values.

| name           | variable                                     | default value|
|----------------|-----------------------------------------------|-------------|
| Scan range     | [scan_range](\ref Rules::scan_range)          | 1000        |
| Scan angle     | [scan_angle](\ref Rules::scan_angle)| \f$ \frac{\pi}{3}\f$  |
| Scan proximity | [scan_proximity](\ref Rules::scan_proximity)  | 100         |

<!--
TODO:  An Image of the arc of vision would be cool.
 -->

To find a target the Agent can use one of the 'scan' functions provided by Robot. The functions [scanClosest](@ref Robot::scanClosest), [scanAny](@ref Robot::scanAny) and [scanAll](@ref Robot::scanAll) are provided.
[scanClosest](@ref Robot::scanClosest) returns a pointer to the closest target in the robots arc of vision.
[scanAny](@ref Robot::scanAny) returns a pointer to any robot, this method is more efficient than [scanClosest](@ref Robot::scanClosest).
[scanAll](@ref Robot::scanAll) returns a list of Robots so the Agent can search them itself.

In the following example we will use [scanClosest](@ref Robot::scanClosest) to find the closest visible Robot and follow it. We will try to keep a constant distance so we don't crash into the target. If we can't see any Robot we will turn on the spot until we see one.

~~~~{.cpp}
Action Follower::update(Robot const &r) {
  auto target_robot = r.scanClosest();

  if (!target_robot) {
    // If no visible Robot, turn in circle.
    const auto w = -r.rules.scan_angle / r.rules.timeStep;
    return {0, w, 0, false};
  }

  // get our position and rotation.
  const auto position = r.getPosition();
  const auto rotation = r.getRotation();

  // get our targets position
  const auto target_position = target_robot->getPosition();
  // calculate vector from our position to the target.
  const auto deltaPosition = target_position - position;

  // calculate the error value of our distance to desired distance.
  const auto distance_error = deltaPosition.magnitude() - target_distance;
  // calculate the error value of our rotation to desired rotation.
  const auto angle_error = angDiffRadians(deltaPosition.angle(), rotation);

  // Apply simple proportional controllers to our errors. Using K_distance and
  // K_angle as gain
  const auto v = K_distance * distance_error;
  const auto w = K_angle * angle_error;

  return {v, w, 0, false};
}
~~~~

`target_robot` holds the std::shared_ptr, that is returned by `scanClosest`. If the pointer is Null we turn on the spot by having zero velocity and constant turn rate. The turn rate we choose makes sure that we never turn more than [scan_angle](\ref Rules::scan_angle) each time step. The actual turning rate may be lower than what we request depending on the value of [w_max](\ref Rules::w_max).

If `target_robot` is not Null it contains the closest target and we can use it.
At first we use vector math to find the vector from our position to the target and store it in `deltaPosition`. The magnitude of `deltaPosition` is our distance from the target. Its angle is the direction in which we must turn to face the target. The Vector class helps us with that.

Next we will calculate the distance error and the angle error. The distance error tells us how far away we are from the desired distance. It can be calculated by subtracting the target distance from the actual distance. The distance error is zero when we are at the desired distance, positive if we are to far away from the target and negative if we are to close.
The angle error is similar, it tells us how far we have to turn and in what direction. It is the difference between the desired angle and our current rotation.  However we can't use simple subtraction to calculate it. The distance between two angles can have two values, one for clockwise and one for counterclockwise, we want the smallest of the two. We use angDiffRadians for that.

Finally we use two Proportional controllers to minimize the distance and angle error. A Proportional controller multiplies the error of a system with a constant factor and feeds it back into the system. In our system the speed is proportional to the distance error. That implies the following behavior:
* the further away we are the faster we drive,
* the closer we are the slower we drive,
* if we are at the target distance the speed is zero,
* and if the error is negative we drive backwards.

As long we are pointing at the target that is exactly the correct behavior. To ensure thats the case we use the second Proportional controller. The turning rate is proportional to the angle error, which means that we always turn in the direction of the target.

See [Math.md](Math.md) for more information on vectors, angles and general utility functions used in CppRobots.

<!--
TODO: getter
-->


## Shooting ##



~~~{.cpp}
Action Sniper::update(Robot const &r) {
  const auto target_robot = r.scanClosest();

  if (!target_robot) {
    const auto turretAngle =
        wrapRadians(r.getTurretAngle() + r.rules.scan_angle);
    return {0, 0, turretAngle, false};
  }

  const auto position = r.getPosition();
  const auto rotation = r.getRotation();

  const auto targetPosition = target_robot->getPosition();
  const auto deltaPosition = targetPosition - position;

  const auto turretAngle = wrapRadians(deltaPosition.angle() - rotation);
  const auto turret_error = angDiffRadians(turretAngle, r.getTurretAngle());
  const auto shooting = fabs(turret_error) < 0.01;

  return {0, 0, angle, shooting};
}
~~~

<!-- const auto turret_angle = wrapRadians(rotation + r.getTurretAngle());
const auto error = abs(angDiffRadians(turret_angle, deltaPosition.angle())); -->

<!--
TODO: turning turret
TODO: shooting
-->

<!--
TODO: double max_health;
TODO: double projectile_speed;  
TODO: double projectile_cooldown;
TODO: double projectile_damage;
TODO: double turret_w_max;      

TODO: arena size
-->



## Advanced Agent ##


~~~{.cpp}
Action Hunter::update(Robot const &r) {

  const auto targetRobot = r.scanClosest();
  const auto position = r.getPosition();
  const auto rotation = r.getRotation();

  double turretAngle;
  Vector_d deltaPosition;
  if (!targetRobot) {
    const auto targetPosition = r.rules.arena_size / 2.0;

    deltaPosition = targetPosition - position;

    turretAngle = r.getTurretAngle() + r.rules.scan_angle;
  } else {
    const auto targetPosition = targetRobot->getPosition();

    deltaPosition = targetPosition - position;

    turretAngle = deltaPosition.angle() - rotation;
  }

  // turn perpandicular to the target
  const auto perp = Vector_d(deltaPosition).rotate(M_PI / 2);
  const auto beta_error = angDiffRadians(perp.angle(), rotation);

  // turn toward the target
  const auto angle_error = angDiffRadians(deltaPosition.angle(), rotation);

  // How far are we away from the target distance, as a value from -1 to 1.
  const auto distance_error = std::min(
      (deltaPosition.magnitude() - targetDistance) / targetDistance, 1.0);

  // Put it all together. Linearly interpolate between driving toward the target
  // and driving perpendicular to it depending on the distance.
  const auto w =
      lerp(beta_error * K_beta, angle_error * K_distance, distance_error);

  // Is the target in front of the turret (within 0.01rad)? if yes then shoot.
  const auto shooting =
      abs(angDiffRadians(turretAngle, r.getTurretAngle())) < 0.01 &&
      targetRobot;

  // drive at full speed.
  const auto v = r.rules.v_max;
  return {v, w, turretAngle, shooting};
}
~~~

## Ideas for Robots

* Improve Sniper with better targeting (Deflection).
* Add wall avoidance to the Wanderer.
* Add Robot avoidance to the Wanderer.
* 'Bouncer' that drives from wall to wall.
* [Boids].


[Turtle]: https://en.wikipedia.org/wiki/Turtle_(robot)
[Boids]: http://www.red3d.com/cwr/boids/
