#pragma once

#include <vector>
#include <algorithm>

#include "opencv_common.h"
#include "opencv2/xfeatures2d.hpp"

using namespace cv;

class sift_detector
{
private:
	Mat marker;

	std::vector<Point2f> marker_corners;
  std::vector<KeyPoint> marker_keypoints;
  Mat marker_descriptors;
  
  Ptr<Feature2D> f2d;
	FlannBasedMatcher matcher;

  void localize_in_scene (const std::vector<DMatch> &good_matches,
		const std::vector<KeyPoint> &scene_keypoints, Mat &orig_scene);

  // avoid copying or assignment
  sift_detector (const sift_detector &);
  const sift_detector& operator= (const sift_detector &);

public:
	sift_detector (Mat &marker_arg) : marker_corners (4)
	{
		marker = marker_arg.clone ();
		cvtColor (marker, marker, CV_BGR2GRAY);

		marker_corners[0] = cvPoint (0, 0);
		marker_corners[1] = cvPoint (marker.cols, 0);
		marker_corners[2] = cvPoint (marker.cols, marker.rows);
		marker_corners[3] = cvPoint (0, marker.rows);
		
		f2d = xfeatures2d::SIFT::create ();
		f2d->detect (marker, marker_keypoints);
		f2d->compute (marker, marker_keypoints, marker_descriptors);	
	}

	void detect (Mat &scene);
};