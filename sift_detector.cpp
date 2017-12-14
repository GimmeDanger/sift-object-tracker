#include "sift_detector.h"

#include <iostream>

void sift_detector::detect (Mat &orig_scene)
{
	Mat scene = orig_scene.clone ();
	cvtColor (scene, scene, CV_BGR2GRAY);

  std::vector<KeyPoint> scene_keypoints;
  Mat scene_descriptors;

	f2d->detect (scene, scene_keypoints);
	f2d->compute (scene, scene_keypoints, scene_descriptors);

	std::vector<std::vector<DMatch> > matches;
	matcher.knnMatch (marker_descriptors, scene_descriptors, matches, 2);

	std::vector<DMatch> good_matches;
	for (int k = 0; k < std::min (scene_descriptors.rows - 1, (int) matches.size()); k++)
		{
			if ((matches[k][0].distance < 0.6 * (matches[k][1].distance)) &&
					((int) matches[k].size() <= 2 && (int) matches[k].size() > 0))
				{
					good_matches.push_back (matches[k][0]);
				}
		}

	if (good_matches.size () > 3)
		localize_in_scene (good_matches, scene_keypoints, orig_scene);	
}

void sift_detector::localize_in_scene (const std::vector<DMatch> &good_matches,
	const std::vector<KeyPoint> &scene_keypoints, Mat &orig_scene)
{
	std::vector<Point2f> marker_points;
	std::vector<Point2f> scene_points;
	for (int i = 0; i < good_matches.size(); i++)
		{
			marker_points.push_back (marker_keypoints[good_matches[i].queryIdx].pt);
			scene_points.push_back (scene_keypoints[good_matches[i].trainIdx].pt);
		}

	try
	  {
	  	std::vector<Point2f> scene_corners(4);
			Mat H = findHomography (marker_points, scene_points, CV_RANSAC);									
			perspectiveTransform (marker_corners, scene_corners, H);

			Point2f marker_left_corner = Point2f (0, 0);
			
			line (orig_scene, scene_corners[0], scene_corners[1], Scalar (255, 0, 0), 4);
			line (orig_scene, scene_corners[1], scene_corners[2], Scalar (255, 0, 0), 4);
			line (orig_scene, scene_corners[2], scene_corners[3], Scalar (255, 0, 0), 4);
			line (orig_scene, scene_corners[3], scene_corners[0], Scalar (255, 0, 0), 4);

		} catch (Exception &e) {}	
}