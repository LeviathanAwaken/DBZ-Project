CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: finalform

mac: lab1mac


finalform: finalform.cpp joshuaA.cpp seanF.cpp juanT.cpp drakeF.cpp lawrenceM.cpp
	g++ $(CFLAGS) finalform.cpp joshuaA.cpp seanF.cpp juanT.cpp drakeF.cpp lawrenceM.cpp libggfonts.a -Wall -Wextra -std=c++11 -lssl -lcrypto $(LFLAGS) -o finalform -DPROFILE

lab1mac: finalform.cpp joshuaA.cpp seanF.cpp juanT.cpp drakeF.cpp lawrenceM.cpp
	g++ -DPROFILE finalform.cpp lawrenceM.cpp \
	joshuaA.cpp \
	seanF.cpp \
	juanT.cpp \
	drakeF.cpp \
	-Wextra \
	-std=c++11 \
	libggfontsMac.a -Wall -ofinalform \
	-I/usr/X11R6/include \
	-L/usr/X11R6/lib -lX11 \
	-I/usr/local/opt/openssl/include \
	-L/usr/local/opt/openssl/lib -lssl -lcrypto \
	-framework OpenGL \
	-framework Cocoa -lm -lGL \



clean:
	rm -f finalform
	rm -f *.o
