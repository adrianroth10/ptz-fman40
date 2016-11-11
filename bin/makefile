SOURCE_PATH = ../src/
INCLUDE_PATH = ../include/
TEST_PATH = ../test/
VPATH = $(SOURCE_PATH) $(INCLUDE_PATH) $(TEST_PATH)

CXXFLAGS = -std=c++11 -pedantic -Wall -Werror -g -funsigned-char -I../include/
OFLAGS = -g -I../include/

OPENCV_FLAGS = -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -lopencv_videoio

OBJS = test.o
TEST_OBJS := $(basename $(OBJS))
TEST_OBJS := $(addsuffix _test.o, $(TEST_OBJS))
TEST_OUTS := $(addsuffix ut, $(TEST_OBJS))

.PHONY: clean test

main.out: $(OBJS)
	$(CXX) $(OFLAGS) -o $@ $^ $(OPENCV_FLAGS)

clean:
	rm -f *.o *.out


test: $(TEST_OUTS)
	$(foreach X, $(TEST_OUTS), ./$X)

%_test.out: %_test.o %.o
	$(CXX) $(OFLAGS) -o $@ $^
