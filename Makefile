CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk


walk: walk.cpp joshuaA.cpp seanF.cpp juanT.cpp drakeF.cpp lawrenceM.cpp
	g++ $(CFLAGS) walk.cpp joshuaA.cpp seanF.cpp juanT.cpp drakeF.cpp lawrenceM.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -owalk


clean:
	rm -f walk
	rm -f *.o
