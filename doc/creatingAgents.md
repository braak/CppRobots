# Creating an Agent #

**Warning: This Text is incomplete and may contain errors.**

<!--TODO: Introduction  -->

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

<!--TODO: slightly longer version, which explains what happens (lib, etc.).-->
## The Simulation ##

The following sections show how different parts of the simulation operate.

### Time and Distance ###

For developing an Agent it's important understand how the Simulation handles time. The Simulation updates everything at a constant rate. Instead of moving smoothly the Robots skip forward every step of the simulation. From the outside it looks smooth because the time between steps is small.

In the Simulation time is measured seconds. Each step of the simulation takes always `timeStep` seconds in simulation time, that simplifies calculation of speeds and other time dependent variables. However one second in the simulation is not always one second in real time, the Simulation may run slower or faster.

Distances are measured in 'units', a unit can't be translated into real live distances without reference. 30 units might mean 30 meters or 30 inch or half a nautical mile. If you want to imagine the Robots as real thing I suggest the conversion 1 unit = 1 cm. A Robot with a default size would be as big as a toy car and have a top speed of 3.6 km/h. The arena would be about the size of a Volleyball field. A Projectile would be the size of a ping pong ball with a speed of 36 km/h.

The translation from units to pixels is also arbitrary. For displaying a simple low resolution scene the conversion 1 unit = 1 pixel is reasonable. The ViewSFML uses that conversion.

All angles are measured in radians. Which allows the use of trigonometric function from the C standard library.

The following table shows the four parameter related to Time and Distance. These parameter depend on your configuration and are stored in Simulation::rules, Robots have references to the Rules.

| Name              | Parameter                | Default value |
|-------------------|--------------------------|---------------|
| Time step         | Rules::timeStep          | 1 / 60.0      |
| Robot size        | Rules::robot_size        | (30, 18)      |
| Arena size        | Rules::arena_size        | (1500, 1000)  |
| Projectile size   | Rules::projectile_size   | (4, 4)        |

<!-- TODO: Time step -> Sample Time  -->

<!--Derived units: u/s, rad/s, u²-->

### Agents and Actions

An Agent gets a reference to the Robot it controls as the parameter of `update`. The Agent can use the information given by the Robots public interface to make its decisions. The decisions it makes are returned in the form of of a Action.

<!--TODO: Abstract Agent  -->

An Action consists of the desired speed `v`, the desired turning rate `w`, the desired angle of the turret `turretAngle`, and the boolean value `shooting`, which says whether to shoot or not.

~~~~{.cpp}
struct Action {
  double v;
  double w;
  double turretAngle;
  bool shooting;
};
~~~~


### Components of the virtual Robot ###

A Robot consists of two basic parts, the body and the turret. The body is a Rectangle and represents the Robots position, rotation and size. If the bodies of two Robots overlap they both take damage. When a Projectile collides with the body of a robot the robot takes damage. To access the body of a Robot the function Robot::getBody() is provided. Robot::getPosition() and Robot::getRotation() are shortcuts to the position and rotation of the body.

The position of the turret is always at the center of the Robot. The turret has a rotation relative to the body. The direction of the turret influences what the robot can see and in what direction it shoots. The rotation can be accessed by Robot::getTurretAngle().


### Movement ###

Lets look a Robot movement and how an Agent can influence it.
First, what does it mean for a Robot to move? An Object in 2D-Space, like the Robot, can perform two types of movement, translation and rotation.[<sup>[1]</sup>](https://en.wikipedia.org/wiki/Rigid_transformation) In other words it can change its position and its rotation. Our Robot has an additional restriction, it can not move sidewards. Different types of vehicles have different restrictions. A car, for example, can't move sidewards and additionally can not turn on a point.<!-- [...](https://en.wikipedia.org/wiki/Parallel_parking) -->
A train is even more constrained, it can only follow a set path. Our Robot is modeled after a differential drive robots, like a autonomous robotic vacuum cleaner or the [Turtle] educational robot.

Additionally the speed and turning rate of the Robot are restricted. The Robot has a maximal forward velocity v_max a minimal forward velocity v_min (usualy negativ) and a maximal angular velocity w_max. This document uses 'speed' and to refer to 'forward velocity' and 'turn rate' to refer to 'angular velocity'.

| Name                          | Parameter       | Default value|
|-------------------------------|-----------------|--------------|
| Maximal speed                 | Rules::v_max    | 100          |
| Minimal speed                 | Rules::v_min    | -30          |
| Maximal turning rate          | Rules::w_max    | 0.6          |

Acceleration is not restricted in the Simulation. A Robot can accelerate and decelerate as fast as it wants.

<!-- TODO: Image of position and movement.  -->


### Health ###

Every Robot has a an amount of 'Health', when a Robot takes damage its Health is reduced. If its Health reaches zero it is removed from the Simulation. there a re three ways in which a Robot can be damaged.

1. Colliding with another Robot.
2. Driving outside the arena.
3. Being shot.

Robots are allowed to pass through each other, but they take damage each time step they overlap. In CppRobots we call that a collision, even if it is not very realistic.  Driving outside the arena has the same effect as colliding with a Robot.

When a Robot is hit by a Projectile it takes damage instantly. The amount of damage a projectile deals a constant value.

The following table shows the four parameter related to Health, defined in Rules.
<!-- These parameter depend on your configuration and are stored in Simulation::rules, Robots have references to the Rules. -->

| Name              | Parameter                   | Default value |
|-------------------|-----------------------------|---------------|
| Maximal Health    | Rules::max_health           | 100           |
| Projectile damage | Rules::projectile_damage    | 5             |
| Collision damage  | Rules::collision_damage     | 10            |


### Targeting ###

Each Robot has a arc of vision centered on its turret. That means the Robot always looks in the direction of the turret. The turret can be rotated, but its turn rate is limited.
The Vision of the Robot is limited by the scan range and scan angle.
In addition the Robot senses targets in its immediate proximity, independent of the angle.

These variables depend on your configuration and are stored in Simulation::rules. The following table shows their default values.

| Name              | Parameter               | Default value |
|-------------------|-------------------------|---------------|
| Scan range        | Rules::scan_range       | 1000          |
| Scan angle        | Rules::scan_angle       | Pi/3          |
| Scan proximity    | Rules::scan_proximity   | 100           |
| Turret turn rate  | Rules::turret_w_max     | 3.14          |

<!--
TODO:  An Image of the arc of vision would be cool.
 -->

To find a target the Agent can use one of the 'scan' functions provided by the Robot. The functions Robot::scanClosest, Robot::scanAny and Robot::scanAll are provided.
Robot::scanClosest returns a pointer to the closest target in the robots arc of vision.
Robot::scanAny returns a pointer to any visible robot, this method is more efficient than Robot::scanClosest.
Robot::scanAll returns a list of visible Robots so the Agent can search them itself.


### Shooting ###

A Robot can fire a Projectile. After shooting it has to wait a cooldown period before it can fire again. Projectiles move with a constant speed.

| Name           | Parameter                        | Default value |
|---------------------|-----------------------------|---------------|
| Projectile speed    | Rules::projectile_speed     | 1000          |
| Projectile cooldown | Rules::projectile_cooldown  | 0.4           |


## Examples

The following sections show examples of Agents. The complete (and up to date) source code of each example Agent can be found in the 'src/Agent' and 'include/Agent' folder.

Use these examples to guide your own experimentation with Agents. Look at the example and see what you can learn. Change a few things and see what it does. If you are stuck look at an earlier example or the basic description of the simulation to see if you missed something.


### Orbiter ###

The following code is from the Orbiter class, it demonstrates how to drive with a constant speed and turn rate. Driving with constant speed and turn rate makes the Robot drive in a circle.

~~~{.cpp}
Action Orbiter::update(Robot const &r) {
  (void)r; // suppress unused parameter warning
  return {v, w, 0, false};
}
~~~

The Orbiter overloads Agent::update(). The speed `v` and the turn rate `w` are members of the Orbiter class. They are constant doubles and are set in the constructor of Orbiter. The Agent::update() method returns an Action. In the Orbiter the Action is constructed with the constant speed and turn rate, a `turretAngle` of 0 and `shooting` set to `false`. We will see how we can use `turretAngle` and `shooting` in future examples.


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

Here `v`, `w` and `random()` are members of the Wanderer class. The speed `v` is a constant. The `random()` function is created using the [pseudo-random number generation library](http://en.cppreference.com/w/cpp/numeric/random), it generates a random value using Gaussian distribution with a standard deviation specified in the constructor of Wanderer.

Each time Step a random value is added to the turn rate `w`, that way `w` changes gradually over time. If we just set the turn rate to a random value it would quickly jump from one value to the next.

In addition the value of `w` is clamped (using the clamp() function) to the maximal turn rate to avoid the integral windup problem. That way the turn rate does not get 'stuck' on one extreme. If we leave this part out the Robot sometimes drives in a circle with maximal turn rate for a while until the internal `w` unwinds.[<sup>[2]</sup>](http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-reset-windup/)


### Follower  ###

In the following example we will use Robot::scanClosest to find the closest visible Robot and follow it. We will try to keep a constant distance so we don't crash into the target. If we can't see any Robot we will turn on the spot until we find one.

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
  const auto delta_position = target_position - position;

  // calculate the error value of our distance to the desired distance.
  const auto distance_error = delta_position.magnitude() - target_distance;
  // calculate the error value of our rotation to the desired rotation.
  const auto angle_error = angDiffRadians(delta_position.angle(), rotation);

  // Apply simple proportional controllers to our errors. Using K_distance and
  // K_angle as gain
  const auto v = K_distance * distance_error;
  const auto w = K_angle * angle_error;

  return {v, w, 0, false};
}
~~~~

`target_robot` holds the std::shared_ptr, that is returned by `scanClosest()`. If the pointer is Null we turn on the spot by having a constant turn rate and no  speed. The turn rate we choose makes sure that we never turn more than Rules::scan_angle each time step. The actual turning rate may be lower than what we request depending on the value of Rules::w_max.

If `target_robot` is not Null it contains the closest target and we can use it.
At first we use vector math to find the vector from our position to the target and store it in `delta_position`. The magnitude of `delta_position` is our distance from the target. Its angle is the direction in which we must turn to face the target. The Vector class helps us with the calculations.

Next we will calculate the `distance_error` and the `angle_error`. The `distance_error` tells us how far away we are from the desired distance. It can be calculated by subtracting the target distance from the actual distance. The `distance_error` is zero when we are at the desired distance, positive if we are to far away from the target and negative if we are to close.
The `angle_error` is similar, it tells us how far we have to turn and in what direction. It is the difference between the desired angle and our current rotation.  However we can't use simple subtraction to calculate it. The distance between two angles can have two values, one for clockwise and one for counterclockwise, we want the smallest of the two. We use angDiffRadians() for that.

Finally we use two proportional controllers to minimize the distance and `angle_error`. A proportional controller multiplies the error of a system with a constant factor (the gain) and feeds it back into the system. In our system the speed is proportional to the `distance_error`. That implies the following behavior:

* the further away we are the faster we drive,
* the closer we are the slower we drive,
* if we are at the target distance the speed is zero,
* and if we are to close (the error is negative) we drive backwards.

As long we are pointing at the target that is exactly the correct behavior. To ensure we always point at the target we use the second proportional controller. The turning rate is proportional to the `angle_error`, which means that we always turn in the direction of the target. We turn faster the further we are away from the target direction and slow down when we approach it.

See [Math](@ref math) for more information on vectors, angles and general utility functions used in CppRobots.

### Sniper ###


The Sniper Agent does not move, it simply turns its turret until it sees a target and shoots it.

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

The beginning of Sniper::update is similar to Follower::update. We scan for the closest target and if it doesn't exist we look for one. But instead of turning the body of the robot we turn the turret. We use wrapRadians() so the result is always an angle between 0 and 2 Pi (0° and 360°).

Next we calculate the angle that the turret must have to point at the target. We calculate the absolute direction of the target the same way we did for the Follower. Then we subtract the rotation of the robot to get the direction relative to the Robots body, which is the angle the turret should have to point at the target.

<!-- TODO: a Picture of the turret, target and body -->

We also see if the the turret is already pointing at the target, and if so we shoot. To do that we calculate the angular difference between the desired and the actual turret angle. If the absolute value of that error is smaller than a allowable deviation we shoot.


### Hunter ###

In this last example we use everything we learned so far to create a more advanced Robot. We apply a new technique for dealing with more than one goal.

~~~{.cpp}
Action Hunter::update(Robot const &r) {
  const auto target_robot = r.scanClosest();
  const auto position = r.getPosition();
  const auto rotation = r.getRotation();


  double turret_angle;
  Vector_d delta_position;

  if (!target_robot) {
    // Set the center of the arena as target.
    const auto target_position = r.rules.arena_size / 2.0;
    delta_position = target_position - position;

    // Rotate the Turret to scan for targets
    turret_angle = r.getTurretAngle() + r.rules.scan_angle;
  } else {
    // Set the found Robot as Target.
    const auto target_position = target_robot->getPosition();
    delta_position = target_position - position;

    // Aim at target
    turret_angle = delta_position.angle() - rotation;
  }

  // calculate w for the two rules.
  const auto w_toward = turn_toward(delta_position.angle(), rotation);
  const auto w_perp = turn_perpendicular(delta_position.angle(), rotation);

  // How far are we away from the target distance, as a value from -1 to 1.
  const auto distance_error =
      clamp(delta_position.magnitude() / targetDistance - 1.0, -1.0, 1.0);

  /*
   * Put it all together. Linearly interpolate between driving toward the target
   * and driving perpendicular to it depending on the distance. So that at the
   * target distance we drive fully perpendicular, if we are further away we
   * drive toward the target and if we are to close we drive away from the
   * target (due to the negative sign of the distance_error).
   */
  const auto w = lerp(w_perp, w_toward, distance_error);

  // Is the target in front of the turret (within 0.01rad)? If yes then shoot.
  const auto turret_error = angDiffRadians(turret_angle, r.getTurretAngle());
  const auto shooting = fabs(turret_error) < 0.01 && target_robot;

  // drive at full speed.
  const auto v = r.rules.v_max;

  return {v, w, turret_angle, shooting};
}

double Hunter::turn_perpendicular(double angle, double rotation) const {
  // turn perpandicular to the target
  const auto perp_error1 = angDiffRadians(angle + M_PI / 2, rotation);
  const auto perp_error2 = angDiffRadians(angle - M_PI / 2, rotation);
  const auto perp_error =
      fabs(perp_error1) < fabs(perp_error2) ? perp_error1 : perp_error2;
  const auto w = perp_error * K_perp;

  return w;
}

double Hunter::turn_toward(double angle, double rotation) const {
  const auto straight_error = angDiffRadians(angle, rotation);
  const auto w = straight_error * K_straight;

  return w;
}
~~~

In the Beginning the Agent performs a scan. Depending on whether or not we have a target we set `target_position` and `turret_angle` as normal or we use a fall back strategy. The fall back is to set the center of the arena as the target position and turn the turret to scan for targets. The rest of the function works as if we found a target.

The Hunter performs two task for driving.

1. Drive toward the target, so it is easier to shoot.
2. Drive perpendicular to the target to avoid being shot.

As you probably realized the two task are not compatible with each other. How can you drive toward something and perpendicular to it at the same time.
Fortunately you don't have to do both at the same time? We can drive toward the target when we are far away and we can drive perpendicular when we are closer.

We can mix both task based on the distance from the target. But how do we do that? The easiest way would be to switch from one rule to the other depending on the distance. The following pseudo code shows how that would look.
~~~{.cpp}
if(distance < target_distance)
  do_perpendicular();
else
  do_toward();
~~~

This is actually a primitive example of a hard coded [Decision tree]. This structure can get very complex with more tasks and decisions. However with the right tools it is a useful method.

We will not use this method, instead we will mix the tasks directly. To understand how that works we will first look at the individual tasks.

To drive toward the target we do the same thing as the Wanderer, except we ignore the speed. We calculate the `toward_error` by finding the angle between our rotation and the rotation of a vector pointing toward the target. Then we
calculate the desired turn rate `w_toward` by multiplying the error with a constant.

Driving perpendicular to the target is similar. The target angle is the angle of the vector pointing toward the target plus or minus 90° (pi/2). `perp_error` is the difference between that angle and our rotation. Because we have two choices to turn we choose the one that produces the smaller absolute error. And `w_perp` is that error multiplied with a constant.

How does that help us combine the two tasks? `w_toward` and `w_perp` are numbers, which means we can perform mathematical operation on them. In particular, we can interpolate between them. By interpolating between the two values we can smoothly switch from one task to the other. The interpolation amount is based on the deviation from the target distance. We call this value the `distance_error`. It is the relative error of the actual distance to the desired distance. The lerp() function does the linear interpolation for us.


## Ideas for Robots

* Improve Sniper or Hunter with better targeting (Deflection).
* Add wall avoidance to the Wanderer or any other Agent.
* Add Robot avoidance to the Wanderer or any other Agent.
* 'Bouncer' that drives from wall to wall.
* A variation of [Boids].
* A 'Visitor', that visits every place in the Arena, like robotic vacuum cleaner.
* Lower `scan_range` to make a short sighted Robot. Create a 'Seeker' Agent, that systematically searches the Arena for targets.


[Turtle]: https://en.wikipedia.org/wiki/Turtle_(robot)
[Boids]: http://www.red3d.com/cwr/boids/
[Behavior tree]: https://en.wikipedia.org/wiki/Behavior_tree_(artificial_intelligence,_robotics_and_control)
