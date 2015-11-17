libSteamList_HYF.a : Company_iterator.o Game_iterator.o List.o util.o Achievements.o
	ar cr libSteamList_HYF.a Company_iterator.o Game_iterator.o List.o util.o Achievements.o
Achievements.o : Achievements.h Achievements.c Game_iterator.h Game.h
	gcc -c Achievements.c -o Achievements.o
Company_iterator.o : Company.h Company_iterator.c Company_iterator.h Game_iterator.h List.h
	gcc -c Company_iterator.c -o Company_iterator.o
Game_iterator.o : Game.h Achievements.h Game_iterator.h Game_iterator.c List.h Company_iterator.h
	gcc -c Game_iterator.c -o Game_iterator.o
List.o : List.h List.c Game_iterator.h Company_iterator.h util.o
	gcc -c List.c -o List.o
util.o : util.h util.c
	gcc -c util.c -o util.o
test : libSteamList_HYF.a main.c
	gcc -L./ -o test main.c -lSteamList_HYF
	./test
clean :
	rm *.o

