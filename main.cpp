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

void help()
{
 cout << "\nThis program demonstrates line finding with the Hough transform.\n"
         "Usage:\n"
         "./houghlines <image_name>, Default is pic1.jpg\n" << endl;
}


void dosomemath(Mat cdst)
{
    Point source(30,40);
    Point target(10,100);

    line( cdst, source, target,Scalar(0,0,255), 3, CV_AA );

    // atan2( y / x) quadrant signs.

    double f = atan2(-(double)(target.y-source.y),(double)(target.x-source.x)) * (180.0/CV_PI) ;


    cout << "Value:" << f << endl;

    imshow("image", cdst);

    waitKey();

    exit(1);
}


Mat trimImage(Mat src)
{
    // This image contains many white spots.  The image is smaller than the real value.

    // Only Squared Resize.
    Size s = src.size();

    size_t i,j;

    i = s.width/2;
    uchar val = src.at<uchar>(i,0);
    cout << (int)val << std::endl;
    for(j=1;j<s.width;j++)
    {
        cout << (int)(src.at<uchar>(i,j)) << std::endl;
        if (abs((src.at<uchar>(i,j))-val)>4)
        {

            break;
        }
    }

    Mat dst;//dst image

    size_t minheight = j;

    std::cout << "Minheight:" <<  minheight << std::endl;

    dst = src(Rect(0,minheight,s.width,s.height-2*minheight));

    return dst;
}

Mat analyzeLines(Mat cdst,vector<Vec4i> lines )
{
    Size s = cdst.size();

    cv::Mat pps(int(sqrt((double)(s.height*s.height+s.width*s.width))),360,CV_8U,cv::Scalar(0));

    for( size_t i = 0; i < lines.size(); i++ )
    {
      Vec4i l = lines[i];
      line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);

      double theta = atan2(-(double)(l[3]-l[1]),(double)(l[2]-l[0]));

      double rho = sqrt ( (double)( pow((double)(l[3]-l[1]),2) + pow((double)(l[2]-l[0]),2) )     );

      //rho = ((double)(l[3]-l[1]) + cos(theta)/sin(theta) * (double)(l[2]-l[0]) ) * sin(theta);

      double x1=l[0],y1=l[1];
      double x2=l[2],y2=l[3];

      rho = abs( (x2 - x1)*y1 - x1*(y2-y1)) / sqrt ( (double)( pow((double)(l[3]-l[1]),2) + pow((double)(l[2]-l[0]),2) ));


      theta = theta * (180.0/CV_PI) + 180.0;


      cout << "(" << rho << "," << theta << ")" <<  endl;

      imshow("detected lines", cdst);
          imwrite("carfront.png", cdst);

      pps.at<uchar>(rho,theta)=255;

      imshow("points", pps);

      cv::waitKey();
    }

    return pps;


}



int main(int argc, char** argv)
{
    const char* filename = argc >= 2 ? argv[1] : "pic1.jpg";

    Mat src = imread(filename, 0);
    if(src.empty())
    {
        help();
        cout << "can not open " << filename << endl;
        return -1;
    }


    src = trimImage(src);

    imshow("source", src);



    Mat dst, cdst;
    Canny(src, dst, 50, 200, 3);
    cvtColor(dst, cdst, CV_GRAY2BGR);

    imshow("source2", cdst);

    vector<Vec4i> lines;
    HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );

    Mat pps = analyzeLines(cdst,lines);

    string newfilename("pps_");
    string filetosave(filename);

    filetosave = newfilename + filetosave;

    imwrite(filetosave, pps);

    //imshow("source", src);
    imshow("detected lines", cdst);

    cv::waitKey();


    exit(1);
}
