.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h
LDFLAGS = -Llib/FastPFor -llibFastPFOR
# replace the CXX variable with a path to a C++11 compatible compiler.
# to build an aligned version, add -DUSE_ALIGNED=1
ifeq ($(INTEL), 1)
# if you wish to use the Intel compiler, please do "make INTEL=1".
    CXX ?= /opt/intel/bin/icpc
    CC ?= /opt/intel/bin/icpc
ifeq ($(DEBUG),1)
    CXXFLAGS = -fpic -std=c++11 -g -Wall -ansi -xAVX -DDEBUG=1 -D_GLIBCXX_DEBUG -ggdb 
    CCFLAGS = -fpic -std=c99 -g -Wall  -ansi -xAVX -DDEBUG=1 -D_GLIBCXX_DEBUG -ggdb 
else
    CXXFLAGS = -fpic -std=c++11 -g -Wall  -ansi -xAVX -DNDEBUG=1 -ggdb 
    CCFLAGS = -fpic -std=c99 -g -Wall   -ansi -xAVX -DNDEBUG=1 -ggdb 
endif # debug
else #intel
    CXX ?= g++-4.7
ifeq ($(DEBUG),1)
    CXXFLAGS = -fpic -mavx -std=c++11  -Weffc++ -pedantic -g -ggdb -DDEBUG=1 -D_GLIBCXX_DEBUG -Wall  -Wextra -Wextra -Wsign-compare  -Wwrite-strings -Wpointer-arith -Winit-self  -Wno-sign-conversion
    CCFLAGS = -fpic -mavx -std=c99  -pedantic -g -ggdb -DDEBUG=1 -D_GLIBCXX_DEBUG -Wall  -Wextra -Wsign-compare -Wwrite-strings -Wpointer-arith -Winit-self  -Wno-sign-conversion 
else
    CXXFLAGS = -fpic -mavx -std=c++11  -Weffc++ -pedantic -g -Wall  -Wextra -Wsign-compare  -Wwrite-strings -Wpointer-arith -Winit-self  -Wno-sign-conversion 
    CCFLAGS = -fpic -mavx -std=c99 -pedantic -g -Wall -Wextra -Wsign-compare -Wwrite-strings -Wpointer-arith -Winit-self -Wno-sign-conversion 
endif #debug
endif #intel





HEADERS= $(shell ls include/*.h) $(shell ls include/ewah/*.h) $(shell ls include/SIMDPFor/*.h) $(shell ls include/vf2lib/*.h)

all: Timer.o HRTree.o HRTreeIVSPFor.o HGraph.o GraphReader.o BuildManager.o HRTreeIV.o SubTempora 
	echo "Build completed"


Timer.o: include/Timer.h src/Timer.cpp 
	$(CXX) $(CXXFLAGS) -c src/Timer.cpp -Iinclude

HRTree.o: include/Timer.h include/ewah/ewah.h include/HRTree.h src/HRTree.cpp 
	$(CXX) $(CXXFLAGS) -c src/HRTree.cpp -Iinclude

HRTreeIV.o: $(HEADERS) src/HRTreeIV.cpp
	$(CXX) $(CXXFLAGS) -c src/HRTreeIV.cpp -Iinclude 

HRTreeIVSPFor.o: include/Timer.h include/HRTree.h include/HRTreeIVSPFor.h src/HRTreeIVSPFor.cpp
	$(CXX) $(CXXFLAGS) -c src/HRTreeIVSPFor.cpp -Iinclude

HGraph.o: include/HRTree.h include/HGraph.h src/HGraph.cpp
	$(CXX) $(CXXFLAGS) -c src/HGraph.cpp -Iinclude

GraphReader.o: include/Timer.h include/LabelMap.h include/HGraph.h include/GraphReader.h src/GraphReader.cpp
	$(CXX) $(CXXFLAGS) -c src/GraphReader.cpp -Iinclude

BuildManager.o: include/HRTree.h include/HRTreeIV.h include/ewah/ewah.h include/HRTreeIVSPFor.h include/GraphReader.h include/BuildManager.h src/BuildManager.cpp
	$(CXX) $(CXXFLAGS) -c src/BuildManager.cpp -Iinclude


UNAME := $(shell uname)

OBJECTS=Timer.o HRTree.o HRTreeIVSPFor.o HGraph.o GraphReader.o BuildManager.o HRTreeIV.o

SubTempora: include/BuildManager.h include/SubTempora.h src/SubTempora.cpp 
	$(CXX) $(CXXFLAGS) -o SubTempora.exe src/SubTempora.cpp $(OBJECTS) -Iinclude -Llib/SIMDPFor -lSIMDCompressionAndIntersection -Llib/vf2lib -lvf

clean:
	rm -f *.o Timer.o HRTree.o Timer.o HRTree.o HRTreeIVSPFor.o HGraph.o GraphReader.o BuildManager.o HRTreeIV.o
	rm -f Results/HRTree/*
	rm -f Results/HRTreeIVSIMDPFor/*
	rm -f Results/HRTreeBMEWAH/*

astyle:
	astyle --options=astyle.conf --recursive "*.cpp" "*.h"

.PHONY: all clean astyle
