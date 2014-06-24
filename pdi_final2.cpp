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


IplImage* GetThresholdedImage(IplImage* img){
    // Convert the image into an HSV image
    IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
    cvCvtColor(img, imgHSV, CV_BGR2HSV);
    IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);
    cvInRangeS(imgHSV, cvScalar(0,0,120), cvScalar(50,50,255), imgThreshed);
    cvReleaseImage(&imgHSV);
    return imgThreshed;
}

/*
void trackObject(IplImage* imgThresh){
// Calculate a moments of 'imgThresh'
CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
cvMoments(imgThresh, moments, 1);
double moment10 = cvGetSpatialMoment(moments, 1, 0);
double moment01 = cvGetSpatialMoment(moments, 0, 1);
double area = cvGetCentralMoment(moments, 0, 0);
 
     // if a area<1000, we cruise which a there have been no intent in a design as good as it's since of a noise, a area is not zero
if(area>1000){
        // work out a on all sides of a ball
int posX = moment10/area;
int posY = moment01/area;
 
       if(lastX>=0 && lastY>=0 && posX>=0 && posY>=0)
{
// Draw a yellow line from a prior indicate to a stream point
cvLine(imgTracking, cvPoint(posX, posY), cvPoint(lastX, lastY), cvScalar(0,0,255), 4);
}
 
lastX = posX;
lastY = posY;
}
 
free(moments);
}
 
*/
int main(int argc, char *argv[])
{

if (argc < 2) return -1;
    LOG("Open video and access its frames : ./pdi_final <videoname.avi> <x> <y>");

    const char *file1 = argv[1];

    /*VideoCapture cap(file1); // open video file
    if(!cap.isOpened())  { // check if we succeeded
        LOG("Cannot read video file");
        return -1;
    }*/

   

    // namedWindow("window", 1);
    int iLowH = 0;
    int iHighH = 50;

    int iLowS = 0; 
    int iHighS = 50;

    int iLowV = 120;
    int iHighV = 255;

  
    // Initialize capturing live feed from the camera
    CvCapture* capture = 0;
    capture = cvCaptureFromFile(argv[1]);

    // Couldn't get a device? Throw an error and quit
    if(!capture)
    {
        printf("Could not initialize capturing...\n");
        return -1;
    }
    // The two windows we'll be using
    cvNamedWindow("video");
    cvNamedWindow("thresh");
    
    // This image holds the "scribble" data...
    // the tracked positions of the ball
    IplImage* imgScribble = NULL;
    
     // An infinite loop
    while(true)
    {
        // Will hold a frame captured from the camera
        IplImage* frame = 0;
        frame = cvQueryFrame(capture);
        
        // If we couldn't grab a frame... quit
        if(!frame)
            break;
            
        // If this is the first frame, we need to initialize it
        if(imgScribble == NULL)
        {
            imgScribble = cvCreateImage(cvGetSize(frame), 8, 3);
        }
        
        // Holds the yellow thresholded image (yellow = white, rest = black)
        IplImage* imgThresh = GetThresholdedImage(frame);
        
        // Calculate the moments to estimate the position of the ball
        CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
        cvMoments(imgThresh, moments, 1);

        // The actual moment values
        double moment10 = cvGetSpatialMoment(moments, 1, 0);
        double moment01 = cvGetSpatialMoment(moments, 0, 1);
        double area = cvGetCentralMoment(moments, 0, 0);
        
        // Holding the last and current ball positions
        static int posX = 0;
        static int posY = 0;

        int lastX = posX;
        int lastY = posY;

        posX = moment10/area;
        posY = moment01/area;
        
        // Print it out for debugging purposes
        printf("position (%d,%d)\n", posX, posY);
        
        // We want to draw a line only if its a valid position
        if(lastX>0 && lastY>0 && posX>0 && posY>0)
        {
            // Draw a yellow line from the previous point to the current point
            cvLine(imgScribble, cvPoint(posX, posY), cvPoint(lastX, lastY), cvScalar(0,255,255), 5);
        }
        
        // Add the scribbling image and the frame...
        cvAdd(frame, imgScribble, frame);
        cvShowImage("thresh", imgThresh);
        cvShowImage("video", frame);
        
        // Wait for a keypress
        int c = cvWaitKey(10);
        if(c!=-1)
        {
            // If pressed, break out of the loop
            break;
        }
        
        // Release the thresholded image+moments... we need no memory leaks.. please
        cvReleaseImage(&imgThresh);
        delete moments;
    }
    
  // We're done using the camera. Other applications can now use it
    cvReleaseCapture(&capture);
    return 0;
}
