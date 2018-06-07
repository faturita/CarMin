
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\nonfree\nonfree.hpp>

#include <QGlobal.h>
#include <QTime>

using namespace cv;
using namespace std;


int a() {
     const char* filename;
    Mat src = imread(filename, 0);
    Mat dst, cdst;

//  vector<Vec2f> lines;
//  HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );

//  for( size_t i = 0; i < lines.size(); i++ )
//  {
//     float rho = lines[i][0], theta = lines[i][1];
//     Point pt1, pt2;
//     double a = cos(theta), b = sin(theta);
//     double x0 = a*rho, y0 = b*rho;
//     pt1.x = cvRound(x0 + 1000*(-b));
//     pt1.y = cvRound(y0 + 1000*(a));
//     pt2.x = cvRound(x0 - 1000*(-b));
//     pt2.y = cvRound(y0 - 1000*(a));
//     line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
//  }

  vector<Vec4i> lines;
  HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
  for( size_t i = 0; i < 2 /*lines.size()*/; i++ )
  {
    Vec4i l = lines[i];
    line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);

    double theta = atan2((double)(l[3]-l[1]),(double)(l[2]-l[1]));

    double rho = sqrt ( (double)( pow((double)(l[3]-l[1]),2) + pow((double)(l[2]-l[1]),2) )     );


    theta = theta * (180.0/CV_PI);


    cout << "(" << rho << "," << theta << ")" <<  endl;


  }

 waitKey();

 imshow("source", src);
 imshow("detected lines", cdst);



 return 0;
}



/**
 * @function main
 */
int mafin( int argc, char** argv )
{
  Mat src, dst;

  /// Load image
  src = imread( argv[1], 1 );

  if( !src.data )
    { return -1; }

  /// Separate the image in 3 places ( B, G and R )
  vector<Mat> bgr_planes;
  split( src, bgr_planes );

  /// Establish the number of bins
  int histSize = 256;

  /// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  bool uniform = true; bool accumulate = false;

  Mat b_hist, g_hist, r_hist;

  /// Compute the histograms:
  calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

  // Draw the histograms for B, G and R
  int hist_w = 512; int hist_h = 400;
  int bin_w = cvRound( (double) hist_w/histSize );

  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

  /// Normalize the result to [ 0, histImage.rows ]
  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

  /// Draw for each channel
  for( int i = 1; i < histSize; i++ )
  {
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
  }

  /// Display
  namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
  imshow("calcHist Demo", histImage );

  waitKey(0);

  return 0;
}


