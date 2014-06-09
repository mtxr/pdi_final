#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
// #include "cv.h"
// #include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

#define DEBUG_MODE
/*
imprime as mensagens de saida se DEBUG_MODE estiver definido
a macro __func__ imprime o nome da função que chamou o log
*/

#ifdef DEBUG_MODE
#define LOG(str) do { cout << "[" << __func__ <<"]=> " << str << endl; } while(false)
#else
#define LOG(str) do { } while(false)
#endif



int main(int argc, char *argv[])
{

      LOG("Open video and access its frames : ./pdi_final <videoname.avi> <x> <y>");
      if (argc < 2) return -1;

      const char *file1 = argv[1];

      VideoCapture cap(file1); // open video file
      if(!cap.isOpened())  { // check if we succeeded
            LOG("Cannot read video file");
            return -1;
      }

      Mat frame;
      cap >> frame; // get a new frame from camera

      namedWindow("window", 1);
      while (!frame.empty())
      {
            cvtColor(frame, frame, CV_BGR2HSV);
            vector<Mat> imColors(3);
            split(frame, imColors);

            if(waitKey(30) >= 0) break;
            imshow("window", imColors[2]);
            cap >> frame; // get a new frame from camera
      }
      return 0;
}
