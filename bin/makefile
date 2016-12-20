SOURCE_PATH = ../src/
INCLUDE_PATH = ../include/
TEST_PATH = ../test/
VPATH = $(SOURCE_PATH) $(INCLUDE_PATH) $(TEST_PATH)

CXXFLAGS = -std=c++11 -pedantic -Wall -Werror -g -funsigned-char -I../include/ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -D LINUX
OFLAGS = -g -I../include/

OPENCV_FLAGS = -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -lcurl -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -lopencv_videoio -lopencv_imgcodecs -lopencv_xfeatures2d -lopencv_flann

OBJS = test.o camera.o
TEST_OBJS := $(basename $(OBJS))
TEST_OBJS := $(addsuffix _test.o, $(TEST_OBJS))
TEST_OUTS := $(addsuffix ut, $(TEST_OBJS))

.PHONY: clean

main.out: $(OBJS)
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

stitching.out: stitch_test.o camera.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

surf.out: surf.o camera.o lilo.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

ptz.out: ptz.o camera.o VirtualCamera.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

surf_ptz.out: surf_ptz.o camera.o VirtualCamera.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

online_cam.out: onlineCamera.o
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

clean:
	rm -f *.o *.out
