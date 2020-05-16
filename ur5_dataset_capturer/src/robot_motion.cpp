//==================================================================
//module name：  robo_motion
 //author： Ning Chuanfang
 //date： 2020-04-22
 //description:control UR5 move camera to preset positions 
 //            write 6D pose infomation into json file
 //            capture rgb image file
 //            capture depth image file
 //            calculate segmentation image
 //          
 //modify：
 //
//==================================================================


#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_visual_tools/moveit_visual_tools.h>

#include "calc_cam_pose.h"
#include "glfunc.h"
<<<<<<< HEAD
#include "imageCapturer.h"
=======
#include "imageconverter.h"
>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339
#include "opengl_math.h"
#include <FreeImage.h>
#include <GL/freeglut.h>
#include <GL/glew.h>
#include <cv_bridge/cv_bridge.h>
#include <jsoncpp/json/json.h>

#include "robot_motion.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
<<<<<<< HEAD
#include <sys/types.h>
#include <sys/stat.h>
#include "json_utl.h"
#include "readconfig.h"
namespace rvt = rviz_visual_tools;
// shader
=======

namespace rvt = rviz_visual_tools;
// shader
const char *STLFileName = "/home/ncf/catkin/src/ur5_setup/meshes/model_stl.stl";
const char *pVSFileName = "/home/ncf/catkin/src/ur5_dataset_capturer/src/shader.vs";
const char *pFSFileName = "/home/ncf/catkin/src/ur5_dataset_capturer/src/shader.fs";
std::string rgbTopic = "/camera_kinect_head_mount/depth/image_raw";
std::string depthTopic = "/camera_kinect_head_mount_ir/depth/image_raw";

>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339

std::string filename, filename1s;
Matrix44f mmatrix, cmatrix, temp, mvp;

const float ar = WINDOWWIDTH / WINDOWHEIGHT;
const float zRange = zNear - zFar;
lego_geo lg;   //LEGO Car's geometric infomation

geometry_msgs::PoseStamped nc; // nc new pose of camera
geometry_msgs::PoseStamped ic; // ic initial pose of camera
geometry_msgs::PoseStamped tp; // target object pose
geometry_msgs::Vector3 uv;
moveit::core::RobotStatePtr current_state;

Matrix44f pmatrix = {
  2 * fx / WINDOWWIDTH, (2 * cx - WINDOWWIDTH) / WINDOWWIDTH, 0, 0,
  0, 2 * fy / WINDOWHEIGHT, (2 * cy - WINDOWHEIGHT) / WINDOWHEIGHT, 0, 
  0, 0, (-zNear - zFar) / zRange, 2.0f * zFar *zNear / zRange,
  0, 0, 1, 0}; // calibrated

int main(int argc, char **argv) {
<<<<<<< HEAD

  //printf("zNear=%f\n",zNear);
  readconfig();
  lg = readstl(STLFileName.c_str());
=======
  lg = readstl(STLFileName);
>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339
  glutInit(&argc, argv);
  // buffercount, color mode, buffer type
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  ros::init(argc, argv, "robo_motion");
  ros::AsyncSpinner spinner(4);
  spinner.start();

  static const std::string PLANNING_GROUP = "manipulator";
  const char *declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
  // moveit_commander moveit_commander_cmdline
  moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);

  const robot_state::JointModelGroup *joint_model_group = move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);
  //move_group options
  move_group.setStartState(*move_group.getCurrentState());
  move_group.setGoalPositionTolerance(0.02);
  move_group.setGoalOrientationTolerance(0.1);
  move_group.setPlanningTime(1.5);
  move_group.setNumPlanningAttempts(20);
  uv.x = 0;
  uv.y = 0;
  uv.z = 1;
  // step-by-step Visualization introspection of a script
  moveit_visual_tools::MoveItVisualTools visual_tools("base_link");
  visual_tools.deleteAllMarkers();
  visual_tools.loadRemoteControl(); // step via buttons in RViz

  // RViz provides many types of markers, in this demo we will use text,
  // cylinders, and spheres
  Eigen::Affine3d text_pose = Eigen::Affine3d::Identity();
  text_pose.translation().z() = 1.0;

  // Names of the reference frame for this robot and end-effector link for this
  // group.
  ROS_INFO_NAMED("ur5", "Reference frame: %s", move_group.getPlanningFrame().c_str());
  ROS_INFO_NAMED("ur5", "End effector link: %s", move_group.getEndEffectorLink().c_str());

  // Press in rviz to start the task
  visual_tools.prompt("Press 'next' to set up the scene");

  geometry_msgs::Pose lego_pose;
  lego_pose.orientation.x = c1_h*s2_h*c3_h+s1_h*c2_h*s3_h;
  lego_pose.orientation.y = s1_h*c2_h*c3_h-c1_h*s2_h*s3_h;
  lego_pose.orientation.z = -s1_h*s2_h*c3_h+c1_h*c2_h*s3_h;
  lego_pose.orientation.w = c1_h*c2_h*c3_h+s1_h*s2_h*s3_h;
  lego_pose.position.x = LEGO_POSITION_X;
  lego_pose.position.y = LEGO_POSITION_Y;
  lego_pose.position.z = LEGO_POSITION_Z;

  visual_tools.prompt("Press 'next' to go on to plan");

  geometry_msgs::Pose target_posen;//preset pose
  moveit::planning_interface::MoveGroupInterface::Plan my_plan;
  bool success;
  int count = -1;

  // controled by ee position
  char *buf = new char[30];
  double b = -1, b1 = -1, xy = 0, z = 0, x = 0, y = 0, xn = 0, yn = 0, c = 0, k = 0, r;
  int firstplan = 0, fp = 0, m = -2, l = -2;
  std::vector<double> jgp; //UR six joints initial value at new positon 
  std::vector<double> jgp1; //UR six joints value after swung

  tp.pose.position.x = lego_pose.position.x;
  tp.pose.position.y = lego_pose.position.y;
  tp.pose.position.z = lego_pose.position.z;

  // preset positions on 1/8 sphere
  double beta = PI / 18;
  for (int k = 0; k < 9; k++) {
    r = 0.7 + 0.02 * k;
    for (int i = 0; i < 9; i++) { // 0~80 degrees
      xy = r * cos(i * beta);     // PI/18 0.1745
      z = r * sin(i * beta);
      b = -b;
      for (int j = 0; j < 10; j++) {
        if (firstplan == 0) {
          c = j * beta;
          xn = xy * cos(c);
          yn = xy * sin(c);
          if (b == -1) {
            c = xn;
            xn = yn;
            yn = c;
          }
          x = 0.707 * (yn - xn);
          y = -0.707 * (xn + yn);
          target_posen.position.x = x + lego_pose.position.x; //target translation
          target_posen.position.y = y + lego_pose.position.y;
          target_posen.position.z = z + lego_pose.position.z;
          ic.pose.position = target_posen.position;
          nc = calc_cam_pose(ic, tp, uv);
          target_posen.orientation = nc.pose.orientation;

          move_group.setStartStateToCurrentState();
          move_group.setPoseTarget(target_posen);
          success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
          if (success) {
            firstplan = 1;
            m = -2;
            l = -2;
<<<<<<< HEAD
=======
            //here I think should add jgp = 
>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339
          }
        }  
        else if (firstplan == 1) {
          // Dynamic adjustment of swing amplitude to avoid car out of camera view
          // swing 1.5 angle at longest distance 0.5meter
          // small swing at close range
          double delta = PI / 180 * 1.5 * r / 0.9 ; 
          printf("delta=%4.2f r=%6.4f\n",delta/PI*180,r);
          if (m < 3 && l < 3) {           
            //joint swing should not bigger than 2Pi
            jgp1[3] = jgp[3] + m * delta; //forth joint 
            if (jgp1[3] > 2 * PI) {
              jgp1[3] -= 2 * PI;
            }
            if (jgp1[3] < -2 * PI) {
              jgp1[3] += 2 * PI;
            }
            if (l < 3) {                    
              jgp1[4] = jgp[4] + l * delta; //fifth joint
              if (jgp1[4] > 2 * PI) {
                jgp1[4] -= 2 * PI;
              }
              if (jgp1[4] < -2 * PI) {
                jgp1[4] += 2 * PI;
              }
              l++;
              if (l == 0 && m == 0)   //no swing amplitude is the initial position
                l++;
            }
          }

          move_group.setStartStateToCurrentState();
          move_group.setJointValueTarget(jgp1); //set target pose with joint's value
          success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS); //plan
        }
        if (success) {
          move_group.execute(my_plan);
          if (fp == 0) {
            current_state = move_group.getCurrentState(); 
            current_state->copyJointGroupPositions(joint_model_group, jgp); //initial joints' value
            current_state->copyJointGroupPositions(joint_model_group, jgp1);//copy initial vecter to target
            fp = 1;
          }

          // counter
          count += 1;
          //json file name
<<<<<<< HEAD
          int isCreate = mkdir(jsonFilePath.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
          filename = jsonFilePath + std::to_string(count) + ".json";
          //segmentation image file name
          filename1s = jsonFilePath + std::to_string(count) + "s.jpg";
=======
          filename = "/home/ncf/ROS/json/" + std::to_string(count) + ".json";
          //segmentation image file name
          filename1s = "/home/ncf/ROS/json/" + std::to_string(count) + "s.jpg";
>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339
          // json populator
          float temprx = move_group.getCurrentPose().pose.orientation.x;
          float tempry = move_group.getCurrentPose().pose.orientation.y;
          float temprz = move_group.getCurrentPose().pose.orientation.z;
          float temprw = move_group.getCurrentPose().pose.orientation.w;
          cv::Mat cr = Quaternion2Matrix(temprx, tempry, temprz, temprw);
          Vector3f ct = {cr.at<float>(0, 0), cr.at<float>(1, 0), cr.at<float>(2, 0)};
          float temppx = move_group.getCurrentPose().pose.position.x 
                         + caml * cr.at<float>(0, 0) / sqrt(cr.at<float>(0, 0) * cr.at<float>(0, 0) 
                         + cr.at<float>(1, 0) * cr.at<float>(1, 0) 
                         + cr.at<float>(2, 0) * cr.at<float>(2, 0));
          float temppy = move_group.getCurrentPose().pose.position.y 
                         + caml * cr.at<float>(1, 0) / sqrt(cr.at<float>(0, 0) * cr.at<float>(0, 0) 
                         + cr.at<float>(1, 0) * cr.at<float>(1, 0) 
                         + cr.at<float>(2, 0) * cr.at<float>(2, 0));
          float temppz = move_group.getCurrentPose().pose.position.z 
                         + caml * cr.at<float>(2, 0) / sqrt(cr.at<float>(0, 0) * cr.at<float>(0, 0) 
                         + cr.at<float>(1, 0) * cr.at<float>(1, 0) 
                         + cr.at<float>(2, 0) * cr.at<float>(2, 0));

          // matrix calc
          Matrix44f mtmatrix = {
            1, 0, 0, (float)LEGO_POSITION_X - temppx, 
            0, 1, 0, (float)LEGO_POSITION_Y - temppy, 
            0, 0, 1, (float)LEGO_POSITION_Z - temppz, 
            0, 0, 0, 1};

          Matrix44f mrmatrix = {
              c1*c3+s1*s2*s3, c3*s1*s2-c1*s3,c2*s1,0,
              c2*s3,c2*c3,-s2,0,
              c1*s2*s3-s1*c3,s1*s3+c1*c3*s2,c1*c2,0,
              0,0,0,1
          };

          Matrix44f msmatrix = {  //scale
            1.0, 0, 0, 0, 
            0, 1.0, 0, 0, 
            0, 0, 1.0, 0, 
            0, 0, 0, 1};
          Vector3f cu = {cr.at<float>(0, 2), cr.at<float>(1, 2), cr.at<float>(2, 2)};
          CameraMatrix44(cmatrix, ct, cu);
          MatrixMultiply44(temp, mrmatrix, msmatrix);
          MatrixMultiply44(mmatrix, mtmatrix, temp);
          MatrixMultiply44(temp, cmatrix, mmatrix);
          //get transform matrix mvp
          MatrixMultiply44(mvp, pmatrix, temp);

          //scence infomation: camera & object 
          Json::Value event;
<<<<<<< HEAD
          event["camera_data"]["location_worldframe"] = jsonset_location(temppx,temppy,temppz);
          event["camera_data"]["quaternion_xyzw_worldframe"] = jsonset_quaternion(temprx,tempry,temprz,temprw);
          event["object_data"]["location_worldframe"] = jsonset_location(LEGO_POSITION_X,LEGO_POSITION_Y,LEGO_POSITION_Z);
          event["object_data"]["quaternion_xyzw_worldframe"] = jsonset_quaternion(lego_pose);
          event["object_data"]["projected_cuboid"] = jsonset_box(lg,mvp,WINDOWWIDTH,WINDOWHEIGHT);
=======
          Json::Value camera_location_worldframe(Json::arrayValue);
          camera_location_worldframe.append(Json::Value(temppx));
          camera_location_worldframe.append(Json::Value(temppy));
          camera_location_worldframe.append(Json::Value(temppz));

          Json::Value camera_quaternion_xyzw_worldframe(Json::arrayValue);
          camera_quaternion_xyzw_worldframe.append(Json::Value(temprx));
          camera_quaternion_xyzw_worldframe.append(Json::Value(tempry));
          camera_quaternion_xyzw_worldframe.append(Json::Value(temprz));
          camera_quaternion_xyzw_worldframe.append(Json::Value(temprw));

          Json::Value object_location(Json::arrayValue);
          object_location.append(Json::Value(LEGO_POSITION_X-temppx));
          object_location.append(Json::Value(LEGO_POSITION_Y-temppy));
          object_location.append(Json::Value(LEGO_POSITION_Z-temppz));
          //inverse of cam quaternion
          float temprq = temprx*temprx+tempry*tempry+temprz*temprz+temprw*temprw;
          float itemprx = -temprx/temprq;
          float itempry = -tempry/temprq;
          float itemprz = -temprz/temprq;
          float itemprw = temprw/temprq;
          //transformation rotation = r_object * inv_r_camera
          float rrx = itemprw*lego_pose.orientation.x + itemprx*lego_pose.orientation.w - itempry*lego_pose.orientation.z + itemprz*lego_pose.orientation.y;
          float rry = itemprw*lego_pose.orientation.y + itemprx*lego_pose.orientation.z + itempry*lego_pose.orientation.w - itemprz*lego_pose.orientation.x;
          float rrz = itemprw*lego_pose.orientation.z - itemprx*lego_pose.orientation.y + itempry*lego_pose.orientation.x + itemprz*lego_pose.orientation.w;
          float rrw = itemprw*lego_pose.orientation.w - itemprx*lego_pose.orientation.x - itempry*lego_pose.orientation.y - itemprz*lego_pose.orientation.z;
          //object's rotation to camera
          Json::Value object_quaternion_xyzw(Json::arrayValue);
          object_quaternion_xyzw.append(Json::Value(rrx));
          object_quaternion_xyzw.append(Json::Value(rry));
          object_quaternion_xyzw.append(Json::Value(rrz));
          object_quaternion_xyzw.append(Json::Value(rrw));

          Json::Value object_cuboidp(Json::arrayValue);
          Json::Value object_cuboidp1(Json::arrayValue);
          Json::Value object_cuboidp2(Json::arrayValue);
          Json::Value object_cuboidp3(Json::arrayValue);
          Json::Value object_cuboidp4(Json::arrayValue);
          Json::Value object_cuboidp5(Json::arrayValue);
          Json::Value object_cuboidp6(Json::arrayValue);
          Json::Value object_cuboidp7(Json::arrayValue);
          Json::Value object_cuboidp8(Json::arrayValue);

          object_cuboidp1.append((-(lg.maxx * mvp[0] + lg.miny * mvp[1] + lg.minz * mvp[2] + mvp[3]) / (lg.maxx * mvp[12] + lg.miny * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWWIDTH);
          object_cuboidp2.append((-(lg.maxx * mvp[0] + lg.maxy * mvp[1] + lg.minz * mvp[2] + mvp[3]) / (lg.maxx * mvp[12] + lg.maxy * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWWIDTH);
          object_cuboidp3.append((-(lg.minx * mvp[0] + lg.maxy * mvp[1] + lg.minz * mvp[2] + mvp[3]) / (lg.minx * mvp[12] + lg.maxy * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWWIDTH);
          object_cuboidp4.append((-(lg.minx * mvp[0] + lg.miny * mvp[1] + lg.minz * mvp[2] + mvp[3]) / (lg.minx * mvp[12] + lg.miny * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWWIDTH);
          object_cuboidp5.append((-(lg.maxx * mvp[0] + lg.miny * mvp[1] + lg.maxz * mvp[2] + mvp[3]) / (lg.maxx * mvp[12] + lg.miny * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWWIDTH);
          object_cuboidp6.append((-(lg.maxx * mvp[0] + lg.maxy * mvp[1] + lg.maxz * mvp[2] + mvp[3]) / (lg.maxx * mvp[12] + lg.maxy * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWWIDTH);
          object_cuboidp7.append((-(lg.minx * mvp[0] + lg.maxy * mvp[1] + lg.maxz * mvp[2] + mvp[3]) / (lg.minx * mvp[12] + lg.maxy * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWWIDTH);
          object_cuboidp8.append((-(lg.minx * mvp[0] + lg.miny * mvp[1] + lg.maxz * mvp[2] + mvp[3]) / (lg.minx * mvp[12] + lg.miny * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWWIDTH);

          object_cuboidp1.append(((lg.maxx * mvp[4] + lg.miny * mvp[5] + lg.minz * mvp[6] + mvp[7]) / (lg.maxx * mvp[12] + lg.miny * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWHEIGHT);
          object_cuboidp2.append(((lg.maxx * mvp[4] + lg.maxy * mvp[5] + lg.minz * mvp[6] + mvp[7]) / (lg.maxx * mvp[12] + lg.maxy * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWHEIGHT);
          object_cuboidp3.append(((lg.minx * mvp[4] + lg.maxy * mvp[5] + lg.minz * mvp[6] + mvp[7]) / (lg.minx * mvp[12] + lg.maxy * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWHEIGHT);
          object_cuboidp4.append(((lg.minx * mvp[4] + lg.miny * mvp[5] + lg.minz * mvp[6] + mvp[7]) / (lg.minx * mvp[12] + lg.miny * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWHEIGHT);
          object_cuboidp5.append(((lg.maxx * mvp[4] + lg.miny * mvp[5] + lg.maxz * mvp[6] + mvp[7]) / (lg.maxx * mvp[12] + lg.miny * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWHEIGHT);
          object_cuboidp6.append(((lg.maxx * mvp[4] + lg.maxy * mvp[5] + lg.maxz * mvp[6] + mvp[7]) / (lg.maxx * mvp[12] + lg.maxy * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWHEIGHT);
          object_cuboidp7.append(((lg.minx * mvp[4] + lg.maxy * mvp[5] + lg.maxz * mvp[6] + mvp[7]) / (lg.minx * mvp[12] + lg.maxy * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWHEIGHT);
          object_cuboidp8.append(((lg.minx * mvp[4] + lg.miny * mvp[5] + lg.maxz * mvp[6] + mvp[7]) / (lg.minx * mvp[12] + lg.miny * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * WINDOWHEIGHT);

          object_cuboidp.append(object_cuboidp1);
          object_cuboidp.append(object_cuboidp2);
          object_cuboidp.append(object_cuboidp3);
          object_cuboidp.append(object_cuboidp4);
          object_cuboidp.append(object_cuboidp5);
          object_cuboidp.append(object_cuboidp6);
          object_cuboidp.append(object_cuboidp7);
          object_cuboidp.append(object_cuboidp8);

          event["camera_data"]["location_worldframe"] = camera_location_worldframe;
          event["camera_data"]["quaternion_xyzw_worldframe"] = camera_quaternion_xyzw_worldframe;
          event["object_data"]["location_worldframe"] = object_location;
          event["object_data"]["quaternion_xyzw_worldframe"] = object_quaternion_xyzw;
          //event["object_data"]["cuboid"] = object_cuboid;
          event["object_data"]["projected_cuboid"] = object_cuboidp;
>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339

          //write scence info into json file
          remove(filename.c_str());  //remove old file
          Json::StyledWriter sw;
          std::ofstream os;
          os.open(filename, std::ios::out | std::ios::app);
          if (!os.is_open())
            std::cout << "error：can not find or create the file which named \" " << count << ".json\"." << std::endl;
          os << sw.write(event);
          os.close();

          //capture rgb file  
<<<<<<< HEAD
          std::string rgbfilename = jsonFilePath + std::to_string(count) + "r.png";
          remove(rgbfilename.c_str());             //remove old image file
          imageCapturerRGB imc(rgbfilename.c_str()); //imageCapturerRGB instance imc 
=======
          std::string rgbfilename = "/home/ncf/ROS/json/" + std::to_string(count) + "r.png";
          remove(rgbfilename.c_str());             //remove old image file
          ImageConverter imc(rgbfilename.c_str()); //ImageConverter instance imc 
>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339
          imc.set_filename(rgbfilename.c_str(), rgbTopic);  //call set_filename 
          while (!file_exists(rgbfilename.c_str()))  // wait until the file were created
            ;

          //capture depth file  
<<<<<<< HEAD
          std::string depthfilename = jsonFilePath + std::to_string(count) + "d.png"; 
          remove(depthfilename.c_str()); 
          imageCapturerDepth imd(depthfilename.c_str());
=======
          std::string depthfilename = "/home/ncf/ROS/json/" + std::to_string(count) + "d.png"; 
          remove(depthfilename.c_str()); 
          ImageConverted imd(depthfilename.c_str());
>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339
          imd.set_filename(depthfilename.c_str(), depthTopic);
          while (!file_exists(depthfilename.c_str()))
            ;

          // start segmentation output
          glutInitWindowPosition(0, 0);
          glutInitWindowSize(WINDOWWIDTH, WINDOWHEIGHT);
          glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

          glutCreateWindow("Camera Capture");
          // Call to the drawing function
          glutDisplayFunc(Render);
          GLenum res = glewInit();
          if (res != GLEW_OK) {
            fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
            return 1;
          }
          glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
          CreateVertexBuffer();
          CreateIndexBuffer();
          CompilerShaders();

          long i = 0;
          while (glutGetWindow() && i < 2) {
            printf("Saving segmentation pictures: %ld percent finished\n", (i + 1) * 50);
            i++;
            glutMainLoopEvent();
            boost::this_thread::sleep(boost::posix_time::milliseconds(100));
          }
          if (glutGetWindow())
            glutDestroyWindow(glutGetWindow());
          glutMainLoopEvent();
          glutMainLoopEvent();
        }
        if (l == 3) {
          m++;
          if (m == 3) {
            //swing finished 
            firstplan = 0;
            fp = 0;
          }
          l = -2;
        }
      }
    }
  }
  ros::shutdown();
  return 0;
}