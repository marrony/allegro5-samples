CC_SRCS=$(wildcard *.c)
CC_PROGS=$(addprefix bin/, $(patsubst %.c,%,$(CC_SRCS)))

CXX_SRCS=$(wildcard *.cpp)
CXX_PROGS=$(addprefix bin/, $(patsubst %.cpp,%,$(CXX_SRCS)))

all: $(CC_PROGS) $(CXX_PROGS)

LIBS=`pkg-config --libs allegro-5.0 allegro_main-5.0 allegro_image-5.0 allegro_audio-5.0 allegro_acodec-5.0 allegro_primitives-5.0 allegro_font-5.0 allegro_ttf-5.0`

bin/%: %.c
	@mkdir -p bin
	$(CC) -std=c99 -Wall -g -o $@ $< $(LIBS)

bin/%: %.cpp
	@mkdir -p bin
	$(CXX) -Wall -g -o $@ $< $(LIBS)

