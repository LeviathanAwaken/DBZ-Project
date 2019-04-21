CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk

mac: lab1mac


walk: walk.cpp joshuaA.cpp seanF.cpp juanT.cpp drakeF.cpp lawrenceM.cpp
	g++ $(CFLAGS) walk.cpp joshuaA.cpp seanF.cpp juanT.cpp drakeF.cpp lawrenceM.cpp libggfonts.a -Wall -Wextra -std=c++11 -lssl -lcrypto $(LFLAGS) -owalk -DPROFILE

lab1mac: walk.cpp joshuaA.cpp seanF.cpp juanT.cpp drakeF.cpp lawrenceM.cpp
	g++ -DPROFILE walk.cpp lawrenceM.cpp \
	joshuaA.cpp \
	seanF.cpp \
	juanT.cpp \
	drakeF.cpp \
	-Wextra \
	libggfontsMac.a -Wall -owalk \
	-I/usr/X11R6/include \
	-L/usr/X11R6/lib -lX11 \
	-I/usr/local/opt/openssl/include \
	-L/usr/local/opt/openssl/lib -lssl -lcrypto \
	-framework OpenGL \
	-framework Cocoa -lm -lGL \
	


clean:
	rm -f walk
	rm -f *.o
