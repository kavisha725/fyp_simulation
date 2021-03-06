# fyp_simulation
Gazebo ROS packages for the simulation used in our final year project. The simulation was tested on Ubuntu 16.04LTS and ROS Kinetic.

---

## Robot Platform

For the two-wheeled self-balancing platform, the openly
available [RoboSavvy](http://wiki.ros.org/Robots/RoboSavvy-Balance) platform was used. This platform
contained the 3D model of the platform and the ability to give
velocity commands as a ROS message. This model was
modified in order to match the design of the personal
mobility system. A Kinect sensor along with a [plugin](http://gazebosim.org/tutorials?tut=ros_depth_camera&cat=connect_ros) for
generating sensor readings was added by modifying the
xacro files of the Robosavvy package.
This set up behaves exactly as the personal mobility system
should when viewed in terms of the ROS control system. It
provides an abstraction which takes ROS velocity commands (in the form of vx and
vtheta) as inputs and provides odometry information and Kinect sensor data (from a platform with variable pitch)
as output.

![](https://raw.githubusercontent.com/kavisha725/fyp_simulation/master/pics/fyp_sim_robot.png) 

---

## Gazebo environment

A test world was created on Gazebo to simulate
the platform. This world
consists of four walls with dummy obstacles in the
middle in order to simulate an indoor
environment. The obstacles were placed in a
manner to ensure that the robots laser scan will always have a distinct reading in 
order to help localize the robot. 

![](https://raw.githubusercontent.com/kavisha725/fyp_simulation/master/pics/fyp_sim_env.png) 

---

## Start the simulation

  Make sure to compile and source workspace. 
  ```
  cd ~/fyp_simulation
  catkin_make
  source ./devel/setup.bash
  ```

  To launch [gmapping](http://wiki.ros.org/gmapping) with the above test environment use:  
  ```
  roslaunch simulation_auto all_gmapping.launch
  ```
  
   To launch [amcl](http://wiki.ros.org/amcl) and [move_base](http://wiki.ros.org/move_base) with pre-built map:  
  ```
  roslaunch simulation_auto all_amcl.launch
  ```

---

## Customization

* Add custom gazebo world:
	* fyp_simulation/src/rsv_balance_simulator/rsv_balance_gazebo/launch/simulation_pmv.launch
* Add custom robot:
	* fyp_simulation/src/rsv_balance_simulator/rsv_balance_gazebo/launch/include/spawn_robot.launch
* Parameters of the ROS navigation stack:
	* gmapping
		* fyp_simulation/src/simulation_auto/launch/gmapping.launch
	* amcl
		* fyp_simulation/src/pmv_2dnav/launch/amcl.launch
	* move_base
		* fyp_simulation/src/pmv_2dnav/config/movebase/
