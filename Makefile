CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk

mac: lab1mac


walk: walk.cpp joshuaA.cpp seanF.cpp juanT.cpp drakeF.cpp lawrenceM.cpp Global.cpp
	g++ $(CFLAGS) walk.cpp Global.cpp joshuaA.cpp seanF.cpp juanT.cpp drakeF.cpp lawrenceM.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -owalk

lab1mac: walk.cpp joshuaA.cpp seanF.cpp juanT.cpp drakeF.cpp lawrenceM.cpp Global.cpp
	g++ walk.cpp lawrenceM.cpp \
	joshuaA.cpp \
	seanF.cpp \
	juanT.cpp \
	drakeF.cpp \
	Global.cpp \
	-Wextra \
	libggfontsMac.a -Wall -owalk \
	-I/usr/X11R6/include \
	-L/usr/X11R6/lib -lX11 \
	-framework OpenGL \
	-framework Cocoa -lm -lGL \
	-framework OpenAl


clean:
	rm -f walk
	rm -f *.o
