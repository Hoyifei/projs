#ifndef NTASK2_GAME_ITERATOR
#define NTASK2_GAME_ITERATOR
#include "Game.h"
class Company_iterator;
class STList;
class Achieve;
class Game_iterator{
public:
    Game_iterator *next,*prev,*cnext,*cprev;
    Company_iterator *comp;
    Achieve* head;
    int order;
    Game content;
    STList *global;
//---------------------------------------------------------------
	void init();
	void unins();
    Game_iterator* Next();
    Game_iterator* Prev();
    Game_iterator* Next_company();
    Game_iterator* Prev_company();
    Achieve* Achieve_First();
    Achieve* Achieve_Last();
    Company_iterator* Company();
    bool isLast();
    bool isFirst();
    bool isLast_company();
    bool isFirst_company();
    void addAchievep(Achieve *val);
    Achieve* addAchievev(const char* name,const char* description);
};
#endif // NTASK2_GAME_ITERATOR
