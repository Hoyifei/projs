libSteamList_HYF.a : Company_iterator.o Game_iterator.o List.o util.o Achievements.o
	ar cr libSteamList_HYF.a obj/Company_iterator.o obj/Game_iterator.o obj/List.o obj/util.o obj/Achievements.o
Achievements.o : Achievements.h Achievements.cpp Game_iterator.h Game.h
	g++ -c Achievements.cpp -o obj/Achievements.o
Company_iterator.o : Company.h Company_iterator.cpp Company_iterator.h Game_iterator.h List.h
	g++ -c Company_iterator.cpp -o obj/Company_iterator.o
Game_iterator.o : Game.h Achievements.h Game_iterator.h Game_iterator.cpp List.h Company_iterator.h
	g++ -c Game_iterator.cpp -o obj/Game_iterator.o
List.o : List.h List.cpp Game_iterator.h Company_iterator.h util.o
	g++ -c List.cpp -o obj/List.o
util.o : util.h util.cpp
	g++ -c util.cpp -o obj/util.o
clean :
	rm obj/* 
	rm libSteamList_HYF.a
