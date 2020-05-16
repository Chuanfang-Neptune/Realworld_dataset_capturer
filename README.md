<<<<<<< HEAD
# Realworld_dataset_capturer
  This project provides an automatic method of creating a real-world dataset for a training object with the help of a UR5 robot.
  The dataset consits of RGB images, depth images, segmentation masks and 6D poses for the camera and training object.
![image](https://github.com/Chuanfang-Neptune/Realworld_dataset_capturer/blob/master/Intro1.jpg)
## Project Discription
  An Intel D435 sensor is mounted on a robotic arm to surround a training object on a table.
  During the motion the RGB and depth images of the training object are captured.
  6D poses of the camera and the training object are calculated from the robot's joint states in real time.
  Mask images and 3D bounding boxes of the object are calculated from the object model and robot joint states.
  6D poses of the camera are automatically calculated from the joint states of the robot.
  The segmentation pictures and 3D bounding boxes of the training object are obtained from the 6D poses and model information.
## Requirements
  1. Ubuntu 16.04
  2. ROS Kinetic
  3. Gazebo 7.x
  4. Freeglut 3.x
  5. Freeimage 3.x
## User Guides
### I.Build the Code
1. Make sure all dependencies are ready and build the src code on your OS.
2. Make a directory where you want to put the configuration files, import files and export dataset for this project
3. Migrate the "conf" and "data" directory to your created directory.
### II.Setup your Camera
1. Mount your RGB and depth sensor on the robotic arm. Measure its offset to the ee_link and edit it in the file:
  package:ur5_setup/urdf/ur5_single_arm.urdf.xacro:line 18 and {YOUR_WORKING_FOLDER}/conf/setting.conf:caml
2. Calibrate your camera to get its intrinsic matrix. Edit id in the file:
  package:ur5_setup/urdf/sensors/kinect/kinect.urdf.xacro and {YOUR_WORKING_FOLDER}/conf/setting.conf
  Note: Your camera's FOV can be calculated from its focal length with: FOV(radian) = 2*arctan(image_height/2fx)
3. Replace the RGB and depth topic in {YOUR_WORKING_FOLDER}/conf/setting.conf with your camera's topics (default topics are camera topics in Gazebo simulation)
### III.Prepare your Training Object
1. Laserscan or build the model of the training object you want to use for the dataset. Export its model file to DAE and binary STL format.
2. Rename the STL file to model_stl.stl and put it in {YOUR_WORKING_FOLDER}/data/
3. Rename the DAE file to model_dae.dae and put it in  package:ur5_setup/meshes/
4. Edit the scale and collision of your training model in package:ur5_setup/urdf/ur5_single_arm.urdf.xacro:line 60
### IV.Setup Experiment Environment
1. Fix your robot base on a pillar besides a table. 
  Measure the robot base's offset to the ground. Edit it in package:ur5_setup/urdf/ur5_single_arm.urdf.xacro:line 22
  Measure the table's size and positions to the robot base. Edit it in package:ur5_setup/urdf/ur5_single_arm.urdf.xacro:line 27
2. Put the training object on a table, measure its inital positions and rotations to the robot base. Edit it in package:ur5_setup/urdf/ur5_single_arm.urdf.xacro:line 60 and {YOUR_WORKING_FOLDER}/conf/setting.conf
### V.Get your Robot running
1. cd {YOUR_WORKING_FOLDER}
2. Launch RVIZ and Gazebo Simulation Environment:
  `roslaunch ur5_setup gazebo_plan_rviz.launch`
   if success you should get:
![image](https://github.com/Chuanfang-Neptune/Realworld_dataset_capturer/blob/master/Intro2.jpg)
   or for real robots run:(not tested)
   Launch RVIZ and camera node:
  `roslaunch ur5_setup ur5_single_arm_rviz.launch`
  `rosrun ur5_dataset_capturer ur5_dataset_capturer_node`
3. Run the main robot program
  `rosrun ur5_dataset_capturer robot_motion`
4. Captured dataset will be stored in the {YOUR_WORKING_FOLDER/data/json} folder
5. Adjust your object pose and repeat the process for more dataset variations
## Dataset Structures
x is the frame number
1. Json File (x.json): stores 6D pose of the camera and the object. (Origin is robot base) stores 3D bounding box vertex positions in original (in meters) and in 2D projection (in pixels)
2. Mask image (xs.jpg): stores the segmentation mask of the training object
3. RGB image (xr.png): stores the RGB image captured from virtual or real sensors of the training object
4. Depth image (xd.png): stores the depth image captured from virtual or real sensors of the training object
=======
<<<<<<< HEAD

# Realworld_dataset_capturer
![image](https://github.com/Chuanfang-Neptune/Realworld_dataset_capturer/blob/master/Intro.jpg)
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


=======
# Realworld_dataset_capturer
>>>>>>> 67b721fee2fa9ea2f98efa9636a7f7e2f9ac4255
>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339
