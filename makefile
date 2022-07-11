LIBS=-lm -LInterval -lstdc++ -ldl -lmpfr -Lexprtk -Lfunkcje -LWindow
CC=g++
GTKLIBS=`pkg-config gtkmm-3.0 --cflags --libs`
main: main.cpp
	$(CC) -o main -g main.cpp Window.o diag_int.o sym_int.o zwykla.o $(LIBS) $(GTKLIBS)

window: Window.cpp
	$(CC) -c -g Window.cpp $(GTKLIBS)

sym: sym_int.cpp
	$(CC) -c -g sym_int.cpp $(LIBS)
diag: diag_int.cpp
	$(CC) -c -g diag_int.cpp $(LIBS)
zwykla: zwykla.cpp
	$(CC) -c -g zwykla.cpp $(LIBS)

clean:
	@if [ -f Window.o ]; then rm Window.o; fi
	@if [ -f main ]; then rm main; fi
all:
	make clean
	make window
	make sym
	make diag
	make zwykla
	make main
	./main
run:
	./main