#include <iostream>
#include <string>
#include <functional>

#include "opencv_common.h"
#include "sift_detector.h"

using namespace cv;

// How to compile: g++ -std=c++14 main.cpp sift_detector.cpp -o sift `pkg-config --cflags --libs opencv`

void process_video (std::function<void (Mat &)> func, std::string window_name);

int main (int argc, char **argv)
{
  if (argc < 2)
  {
    std::cout << "Need marker image file name!" << std::endl;
    return -1;
  }
  namedWindow ("Original image", WINDOW_NORMAL);
  
  Mat marker = imread (argv[1]);
  sift_detector detector (marker);
  imshow ("Original image", marker);
  waitKey (0);

  auto f = [&detector] (Mat &im) { detector.detect (im); };
	process_video (f, std::string ("Video"));
	return 0;
}

void process_video (std::function<void (Mat &)> func, std::string window_name)
{
  VideoCapture cap (CV_CAP_ANY);
  if (!cap.isOpened())
    {
      std::cout << "Cannot found a specified webcam." << std::endl;
      return;
    }
  
  cvNamedWindow (window_name.c_str (), CV_WINDOW_AUTOSIZE);
   
  Mat orig_frame, processed_frame;
  
  char key = 0;
  while (key != 27) // ESC
    {          
      if (!cap.read (orig_frame))
      {
          std::cout << "Error: cvQueryFrame has failed." << std::endl;
          break;
      }

      processed_frame = orig_frame.clone ();

      func (processed_frame);

      imshow (window_name.c_str (), processed_frame);

      // Exit when user press ESC
      key = cvWaitKey(10);
    }
}