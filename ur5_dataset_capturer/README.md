
# usb_cam
  based on usb_cam [![Build Status](https://api.travis-ci.org/bosch-ros-pkg/usb_cam.png)](https://travis-ci.org/bosch-ros-pkg/usb_cam)


## 2020.05.01 Ning chuanfang
1. control ur5 move to planned poses
2. write json file : camera pose , object pose and projected_cuboid
3. subscribe camera image_raw topic, write rgb and depth image using opencv
4. write segmentation image using glut


manual
step 
 1. 设置相机参数:
   参数文件: Gazebo_plan_rviz/ur5_single_arm_tufts/urdf/sensors/kinect/kinect.gazebo.xacro
   type: 深度相机 <sensor name="${name}_camera" type="depth" >
         摄像机形状mesh : filename="package://ur5_single_arm_tufts/meshes/sensors/kinect/kinect.dae"
   内参:  摄像机校正后会生成参数文件  ~/.ros/camera_info/head_camera.yaml,将相应的真实数据填写到摄像机配置文件中，可以设置的参数有
        <horizontal_fov>0.8934</horizontal_fov>  
        <near>0.01</near>
        <far>2</far>
        <Cx>321.74</Cx>
        <Cy>238.02</Cy>
   话题设置: gazebo imageTopicName
            depth image: ${camera_name}_ir/depth/image_raw
            rgb image:  ${camera_name}/depth/image_raw 
          真实相机话题，一般笔记本相机会是 /usb_cam/image_raw
   offset :在文件 Gazebo_plan_rviz/ur5_single_arm_tufts/urdf/ur5_single_arm.urdf.xacro中设置
          <xacro:kinect name="camera" parent="ee_link" >
            <origin xyz="0.06 0.0 0.00" rpy="0.0 0.00 0.00" />
          </xacro:kinect>
 2. 设置工作台:
    在文件 Gazebo_plan_rviz/ur5_single_arm_tufts/urdf/ur5_single_arm.urdf.xacro中设置
    在片段 <link name="table">中
      可以设置几何中心、几何尺寸、材质颜色 和 碰撞尺寸

 3. 设置训练目标:
    在文件 Gazebo_plan_rviz/ur5_single_arm_tufts/urdf/ur5_single_arm.urdf.xacro中设置
    在片段 <link name="block">中
      可以设置几何图像文件、几何中心、Z方向旋转角度、几何尺寸、材质颜色 和 碰撞尺寸
    STL model type : binary STL
    mesh filename 可以用绝对路径或者相对路径(package://ur5_single_arm_tufts/meshes/LEGOCbinary.stl)以及缩放比例

4. robot_motion:  
    robot_motion.h 
      相机参数：
        #define zNear 0.1f
        #define zFar 3.0f
        #define fx 668.0703112366639
        #define fy 668.0703112366639
        #define cx 321.74
        #define cy 238.02
        #define caml 0.06
      训练目标：
        #define LEGO_POSITION_X 0.4
        #define LEGO_POSITION_Y 0.55
        #define LEGO_POSITION_Z 1.1
        #define LEGO_ROTATION_EULER_Z -120 //LEGO euler z rotation degree

    robot_motion.cpp
      着色器：line 44-45
        const char *pVSFileName = "/home/ncf/catkin/src/usb_cam/src/shader.vs";
        const char *pFSFileName = "/home/ncf/catkin/src/usb_cam/src/shader.fs";
      订阅话题： line 46-47
        rgb图像话题 std::string rgbTopic = "/camera_kinect_head_mount/depth/image_raw";
        depth图像话题 std::string depthTopic = "/camera_kinect_head_mount_ir/depth/image_raw";
      模型文件路径: line 69
        lg = readstl("/home/ncf/ROS/LEGOCbinary.stl");
      输出文件 ：
        filename = "/home/ncf/ROS/json/" + 顺序编号 + ".json";   json文件 line 232
        filename1s = "/home/ncf/ROS/json/" + 顺序编号 + "s.jpg"; 分段图  line 234
        rgbfilename = "/home/ncf/ROS/json/" + 顺序编号 + "r.png";  rgb图 line 403
        depthfilename = "/home/ncf/ROS/json/" + 顺序编号 + "d.png"; 深度图 line 411

to run program:
1. startup Gazebo Rviz and Planning server
`roslaunch ur5_single_arm_manipulation gazebo_plan_rviz.launch`
2. run this program
`rosrun usb_cam robot_motion`




