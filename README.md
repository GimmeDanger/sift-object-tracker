# sift-object-tracker

Purpose: find object location on webcam frame and track it with opencv lib

Required libs: opencv with opencv_contrib modules (sift features)

Compilation: g++ -std=c++14 -O3 main.cpp sift_detector.cpp -o sift_detector 'pkg-config --cflags --libs opencv'

Run: ./sift_detector <path_to_marker_image>
