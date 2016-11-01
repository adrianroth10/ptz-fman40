SOURCE_PATH = ../src/
INCLUDE_PATH = ../include/
TEST_PATH = ../test/
VPATH = $(SOURCE_PATH) $(INCLUDE_PATH) $(TEST_PATH)

CXXFLAGS = -std=c++11 -pedantic -Wall -Werror -g -funsigned-char -I../include/
OFLAGS = -g -I../include/

#OBJS =
TEST_OBJS := $(basename $(OBJS))
TEST_OBJS := $(addsuffix _test.o, $(TEST_OBJS))
TEST_OUTS := $(addsuffix ut, $(TEST_OBJS))

.PHONY: all clean test

all: main.out $(PROGRAMS)

main.out: $(OBJS) main.o
	$(CXX) $(OFLAGS) -o $@ $^

clean:
	rm -f *.o *.out


test: $(TEST_OUTS)
	$(foreach X, $(TEST_OUTS), ./$X)

%_test.out: %_test.o %.o
	$(CXX) $(OFLAGS) -o $@ $^
