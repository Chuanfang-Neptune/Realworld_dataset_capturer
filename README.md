# Realworld_dataset_capturer
  This project provides an automatic method of creating a real-world dataset for a training object with the help of a UR5 robot.
  The dataset consits of RGB images, depth images, segmentation masks and 6D poses for the camera and training object.
![image](https://github.com/Chuanfang-Neptune/Realworld_dataset_capturer/blob/master/Intro1.jpg)
## Project Discription
  An Intel D435 sensor is mounted on a UR5 robotic arm on a mobile platform.
  The robotic arm holds the sensor to wander around an object of interest on a table to capture RGB and depth images to prepare a dataaset for training deep models.
  The ground truth of object's 6D poses, instance/segmentation mask and 3D bounding boxes are calculated from the camera matrix corresponding to robot's joint states in real time.
## Tested On
|![ROS Kinetic](https://img.shields.io/badge/ROS-Kinetic-red.svg) ![Ubuntu 16.04](https://img.shields.io/badge/Ubuntu-16.04-blue.svg) |![Freeglut](https://img.shields.io/badge/Freeglut-3.x-green.svg) ![Freeimage](https://img.shields.io/badge/Freeimage-3.x-green.svg)|
## User Guides
### I.Build the Code
1. Make sure all dependencies are ready and build the src code on your OS.
2. Make a directory where you want to put the configuration files, import files and export dataset for this project
3. Migrate the "conf" and "data" directory to your created directory.
### II.Setup your Camera
1. Mount RGB and depth sensor on the robotic arm. Measure its offset to the ee_link and edit it in the file:
  package:ur5_setup/urdf/ur5_single_arm.urdf.xacro:line 18 and {YOUR_WORKING_FOLDER}/conf/setting.conf:caml
2. Calibrate your camera to get its intrinsic matrix. Edit id in the file:
  package:ur5_setup/urdf/sensors/kinect/kinect.urdf.xacro and {YOUR_WORKING_FOLDER}/conf/setting.conf
  Note: Camera's FOV can be calculated from its focal length with: FOV(radian) = 2*arctan(image_height/2fx)
3. Replace the RGB and depth topic in {YOUR_WORKING_FOLDER}/conf/setting.conf with your camera's topics (default topics are camera topics in Gazebo simulation)
### III.Prepare your Training Object
1. Laserscan or build the model of the training object you want to use for the dataset. Export its model file to DAE and binary STL format.
2. Rename the STL file to model_stl.stl and put it in {YOUR_WORKING_FOLDER}/data/
3. Rename the DAE file to model_dae.dae and put it in  package:ur5_setup/meshes/
4. Edit the scale and collision of your training model in package:ur5_setup/urdf/ur5_single_arm.urdf.xacro:line 60
### IV.Setup Experiment Environment
1. Fix the robot base on the mobile platform. 
  Measure its z offset to the ground. Edit it in package:ur5_setup/urdf/ur5_single_arm.urdf.xacro:line 22
  Measure its initial offset to the mobile platform base. Edit it in package:ur5_setup/urdf/ur5_single_arm.urdf.xacro:line 27
2. Put the training object on a table, measure its inital offset to the robot base. Edit it in package:ur5_setup/urdf/ur5_single_arm.urdf.xacro:line 60 and {YOUR_WORKING_FOLDER}/conf/setting.conf
### V.Get the Robot running
1. cd {YOUR_WORKING_FOLDER}
2. (If simulation) Launch RVIZ and Gazebo Simulation Environment:
  `roslaunch ur5_setup gazebo_plan_rviz.launch`
   if success you should get:
![image](https://github.com/Chuanfang-Neptune/Realworld_dataset_capturer/blob/master/Intro2.jpg)
   (If real robots)
   Launch RVIZ and camera node:
  `roslaunch ur5_setup ur5_single_arm_rviz.launch`
  `rosrun ur5_dataset_capturer ur5_dataset_capturer_node`
  `bring up mobile platform wandering node (not implemented in this project)
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
