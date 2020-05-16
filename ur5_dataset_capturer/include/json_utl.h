#ifndef _Json_utl_H_
#define _Json_utl_H_

#include "glfunc.h"
#include "opengl_math.h"
#include <geometry_msgs/Polygon.h>
#include <jsoncpp/json/json.h>
//==================================================================
// function name：  jsonset_location
// author： Ning Chuanfang
// date： 2020-05-12
// function： set json value
// input parameters：
//          coordinate float x,float y,float z
// return：  type（Json::Value)
//          json node include object's coordinate
// modify：
//
//==================================================================
Json::Value jsonset_location(float x, float y, float z) {
  Json::Value object_location(Json::arrayValue);
  object_location.append(Json::Value(x));
  object_location.append(Json::Value(y));
  object_location.append(Json::Value(z));
  return (object_location);
}

//==================================================================
// function name：  jsonset_quaternion
// author： Ning Chuanfang
// date： 2020-05-12
// function： set json value
// input parameters：
//          pose include quaternion
// return：  type（Json::Value)
//          json node include object's quaternion
// modify：
//
//==================================================================
Json::Value jsonset_quaternion(geometry_msgs::Pose lego_pose) {
  Json::Value object_quaternion_xyzw(Json::arrayValue);
  object_quaternion_xyzw.append(Json::Value(lego_pose.orientation.x));
  object_quaternion_xyzw.append(Json::Value(lego_pose.orientation.y));
  object_quaternion_xyzw.append(Json::Value(lego_pose.orientation.z));
  object_quaternion_xyzw.append(Json::Value(lego_pose.orientation.w));
  return (object_quaternion_xyzw);
}
//==================================================================
// function name：  jsonset_quaternion
// author： Ning Chuanfang
// date： 2020-05-12
// function： set json value
// input parameters：
//          quaternion x y z w
// return：  type（Json::Value)
//          json node include object's quaternion
// modify：
//

//==================================================================
Json::Value jsonset_quaternion(float x, float y, float z, float w) {
  Json::Value object_quaternion_xyzw(Json::arrayValue);
  object_quaternion_xyzw.append(Json::Value(x));
  object_quaternion_xyzw.append(Json::Value(y));
  object_quaternion_xyzw.append(Json::Value(z));
  object_quaternion_xyzw.append(Json::Value(w));
  return (object_quaternion_xyzw);
}
//==================================================================
// function name：  set_cuboidp
// author： Ning Chuanfang
// date： 2020-05-12
// function： set object's geometry box
// input parameters：
//          lego_geo lg target object's geometry max and min xyz
//          Matrix44f mvp
//          image width and height
// return：  type（Json::Value)
//          json node include object's geometry
// modify：
//
//==================================================================
Json::Value jsonset_box(lego_geo lg, Matrix44f mvp, int windoww, int windowh) {
  Json::Value object_cuboidp(Json::arrayValue);
  Json::Value object_cuboidp1(Json::arrayValue);
  Json::Value object_cuboidp2(Json::arrayValue);
  Json::Value object_cuboidp3(Json::arrayValue);
  Json::Value object_cuboidp4(Json::arrayValue);
  Json::Value object_cuboidp5(Json::arrayValue);
  Json::Value object_cuboidp6(Json::arrayValue);
  Json::Value object_cuboidp7(Json::arrayValue);
  Json::Value object_cuboidp8(Json::arrayValue);

  object_cuboidp1.append((-(lg.maxx * mvp[0] + lg.miny * mvp[1] + lg.minz * mvp[2] + mvp[3]) / (lg.maxx * mvp[12] + lg.miny * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * windoww);
  object_cuboidp2.append((-(lg.maxx * mvp[0] + lg.maxy * mvp[1] + lg.minz * mvp[2] + mvp[3]) / (lg.maxx * mvp[12] + lg.maxy * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * windoww);
  object_cuboidp3.append((-(lg.minx * mvp[0] + lg.maxy * mvp[1] + lg.minz * mvp[2] + mvp[3]) / (lg.minx * mvp[12] + lg.maxy * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * windoww);
  object_cuboidp4.append((-(lg.minx * mvp[0] + lg.miny * mvp[1] + lg.minz * mvp[2] + mvp[3]) / (lg.minx * mvp[12] + lg.miny * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * windoww);
  object_cuboidp5.append((-(lg.maxx * mvp[0] + lg.miny * mvp[1] + lg.maxz * mvp[2] + mvp[3]) / (lg.maxx * mvp[12] + lg.miny * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * windoww);
  object_cuboidp6.append((-(lg.maxx * mvp[0] + lg.maxy * mvp[1] + lg.maxz * mvp[2] + mvp[3]) / (lg.maxx * mvp[12] + lg.maxy * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * windoww);
  object_cuboidp7.append((-(lg.minx * mvp[0] + lg.maxy * mvp[1] + lg.maxz * mvp[2] + mvp[3]) / (lg.minx * mvp[12] + lg.maxy * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * windoww);
  object_cuboidp8.append((-(lg.minx * mvp[0] + lg.miny * mvp[1] + lg.maxz * mvp[2] + mvp[3]) / (lg.minx * mvp[12] + lg.miny * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * windoww);

  object_cuboidp1.append(((lg.maxx * mvp[4] + lg.miny * mvp[5] + lg.minz * mvp[6] + mvp[7]) / (lg.maxx * mvp[12] + lg.miny * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * windowh);
  object_cuboidp2.append(((lg.maxx * mvp[4] + lg.maxy * mvp[5] + lg.minz * mvp[6] + mvp[7]) / (lg.maxx * mvp[12] + lg.maxy * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * windowh);
  object_cuboidp3.append(((lg.minx * mvp[4] + lg.maxy * mvp[5] + lg.minz * mvp[6] + mvp[7]) / (lg.minx * mvp[12] + lg.maxy * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * windowh);
  object_cuboidp4.append(((lg.minx * mvp[4] + lg.miny * mvp[5] + lg.minz * mvp[6] + mvp[7]) / (lg.minx * mvp[12] + lg.miny * mvp[13] + lg.minz * mvp[14] + mvp[15]) + 1) / 2 * windowh);
  object_cuboidp5.append(((lg.maxx * mvp[4] + lg.miny * mvp[5] + lg.maxz * mvp[6] + mvp[7]) / (lg.maxx * mvp[12] + lg.miny * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * windowh);
  object_cuboidp6.append(((lg.maxx * mvp[4] + lg.maxy * mvp[5] + lg.maxz * mvp[6] + mvp[7]) / (lg.maxx * mvp[12] + lg.maxy * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * windowh);
  object_cuboidp7.append(((lg.minx * mvp[4] + lg.maxy * mvp[5] + lg.maxz * mvp[6] + mvp[7]) / (lg.minx * mvp[12] + lg.maxy * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * windowh);
  object_cuboidp8.append(((lg.minx * mvp[4] + lg.miny * mvp[5] + lg.maxz * mvp[6] + mvp[7]) / (lg.minx * mvp[12] + lg.miny * mvp[13] + lg.maxz * mvp[14] + mvp[15]) + 1) / 2 * windowh);

  object_cuboidp.append(object_cuboidp1);
  object_cuboidp.append(object_cuboidp2);
  object_cuboidp.append(object_cuboidp3);
  object_cuboidp.append(object_cuboidp4);
  object_cuboidp.append(object_cuboidp5);
  object_cuboidp.append(object_cuboidp6);
  object_cuboidp.append(object_cuboidp7);
  object_cuboidp.append(object_cuboidp8);
  return (object_cuboidp);
}
#endif
