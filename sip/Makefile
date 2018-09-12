PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/
CC=g++

all: unisip

unisip: uniSip.o Manager.o sipAccount.o sipBuddy.o sipCall.o UI.o
	$(CC) -o $@ uniSip.o Manager.o sipAccount.o sipBuddy.o sipCall.o  UI.o  -std=c++11   `pkg-config --cflags --libs libpjproject`

uniSip.o:uniSip.cpp

Manager.o:Manager.cpp

sipAccount.o:sipAccount.cpp

sipBuddy.o:sipBuddy.cpp

sipCall.o:sipCall.cpp

UI.o:UI.cpp

clean:
	rm -f *.o unisip
