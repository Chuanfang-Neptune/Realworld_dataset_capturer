/*
calculate camera's pose
 input target object's pose and camera's position
 output camera's pose
 calculate camera's orientation,  should be face object straightly
*/
#include "calc_cam_pose.h"
#include <sys/stat.h>
using namespace cv;

geometry_msgs::PoseStamped initial_cam_pose, target_pose, new_cam_pose;
geometry_msgs::Vector3 v, up_vector, new_cam_x, new_cam_y, new_cam_z;
float *R = (float *)calloc(9, sizeof(float));


cv::Mat Quaternion2Matrix(float x, float y, float z, float w) {
  float xx = x * x;
  float yy = y * y;
  float zz = z * z;
  float xy = x * y;
  float wz = w * z;
  float wy = w * y;
  float xz = x * z;
  float yz = y * z;
  float wx = w * x;

  float ret[4][4];
  ret[0][0] = 1.0f - 2 * (yy + zz);
  ret[0][1] = 2 * (xy - wz);
  ret[0][2] = 2 * (wy + xz);
  ret[0][3] = 0.0f;

  ret[1][0] = 2 * (xy + wz);
  ret[1][1] = 1.0f - 2 * (xx + zz);
  ret[1][2] = 2 * (yz - wx);
  ret[1][3] = 0.0f;

  ret[2][0] = 2 * (xz - wy);
  ret[2][1] = 2 * (yz + wx);
  ret[2][2] = 1.0f - 2 * (xx + yy);
  ret[2][3] = 0.0f;

  ret[3][0] = 0.0f;
  ret[3][1] = 0.0f;
  ret[3][2] = 0.0f;
  ret[3][3] = 1.0f;

  return cv::Mat(4, 4, CV_32FC1, ret).clone();
}

cv::Mat Matrix2Quaternion(cv::Mat matrix) {
  float tr, qx, qy, qz, qw;
  float a[4][4] = {0};
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      a[i][j] = matrix.at<float>(i, j);
  float trace = a[0][0] + a[1][1] + a[2][2];
  if (trace > 0) {
    float s = 0.5f / sqrtf(trace + 1.0f);
    qw = 0.25f / s;
    qx = (a[2][1] - a[1][2]) * s;
    qy = (a[0][2] - a[2][0]) * s;
    qz = (a[1][0] - a[0][1]) * s;
  } else {
    if (a[0][0] > a[1][1] && a[0][0] > a[2][2]) {
      float s = 2.0f * sqrtf(1.0f + a[0][0] - a[1][1] - a[2][2]);
      qw = (a[2][1] - a[1][2]) / s;
      qx = 0.25f * s;
      qy = (a[0][1] + a[1][0]) / s;
      qz = (a[0][2] + a[2][0]) / s;
    } else if (a[1][1] > a[2][2]) {
      float s = 2.0f * sqrtf(1.0f + a[1][1] - a[0][0] - a[2][2]);
      qw = (a[0][2] - a[2][0]) / s;
      qx = (a[0][1] + a[1][0]) / s;
      qy = 0.25f * s;
      qz = (a[1][2] + a[2][1]) / s;
    } else {
      float s = 2.0f * sqrtf(1.0f + a[2][2] - a[0][0] - a[1][1]);
      qw = (a[1][0] - a[0][1]) / s;
      qx = (a[0][2] + a[2][0]) / s;
      qy = (a[1][2] + a[2][1]) / s;
      qz = 0.25f * s;
    }
  }
  float q[] = {qw, qx, qy, qz};
  // cout<< "\n quaternion:"<<cv::Mat(4,1,CV_32FC1,q).t()<<endl;
  return cv::Mat(4, 1, CV_32FC1, q).clone();
}

geometry_msgs::Vector3 normalize(geometry_msgs::Vector3 input_vector) {
  float mag = pow(pow(input_vector.x, 2) + pow(input_vector.y, 2) + pow(input_vector.z, 2), 0.5);
  geometry_msgs::Vector3 unit_vector;
  unit_vector.x = input_vector.x / mag;
  unit_vector.y = input_vector.y / mag;
  unit_vector.z = input_vector.z / mag;
  return (unit_vector);
}

geometry_msgs::Vector3 cross_product(geometry_msgs::Vector3 u, geometry_msgs::Vector3 v) {
  geometry_msgs::Vector3 cross;
  cross.x = u.y * v.z - v.y * u.z;
  cross.y = v.x * u.z - u.x * v.z;
  cross.z = u.x * v.y - u.y * v.x;
  return (cross);
}

double dot_product(geometry_msgs::Vector3 u, geometry_msgs::Vector3 v) {
  double dot = u.x * v.x + u.y * v.y + u.z * v.z;
  return dot;
}

void calc_new_cam_unit_vectors() {
  new_cam_x = normalize(v);
  // Check for edge cases
  geometry_msgs::Vector3 cross = cross_product(new_cam_x, up_vector);
  if (cross.x == 0 and cross.y == 0 and cross.z == 0) {
    new_cam_z.x = -new_cam_x.z;
    new_cam_z.y = new_cam_x.x;
    new_cam_z.z = -new_cam_x.y;
    new_cam_y = cross_product(new_cam_x, new_cam_z);
  } else {
    new_cam_y = cross_product(new_cam_x, up_vector);
    new_cam_y = normalize(new_cam_y);
    new_cam_z = cross_product(new_cam_x, new_cam_y);
    if (dot_product(new_cam_z, up_vector) < 0) {
      new_cam_z.x = -new_cam_z.x;
      new_cam_z.y = -new_cam_z.y;
      new_cam_z.z = -new_cam_z.z;
      new_cam_y = cross_product(new_cam_z, new_cam_x);
    }
  }
}

geometry_msgs::PoseStamped calc_cam_pose(geometry_msgs::PoseStamped ic, geometry_msgs::PoseStamped tp, geometry_msgs::Vector3 uv) {
  target_pose = tp;
  initial_cam_pose = ic;
  up_vector = uv;
  v.x = target_pose.pose.position.x - initial_cam_pose.pose.position.x;
  v.y = target_pose.pose.position.y - initial_cam_pose.pose.position.y;
  v.z = target_pose.pose.position.z - initial_cam_pose.pose.position.z;
  calc_new_cam_unit_vectors();
  R[0] = new_cam_x.x;
  R[3] = new_cam_x.y;
  R[6] = new_cam_x.z;
  R[1] = new_cam_y.x;
  R[4] = new_cam_y.y;
  R[7] = new_cam_y.z;
  R[2] = new_cam_z.x;
  R[5] = new_cam_z.y;
  R[8] = new_cam_z.z;
  new_cam_pose.pose = initial_cam_pose.pose;
  float r[] = {R[0], R[1], R[2], 0, R[3], R[4], R[5], 0, R[6], R[7], R[8], 0, 0, 0, 0, 1};
  cv::Mat T(4, 4, CV_32FC1, r);
  cv::Mat q = Matrix2Quaternion(T);
  new_cam_pose.pose.orientation.w = q.at<float>(0, 0);
  new_cam_pose.pose.orientation.x = q.at<float>(0, 1);
  new_cam_pose.pose.orientation.y = q.at<float>(0, 2);
  new_cam_pose.pose.orientation.z = q.at<float>(0, 3);
  return (new_cam_pose);
}
