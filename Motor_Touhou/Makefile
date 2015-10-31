motor : main.o ScreenItem.o GameBoard.o defines.h
	g++ -L/usr/lib -o motor GameBoard.o main.o ScreenItem.o -lGL -lGLU -lglfw -lX11 -lpthread -lXxf86vm -lXrandr -lXi -ldl

main.o: ScreenItem.o GameBoard.o defines.h
	g++ -Wall -g -I/usr/include -c main.cpp -o main.o

ScreenItem.o: defines.h ScreenItem.h
	g++ -Wall -g -I/usr/include -c ScreenItem.cpp -o ScreenItem.o

GameBoard.o: ScreenItem.h ScreenItem.o defines.h
	g++ -Wall -g -I/usr/include -c GameBoard.cpp -o GameBoard.o

clean:
	rm *.o
