<<<<<<< HEAD
# ur5_dataset_capturer
This package includes the main programs and functions used in this project
See each cpp and header file for detailed codes
## calc_cam_pose
Mathematical function to aim the camera at the object
## glfunc
Segmentation image calculation and display functions
## imageCapturer
RGB image topic handler
## usb_cam
ROS driver for USB cameras. Based on http://wiki.ros.org/usb_cam
## readconfig
Read camera topics, training object transformations, shader file locations from setting.conf
## robot_motion
Main program of driving robot to move and call other functions of capturing images and calculating pose data.
=======

# usb_cam
  based on usb_cam [![Build Status](https://api.travis-ci.org/bosch-ros-pkg/usb_cam.png)](https://travis-ci.org/bosch-ros-pkg/usb_cam)


## 2020.05.01 Ning chuanfang
1. control ur5 move to planned poses
2. write json file : camera pose , object pose and projected_cuboid
3. subscribe camera image_raw topic, write rgb and depth image using opencv
4. write segmentation image using glut

>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339


