#ifndef _Calc_cam_pose_H_
#define _Calc_cam_pose_H_

#include <cv_bridge/cv_bridge.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <string>
#include<opencv2/highgui/highgui.hpp>
#include<geometry_msgs/Polygon.h>
using namespace geometry_msgs;
using namespace std;


cv::Mat Matrix2Quaternion(cv::Mat matrix);
cv::Mat Quaternion2Matrix (float x, float y, float z, float w);
geometry_msgs::Vector3 normalize(geometry_msgs::Vector3 input_vector);
geometry_msgs::Vector3 cross_product(geometry_msgs::Vector3 u,geometry_msgs::Vector3 v);
double dot_product(geometry_msgs::Vector3 u,geometry_msgs::Vector3 v);


geometry_msgs::PoseStamped calc_cam_pose(
    geometry_msgs::PoseStamped ic,
    geometry_msgs::PoseStamped tp,
    geometry_msgs::Vector3 uv);
#endif     