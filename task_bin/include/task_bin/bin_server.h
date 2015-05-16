#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <task_bin/binAction.h>
#include <actionlib/server/simple_action_server.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;

typedef actionlib::SimpleActionServer<task_bin::binAction> Server;

class Bin
{
private:
	ros::NodeHandle _n;
	Server _s;
	image_transport::ImageTransport _it;
	image_transport::Subscriber _sub;
	image_transport::Publisher _pub;
	Mat _image, templ;
	task_bin::binFeedback _feedback;
	task_bin::binResult _result;
	std::string _actionName;

public:
	Bin(std::string_name);
	void executeCB(const task_bin::binGoalConstPtr &_goal);
	void imageCallBack(const sensor_msgs::ImageConstPtr &msg);
	Point tempMatch(Mat feed_image, Mat templ);
	~Bin();
}