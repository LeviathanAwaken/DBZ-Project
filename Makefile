CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk

walk: walk.cpp seanF.cpp
	g++ $(CFLAGS) walk.cpp seanF.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -owalk

clean:
	rm -f walk
	rm -f *.o
