/*
class imageconverter： 
  subscribe rgb image topic
  write image file 
class imageconverted：
  subscribe depth image topic
  write image file 

bug: strange filename occasionally . overwrite?
*/

#include "imageCapturer.h"
#include <sys/stat.h>
using namespace cv;

//==================================================================
//function name：  file_exists
 //author： Ning Chuanfang
 //date： 2020-04-22
 //function： Judge whether the file exists
 //input parameters：
 //          name  file's path 
 //return：  type（boolean)
 //          true : file exists
 //          false :file not exists
 //modify：
 //
//==================================================================
bool file_exists(const std::string &name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

//==================================================================
//class name：  imageCapturerRGB
 //author： Ning Chuanfang
 //date： 2020-04-22
 //function：subscribe rgb image topic write image file 
 //
 //input parameters：
 //          filename  file's path 
 //member function:
 //
 //modify：
 //
//==================================================================

//Constructor
imageCapturerRGB::imageCapturerRGB(string fn1) : it_(nh_) { 
  filename = fn1.c_str();
}

//Destructor
imageCapturerRGB::~imageCapturerRGB() { 
  // cv::destroyWindow(OPENCV_WINDOW);
}

//callback function copy image topic into opencv and write it into file
void imageCapturerRGB::imageCb(const sensor_msgs::ImageConstPtr &msg) {
  int l,n = 1;
  l = filename.length();
  n = filename.compare(l-4, 4, ".png"); // filename should be ended with .png
  if(n == 0){
    while (!file_exists(filename.c_str()) && !working) {
      working = true;   //should not been called again when working
      cv_bridge::CvImagePtr cv_ptr; //image handler
      //copy image_raw topic into cv_ptr with BGR8 format
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
      try {
        if(filename.compare(1, 4, "/dat") == 0){  //todo:filename were overlaped by GOD 
          cv::imwrite(filename.c_str(), cv_ptr->image); //write image into file
          working = false;
        } else {
          printf("r filename is wrong\n");
        }
      } catch (runtime_error &ex) {
        fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
        return;
      }
    }
  }else{
    printf("imageCapturerRGB:wrong format:%s %s\n",filename.c_str(),filename.substr(l-4).c_str());
  }
  // cv::destroyAllWindows();
}
//member function  call set_filename to set imagefilename and begin subcribe topic
void imageCapturerRGB::set_filename(string fn, std::string imageTopic) {
  filename = fn;
  working = false;
  image_sub_ = it_.subscribe(imageTopic.c_str(), 1, &imageCapturerRGB::imageCb, this);
}


//==================================================================
//class name：  imageCapturerDepth
 //author： Ning Chuanfang
 //date： 2020-04-22
 //function：subscribe depth image topic write image file 
 //
 //input parameters：
 //          filename  file's path 
 //member function:
 //
 //modify：
 //
//==================================================================

//Constructor
imageCapturerDepth::imageCapturerDepth(string fn1) : it_(nh_) { 
  filename = fn1.c_str();                               
}

//Destructor
imageCapturerDepth::~imageCapturerDepth() { 
  // cv::destroyWindow(OPENCV_WINDOW);
}

//callback function copy image topic into opencv and write it into file
void imageCapturerDepth::imageCbDepth(const sensor_msgs::ImageConstPtr &msg) {
  int l,n = 1;
  l = filename.length();
  n = filename.compare(l-4, 4, ".png"); // file type
  if(n == 0){
    cv_bridge::CvImageConstPtr cv_ptr;
    cv_ptr = cv_bridge::toCvShare(msg);
    Mat mono8_img = cv::Mat(cv_ptr->image.size(), CV_8UC1);
    cv::convertScaleAbs(cv_ptr->image, mono8_img, 100, 0.0);
    while (!file_exists(filename.c_str()) && !working) {
      working = true;
      try {
        if(filename.compare(1, 4, "/dat") == 0){  //todo : find the bug filename overlay
          cv::imwrite(filename.c_str(), mono8_img); //write file 
          working = false;
        } else {
          printf("imageCapturerDepth filename is wrong\n");
        }
      } catch (runtime_error &ex) {
        printf("imageCapturerDepth fn=%s %s %s\n",filename.c_str(),filename.substr(1,4).c_str(),ex.what());
        return;
      }
    }
  }else{
    printf("imageCapturerDepth wrong file type:%s\n",filename.c_str());
  }
}

//member function  call set_filename to set imagefilename and begin subcribe topic
void imageCapturerDepth::set_filename(string fn, std::string imageTopic) {
  filename = fn;
  working = false;
  image_sub_ = it_.subscribe(imageTopic.c_str(), 1, &imageCapturerDepth::imageCbDepth, this);
}
