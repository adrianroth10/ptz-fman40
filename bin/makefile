SOURCE_PATH = ../src/
INCLUDE_PATH = ../include/
VPATH = $(SOURCE_PATH) $(INCLUDE_PATH)

CXXFLAGS = -std=c++11 -pedantic -Wall -Werror -g -funsigned-char -I../include/ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -D LINUX
OFLAGS = -g -I../include/ -O3

OPENCV_FLAGS = -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -lcurl -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -lopencv_videoio -lopencv_imgcodecs -lopencv_xfeatures2d -lopencv_flann

.PHONY: all clean

all: calc_homography.out calib.out live_action.out online_cam.out stitch_results.out surf.out surf_ptz.out test_camera.out

calc_homography.out: calc_homography.o Camera.o Lilo.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

calib.out: camera_calibration.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

live_action.out: live_action.o Camera.o VirtualCamera.o Lilo.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

online_cam.out: onlineCamera.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

ptz.out: ptz.o Camera.o VirtualCamera.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

stitching.out: stitch_test.o Camera.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

stitch_results.out: stitch_results.o Camera.o Lilo.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

surf.out: surf.o Camera.o Lilo.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

surf_ptz.out: surf_ptz.o Camera.o VirtualCamera.o Lilo.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

test_camera.out: test_camera.o Camera.o Lilo.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

clean:
	rm -f *.o *.out
