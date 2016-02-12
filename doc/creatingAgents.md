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

It's important that you run `cmake` not just `make`, except for a header only classes, because make doesn't know about the Agent yet.

## The Simulation ##



### Time and Distance ###

For developing an Agent it's important understand how the Simulation handles time. The Simulation updates everything at a constant rate. Instead of moving smoothly the Robots skip forward every step of the simulation. From the outside it looks smooth because the time between steps is small. An Agent gets to make one Devision every time step.

In the Simulation time is measured in seconds. However one second in the simulation is not always one second in realtime. The Simulation may run slower or faster. Each step of the simulation takes always `timeStep` seconds in simulation time, that simplifies calculation of speeds and other time dependent variables.

Distances are measured in 'units', a unit can't be translated into real live distances without reference. 30 units might mean 30 meters or 30 inch or half a nautical mile. If you want to imagine the Robots as real thing I suggest the conversion 1 unit = 1 cm. A Robot with a default size would be as big as a toy car. The arena would be about the size of a Volleyball field. A Projectile would be the size of a ping pong ball.

The following table shows four parameter ralated to Time and Distance. These parameter depend on your configuration and are stored in Simulation::rules, Robots have references to the Rules.

| Name              | Parameter                                | Default value |
|-------------------|----------------------------------------------|----------|
| Time step         | [timeStep](\ref Rules::timeStep)             | 1 / 60.0 |
| Robot size        | [robot_size](\ref Rules::robot_size)         | {30, 18} |
| Arena size        | [arena_size](\ref Rules::arena_size)    | {1500, 1000}  |
| Projectile size   | [projectile_size](\ref Rules::projectile_size) | {4, 4} |


### Components of the virtual Robot ###

A Robot consists of two basic parts, the body and the turret. The body is a Rectangle and represents the Robots position, rotation and size. If the bodies of two Robots overlap they both take damage. When a Projectile collides with the body of a robot the robot takes damage. To access the body of a Robot the function Robot::getBody() is provided. Robot::getPosition() and Robot::getRotation() are shortcuts to the position and rotation of the body.

The position of turret is always in the center of the Robot. The turret has a rotation relative to the body. The rotation can be accessed by Robot::getTurretAngle(). The direction of the turret influences what the robot can see and in what direction it shoots.



### Health ###


| Name              | Parameter                                | Default value |
|-------------------|----------------------------------------------------|-----|
| Maximal Health    | [max_health](\ref Rules::max_health)               | 100 |
| Projectile damage | [projectile_damage](\ref Rules::projectile_damage) | 5   |
| Collision damage  | [collision_damage](\ref Rules::collision_damage)   | 10  |


### Agents
Your Agent gets a reference to the Robot it controls as the parameter of `update`. Your Agent can use the information given by the Robots public interface to make its decisions.


### Movement ###

Lets look a Robot movement and how an Agent can influence it.
First, what does it mean for a Robot to move? An Object in 2D-Space, like the Robot, can perform two types of movement, translation and rotation.[[1]](https://en.wikipedia.org/wiki/Rigid_transformation) It can change its position and its rotation. Our Robot has an additional restriction, it can not move (translate) sidewards. Different types of vehicles have different restrictions. A car, for example, can't move sidewards and additionally can not turn on a point.<!-- [...](https://en.wikipedia.org/wiki/Parallel_parking) -->
A train is even more constrained, it can only follow a set path. Our Robot is modeled after a differential drive robots, like a autonomous robotic vacuum cleaner or the [Turtle] educational robot.

Additionally the speed and turning rate of the Robot are restricted. The Robot has a maximal forward velocity \f$ v_{max}\f$ a minimal forward velocity \f$ v_{min}\f$ (usualy negativ) and a maximal angular velocity \f$ \omega_{max} \f$. This document uses 'speed' and to refer to 'forward velocity' and 'turn rate' to refer to 'angular velocity'.

| Name                                 | Parameter              | Default value|
|--------------------------------------|--------------------------------|------|
| Maximal velocity \f$v_{max}\f$      | [v_max](\ref Rules::v_max)     | 100  |
| Minimal velocity \f$v_{min}\f$      | [v_min](\ref Rules::v_min)     | -30  |
| Maximal turning rate \f$\omega_{max}\f$ | [w_max](\ref Rules::w_max) | 0.6  |

Acceleration is not restricted in the Simulation.

<!-- TODO: Image of position and movement.  -->



### Targeting ###

<!-- Now that we can move lets try interacting with our environment in a more meaningful way. To do that we will make our Robot 'see' other Robots. First it is important to know how vision works in the simulation. -->

Each Robot has a arc of vision centered on its turret. That means the Robot always looks in the direction of the turret. The Vision of the Robot is limited by the scan range and scan angle.
In addition the Robot senses targets in its immediate proximity, independent of the angle.
These variables depend on your configuration and are stored in Simulation::rules. The following table shows their default values.

| Name           | Parameter                                    | Default value|
|----------------|-----------------------------------------------|-------------|
| Scan range     | [scan_range](\ref Rules::scan_range)          | 1000        |
| Scan angle     | [scan_angle](\ref Rules::scan_angle)| \f$ \pi/3\f$  |
| Scan proximity | [scan_proximity](\ref Rules::scan_proximity)  | 100         |


<!--
TODO:  An Image of the arc of vision would be cool.
 -->

To find a target the Agent can use one of the 'scan' functions provided by Robot. The functions [scanClosest](@ref Robot::scanClosest), [scanAny](@ref Robot::scanAny) and [scanAll](@ref Robot::scanAll) are provided.
[scanClosest](@ref Robot::scanClosest) returns a pointer to the closest target in the robots arc of vision.
[scanAny](@ref Robot::scanAny) returns a pointer to any robot, this method is more efficient than [scanClosest](@ref Robot::scanClosest).
[scanAll](@ref Robot::scanAll) returns a list of Robots so the Agent can search them itself.

### Shooting ###

| Name           | Parameter                                    | Default value|
|----------------|-----------------------------------------------|-------------|
| Turret turn rate | [turret_w_max](\ref Rules::turret_w_max)    | 3.14        |
| Projectile speed  | [projectile_speed](\ref Rules::projectile_speed)| 1000  |
| Projectile cooldown | [projectile_cooldown](\ref Rules::projectile_cooldown) | 0.4|

## Examples


### Orbiter ###
The following code is from the Orbiter class, it demonstrates how to drive with a constant speed and turn rate. Driving with constant seed and turn rate makes the Robot drive in a circle.

~~~{.cpp}
Action Orbiter::update(Robot const &r) {
  (void)r; // suppress unused parameter warning
  return {v, w, 0, false};
}
~~~

The Orbiter overloads the Agent::update(). The speed `v` and the turn rate `w` are members of the Orbiter class. They are constant doubles and are set in the constructor of Orbiter. The Agent::update() method returns an Action. In the Orbiter the Action is constructed with the constant speed and turn rate, a `turretAngle` of 0 and `shooting` set to `false`. We will see what `turretAngle` and `shooting` does in future examples.


### Wanderer ###

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

Here `v`, `w` and `random()` are members of the Wanderer class. The speed `v` is a constant. The `random()` function is created using the [pseudo-random number generation library](http://en.cppreference.com/w/cpp/numeric/random).

Each time Step a random value is added to the turn rate `w`, that way `w` changes gradually over time. In addition the value of `w` is [clamped](\ref clamp) to the maximal turn rate to avoid the integral windup problem.


### Follower  ###
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

`target_robot` holds the std::shared_ptr, that is returned by `scanClosest()`. If the pointer is Null we turn on the spot by having zero velocity and constant turn rate. The turn rate we choose makes sure that we never turn more than [scan_angle](\ref Rules::scan_angle) each time step. The actual turning rate may be lower than what we request depending on the value of [w_max](\ref Rules::w_max).

If `target_robot` is not Null it contains the closest target and we can use it.
At first we use vector math to find the vector from our position to the target and store it in `deltaPosition`. The magnitude of `deltaPosition` is our distance from the target. Its angle is the direction in which we must turn to face the target. The Vector class helps us with that.

Next we will calculate the distance error and the angle error. The distance error tells us how far away we are from the desired distance. It can be calculated by subtracting the target distance from the actual distance. The distance error is zero when we are at the desired distance, positive if we are to far away from the target and negative if we are to close.
The angle error is similar, it tells us how far we have to turn and in what direction. It is the difference between the desired angle and our current rotation.  However we can't use simple subtraction to calculate it. The distance between two angles can have two values, one for clockwise and one for counterclockwise, we want the smallest of the two. We use angDiffRadians() for that.

Finally we use two Proportional controllers to minimize the distance and angle error. A Proportional controller multiplies the error of a system with a constant factor and feeds it back into the system. In our system the speed is proportional to the distance error. That implies the following behavior:
* the further away we are the faster we drive,
* the closer we are the slower we drive,
* if we are at the target distance the speed is zero,
* and if the error is negative we drive backwards.

As long we are pointing at the target that is exactly the correct behavior. To ensure thats the case we use the second Proportional controller. The turning rate is proportional to the angle error, which means that we always turn in the direction of the target.

See [Math](@ref math) for more information on vectors, angles and general utility functions used in CppRobots.

### Sniper ###

<!--
TODO: Introductory text
TODO: shooting
-->

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

  const auto target_position = target_robot->getPosition();
  const auto delta_position = target_position - position;

  const auto turret_angle = wrapRadians(delta_position.angle() - rotation);

  const auto turret_error = angDiffRadians(turret_angle, r.getTurretAngle());
  const auto shooting = fabs(turret_error) < 0.01;

  return {0, 0, turret_angle, shooting};
}
~~~

The beginning of Sniper::update is similar to Follower::update. We scan for the closest target and if it doesn't exist we look for one. But instead of turning the body of the robot we turn the turret. We use wrapRadians() so the result is always an angle between \f$ 0 \f$ and \f$ 2\pi\f$.

Next we calculate the angle that the turret must have to point at the target. We calculate the absolute direction of the target the same way we did for the Follower. Then we subtract the rotation of the robot to get the direction relative to the Robots body.

We also see if the the turret is already pointing at the target, and if so we shoot. To do that we calculate the angular difference between the desired and the actual turret angle. if the absolute value of that error is smaller than a allowable deviation we shoot.

### Hunter ###

In this last example we use everything we learned so far to create a more advanced Robot.

~~~{.cpp}
Action Hunter::update(Robot const &r) {

  const auto target_robot = r.scanClosest();
  const auto position = r.getPosition();
  const auto rotation = r.getRotation();

  // TODO: seperate movement and shooting code. Add Wall avoidance.
  double turret_angle;
  Vector_d delta_position;

  if (!target_robot) {
    // Set the center of the arena as target.
    const auto targetPosition = r.rules.arena_size / 2.0;
    delta_position = targetPosition - position;

    // Rotate the Turret to scan for targets
    turret_angle = r.getTurretAngle() + r.rules.scan_angle;
  } else {
    // Set the found Robot as Target.
    const auto targetPosition = target_robot->getPosition();
    delta_position = targetPosition - position;

    // Aim at target
    turret_angle = delta_position.angle() - rotation;
  }

  // turn perpandicular to the target
  const auto perp = Vector_d(delta_position).rotate(M_PI / 2);
  const auto perp_error = angDiffRadians(perp.angle(), rotation);

  // turn toward the target
  const auto straight_error = angDiffRadians(delta_position.angle(), rotation);

  // How far are we away from the target distance, as a value from -1 to 1.
  const auto distance_error = std::min(
      (delta_position.magnitude() - targetDistance) / targetDistance, 1.0);

  // Put it all together. Linearly interpolate between driving toward the target
  // and driving perpendicular to it depending on the distance. So that at the
  // target distance we drive fully perpendicular, if we are farher away we
  // drive toward the target and if we are to close we drive away from the
  // tarrget (due to the negatice sign of the distance error)
  const auto w_perp = perp_error * K_perp;
  const auto w_straight = straight_error * K_straight;
  const auto w = lerp(w_perp, w_straight, distance_error);

  // Is the target in front of the turret (within 0.01rad)? If yes then shoot.
  const auto turret_error = angDiffRadians(turret_angle, r.getTurretAngle());
  const auto shooting = fabs(turret_error) < 0.01 && target_robot;

  // drive at full speed.
  const auto v = r.rules.v_max;

  return {v, w, turret_angle, shooting};
}
~~~


<!--
The only thing new here is lerp(). The lerp() function interpolates linearly between two values. -->


## Ideas for Robots

* Improve Sniper with better targeting (Deflection).
* Add wall avoidance to the Wanderer.
* Add Robot avoidance to the Wanderer.
* 'Bouncer' that drives from wall to wall.
* A variation of [Boids].


[Turtle]: https://en.wikipedia.org/wiki/Turtle_(robot)
[Boids]: http://www.red3d.com/cwr/boids/
