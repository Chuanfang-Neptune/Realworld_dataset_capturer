
# Realworld_dataset_capturer
![image](http://github.com/Chuanfang-Neptune/Realworld_dataset_capturer/Intro.png)
    This project controls a UR5 robot arm with an Intel D435 sensor to move in a spherical path around a training object on a table.
    During the robot motion RGB and depth images of the training object are captured. 
    6D poses of the camera are automatically calculated from the joint states of the robot.
    The segmentation pictures and 3D bounding boxes of the training object are obtained from the 6D poses and model information.
## Requirements

1. Ubuntu 16.04
2. ROS Kinetic
3. Gazebo 7.x
4. Freeglut 3.x
5. Freeimage 3.x

## Package Info
### universal_robot
  
  The Move_it! configuration of UR5 robot is based on gitatiya's work Hacking-SotA-UR5 on https://github.com/gtatiya/Hacking-SotA-UR5
## User Guides
1. The directory structure has been modified, only UR5 related functions have been reserved, and the gripper function has been removed
2. URDF file has been modified to stick camera to ee_link
3. camera parameters has been added according to Intel RealSense D435
4. sensor can publish both rgb and depth image_raw topic
5. This package launches gazebo world, UR5 arm, table and LEGOcar on the table. 
to launch Rviz Gazebo Plan:
`roslaunch ur5_single_arm_manipulation gazebo_plan_rviz.launch`

### Rviz:
`roslaunch ur5_setup ur5_single_arm_rviz.launch`

### Gazebo:
```
roslaunch gazebo_ros empty_world.launch paused:=true use_sim_time:=false gui:=true throttled:=false recording:=false debug:=true
rosrun gazebo_ros spawn_model -file $(rospack find ur5_setup)/urdf/ur5_single_arm.urdf -urdf -x 0 -y 0 -z 0 -model ur5_single_arm
```

### Gazebo + MoveIt Rviz:
`roslaunch ur5_setup ur5_single_arm_gazebo.launch`


