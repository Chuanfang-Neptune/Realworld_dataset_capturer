#ifndef _Imageconverter_H_
#define _Imageconverter_H_

#include <cv_bridge/cv_bridge.h>
#include <string>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
class ImageConverter{
private:
    ros::NodeHandle nh_;  //rosnode handler
    image_transport::ImageTransport it_;    //variable it_ imageconvert
    image_transport::Subscriber image_sub_;  //subscriber handler
    string filename = "test.jpg"; // image file name
    bool working = false;
    public:
    ImageConverter(string);
    ~ImageConverter();
    void imageCb(const sensor_msgs::ImageConstPtr& msg);
    void set_filename(string fn,std::string imageTopic);
};
class ImageConverted{
private:
    ros::NodeHandle nh_;  //rosnode handler
    image_transport::ImageTransport it_;   //variable it_ imageconvert
    image_transport::Subscriber image_sub_;  //subscriber handler
    string filename = "test.jpg"; // image file name
    bool working = false;
    public:
    ImageConverted(string);
    ~ImageConverted();
    void imageCbDepth(const sensor_msgs::ImageConstPtr& msg);
    void set_filename(string fn,std::string imageTopic);
};

bool file_exists (const std::string& name);

#endif     