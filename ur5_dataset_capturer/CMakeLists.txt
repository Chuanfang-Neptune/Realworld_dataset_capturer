cmake_minimum_required(VERSION 2.8.3)

project(ur5_dataset_capturer)

add_compile_options(-std=c++11)
## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS image_transport roscpp std_msgs std_srvs sensor_msgs camera_info_manager cv_bridge interactive_markers moveit_core moveit_visual_tools moveit_ros_planning moveit_ros_planning_interface pluginlib geometric_shapes pcl_ros pcl_conversions rosbag tf2_ros tf2_eigen tf2_geometry_msgs)

## pkg-config libraries
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
pkg_check_modules(avcodec libavcodec REQUIRED)
pkg_check_modules(swscale libswscale REQUIRED)

###################################################
## Declare things to be passed to other projects ##
###################################################

## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
)

###########
## Build ##
###########
include_directories(
  include ${catkin_INCLUDE_DIRS}include
  ${catkin_INCLUDE_DIRS}
  ${avcodec_INCLUDE_DIRS}
  ${swscale_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIRS}  
  ${GLFW3_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS} 
  ${FREEIMAGE3_INCLUDE_DIRS}
)

## Build the USB camera library
add_library(${PROJECT_NAME} src/usb_cam.cpp)
target_link_libraries(${PROJECT_NAME}
  ${avcodec_LIBRARIES}
  ${swscale_LIBRARIES}
  ${catkin_LIBRARIES}
)

## Declare a cpp executable
add_executable(${PROJECT_NAME}_node nodes/usb_cam_node.cpp)
target_link_libraries(${PROJECT_NAME}_node
  ${PROJECT_NAME}
  ${avcodec_LIBRARIES}
  ${swscale_LIBRARIES}
  ${catkin_LIBRARIES}
)

#############
## Install ##
#############

## Mark executables and/or libraries for installation
install(TARGETS ${PROJECT_NAME}_node ${PROJECT_NAME}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

## Copy launch files
install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
  FILES_MATCHING PATTERN "*.launch"
)

install(DIRECTORY include/${PROJECT_NAME}/
   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
   FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

add_executable(robot_motion
<<<<<<< HEAD
  src/readconfig.cpp
  src/glfunc.cpp
  src/imageCapturer.cpp
=======
  src/glfunc.cpp
  src/imageconverter.cpp
>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339
  src/calc_cam_pose.cpp
  src/robot_motion.cpp
)
add_dependencies(robot_motion ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(robot_motion
  ${OPENGL_LIBRARIES} 
  ${GLEW_LIBRARIES}
  ${GLUT_LIBRARY}
  /usr/lib/x86_64-linux-gnu/libfreeimage-3.17.0.so
  ${catkin_LIBRARIES}
)

<<<<<<< HEAD
=======
add_executable(gltest
  src/gltest.cpp
)
add_dependencies(gltest ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(gltest
  ${OPENGL_LIBRARIES} 
  ${GLEW_LIBRARIES}
  ${GLUT_LIBRARY}
  /usr/lib/x86_64-linux-gnu/libfreeimage-3.17.0.so
  ${catkin_LIBRARIES}
)
>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339


