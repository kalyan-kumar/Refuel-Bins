#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
Mat img; Mat templ; Mat result; Mat templ1; Mat templ2; Mat templ3; Mat templ4;
char* const image_window = "Source Image";
char* const result_window = "Result window";

//int match_method;
//int max_Trackbar = 5;

/// Function Headers
void MatchingMethod( int, void* );

/** @function main */
int main( int argc, char** argv )
{
  /// Load image and template
  img = imread("/home/kalyan/RoboSub2015_silhouettes0.png");
  templ1 = imread("/home/kalyan/RoboSub2015_silhouettes1.png");
  templ2 = imread("/home/kalyan/RoboSub2015_silhouettes2.png");
  templ3 = imread("/home/kalyan/RoboSub2015_silhouettes3.png");
  templ4 = imread("/home/kalyan/RoboSub2015_silhouettes4.png");
  
  /// Create windows
  namedWindow( image_window, CV_WINDOW_AUTOSIZE );
  namedWindow( result_window, CV_WINDOW_AUTOSIZE );

  /// Create Trackbar
  //char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
  //createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );

  MatchingMethod( 0, 0 );

  waitKey(0);
  return 0;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void* )
{
  /// Source image to display
  Mat img_display;
  img.copyTo( img_display );

  /// Create the result matrix
  int result_cols =  img.cols - templ3.cols + 1;
  int result_rows = img.rows - templ3.rows + 1;

  result.create( result_rows, result_cols, CV_32FC1 );

  /// Do the Matching and Normalize
  //Here, we are using SQDIFF_NORMED matching method

  matchTemplate( img, templ3, result, 1 );
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

  matchLoc = minLoc;

  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  //if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    //{ matchLoc = minLoc; }
  //else
    //{ matchLoc = maxLoc; }

  /// Show me what you got
  rectangle( img_display, matchLoc, Point( matchLoc.x + templ1.cols , matchLoc.y + templ1.rows ), Scalar::all(0), 2, 8, 0 );
  rectangle( result, matchLoc, Point( matchLoc.x + templ1.cols , matchLoc.y + templ1.rows ), Scalar::all(0), 2, 8, 0 );

  imshow( image_window, img_display );
  imshow( result_window, result );

  return;
}