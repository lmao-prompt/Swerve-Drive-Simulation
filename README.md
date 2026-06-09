# Swerve Drive Simulation

[![ROS 2](https://img.shields.io/badge/ROS2-Jazzy-blue)](https://docs.ros.org/en/jazzy/index.html)
[![Gazebo](https://img.shields.io/badge/Gazebo-Harmonic-orange)](https://gazebosim.org/home)

## How to

```bash
# 1. Make ros ws
mkdir -p ~/swerve_ws/src
cd ~/swerve_ws/src

# 2. copy this
git clone [https://github.com/lmao-prompt/Swerve-Drive-Simulation.git](https://github.com/lmao-prompt/Swerve-Drive-Simulation.git) swerve

# 3. build
cd ~/swerve_ws
colcon build --symlink-install
source install/setup.bash

# 4. run
gz sim swerve.urdf
ros2 launch swerve_controller swerve_launch.py
ros2 run ros_gz_bridge parameter_bridge \
  /model/three_wheel_swerve/joint/steer_joint_1/cmd_pos@std_msgs/msg/Float64@gz.msgs.Double \
  /model/three_wheel_swerve/joint/steer_joint_2/cmd_pos@std_msgs/msg/Float64@gz.msgs.Double \
  /model/three_wheel_swerve/joint/steer_joint_3/cmd_pos@std_msgs/msg/Float64@gz.msgs.Double \
  /model/three_wheel_swerve/joint/wheel_joint_1/cmd_vel@std_msgs/msg/Float64@gz.msgs.Double \
  /model/three_wheel_swerve/joint/wheel_joint_2/cmd_vel@std_msgs/msg/Float64@gz.msgs.Double \
  /model/three_wheel_swerve/joint/wheel_joint_3/cmd_vel@std_msgs/msg/Float64@gz.msgs.Double

#5. test
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.5, y: -0.5, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 0.0}}"
#or u can use teleop
ros2 run teleop_twist_keyboard teleop_twist_keyboard 
