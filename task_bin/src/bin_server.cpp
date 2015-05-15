#include <ros/ros.h>
#include <task_bin/task_bin.h>

Bin::Bin(std::string name): _it(_n), _s(_n, name, boost::bind(&Bin::executeCB, this, _1), false), _actionName(name)
{
	_sub = _it.subscribe(topics::CAMERA_FRONT_RAW_IMAGE, 1, &Buoy::imageCallBack, this);
    _pub = _it.advertise(topics::CAMERA_FRONT_BUOY_IMAGE, 1);
	_s.start();
}

void Bin::executeCB(const task_bin::binGoalConstPtr &_goal)
{
	ros::Rate looprate(10);
	Point templ_center;

	switch(_goal->order)
	{
		case DETECT_SLIHOUETTE;
		{
			while(ros::ok())
            {
                if (_s.isPreemptRequested() || !ros::ok())
                {
                    ROS_INFO("%s: Preempted", _actionName.c_str());
                    _s.setPreempted(_image, hkjhkjh);
                    break;
                }
                templ_center = tempMatch();
                _finalImage.image = _imageBW;
                _finalImagemsg = _finalImage.toImageMsg();
                _pub.publish(_finalImagemsg);

                if(_detected)
                {
                    _result.sequence.push_back(BUOY_DETECTED);
                    break;
                }
                looprate.sleep();
            }
            break;
		}
	}
}

void Bin::imageCallBack(const sensor_msgs::ImageConstPtr &_msg)
{
	cv_bridge::CvImagePtr _imagePtr;
	try
	{
		_imagePtr = cv_bridge::toCvCopy(_msg, "bgr8");
	}
	catch
	{
		ROS_ERROR("Could not convert from '%s' to 'bgr8'.", _msg->encoding.c_str());
	}
	_image = _imagePtr->image;
}

Point tempMatch(Mat feed_image, Mat templ)
{
  	Mat img, result;
  	feed_image.copyTo( img );

	int result_cols =  img.cols - templ.cols + 1;
  	int result_rows = img.rows - templ.rows + 1;

  	result.create( result_rows, result_cols, CV_32FC1 );

  	//Here, we are using SQDIFF_NORMED matching method

  	matchTemplate( img, templ, result, 1 );
  	normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  	double minVal; double maxVal; Point minLoc; Point maxLoc; Point matchLoc;

  	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

  	matchLoc = minLoc;

  	rectangle( img, matchLoc, Point( matchLoc.x + templ1.cols , matchLoc.y + templ1.rows ), Scalar::all(0), 2, 8, 0 );
  	rectangle( result, matchLoc, Point( matchLoc.x + templ1.cols , matchLoc.y + templ1.rows ), Scalar::all(0), 2, 8, 0 );

  	namedWindow( image_window, CV_WINDOW_AUTOSIZE );
  	namedWindow( result_window, CV_WINDOW_AUTOSIZE );

  	imshow( image_window, img );
  	imshow( result_window, result );

  	return Point(matchLoc.x + (templ.cols)/2 , matchLoc.y + (templ.rows)/2 );
}