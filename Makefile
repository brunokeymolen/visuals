UNAME := $(shell uname -s)
CXX=g++
CC=gcc
OPTFLAGS=-g3 -O0
#OPTFLAGS=-g0 -O3
CXXFLAGS=-Wall -I. -I../../ -I./3rd/libcflie/ -I/usr/local/include -fPIC $(OPTFLAGS) -std=c++0x
CFLAGS=-Wall -fPIC $(OPTFLAGS)
LDFLAGS= -L/usr/local/lib -lpthread  $(OPTFLAGS)

#/usr/lib/x86_64-linux-gnu/libusb-1.0.so

#boost
#LDFLAGS+= -lboost_filesystem -lboost_system -lboost_date_time

#openCV
#CXXFLAGS+= -I ../../tv -I/opt/fncx/opencv/2.4.2/include
#LDFLAGS+= -lopencv_highgui -lopencv_core -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_imgproc -lopencv_objdetect -lopencv_ts
#LDFLAGS+= -L/opt/fncx/opencv/2.4.2/lib -Wl,-rpath,/opt/fncx/opencv/2.4.2/lib

#ifeq ($(UNAME),Darwin)
#	LDFLAGS+= -lboost_thread 
#else
#	LDFLAGS+= -lboost_thread-mt
#endif


SRC = 	main.o \
		demo.o  \
		drawtools.o \
		visualcore.o \
		trackball.o \

MYLIBS = 

#MODULES = 
#	../match_common/JSONValue.o \
#	../match_common/logging.o \
#	../match_common/urls.o \
#	../match_common/util.o \
#	../match_common/service.o 
	
OPENCVFLAGS+= `pkg-config opencv --cflags --libs`  
GLFLAGS= `pkg-config glu --cflags --libs` -lglut

all: visuals 

visuals: $(SRC) $(MODULES)
	$(CXX) $(LDFLAGS) $(MODULES) $(SRC) $(OPENCVFLAGS) $(MYLIBS) $(GLFLAGS) -o visuals

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o ../../tv/*.o visuals

PREFIX ?= /usr

install: all
	install -d $(PREFIX)/bin
	install img_trans  $(PREFIX)/bin

dependencies:
	sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev

.PHONY: clean all visuals install
