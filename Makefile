CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk

mac: lab1mac


walk: walk.cpp joshuaA.cpp seanF.cpp juanT.cpp drakeF.cpp lawrenceM.cpp
	g++ $(CFLAGS) walk.cpp joshuaA.cpp seanF.cpp juanT.cpp drakeF.cpp lawrenceM.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -owalk

lab1mac: walk.cpp joshuaA.cpp seanF.cpp juanT.cpp drakeF.cpp lawrenceM.cpp
	g++ walk.cpp lawrenceM.cpp \
	joshuaA.cpp \
	seanF.cpp \
	juanT.cpp \
	drakeF.cpp \
	-Wextra \
<<<<<<< HEAD
	libggfonts.a -Wall -owalk \
=======
	libggfontsMac.a -Wall -owalk \
>>>>>>> master
	-I/usr/X11R6/include \
	-L/usr/X11R6/lib -lX11 \
	-framework OpenGL \
	-framework Cocoa -lm -lGL \
	-framework OpenAl


clean:
	rm -f walk
	rm -f *.o
