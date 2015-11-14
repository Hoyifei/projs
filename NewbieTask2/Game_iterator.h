#ifndef NTASK2_GAME_ITERATOR
#define NTASK2_GAME_ITERATOR
#include "Game.h"
class Company_iterator;
class STList;
class Game_iterator{
public:
    Game_iterator *next,*prev,*cnext,*cprev;
    Company_iterator *comp;
    int order;
    Game content;
    STList *global;
//---------------------------------------------------------------
	 Game_iterator();
	 ~Game_iterator();
    Game_iterator* Next();
    Game_iterator* Prev();
    Game_iterator* Next_company();
    Game_iterator* Prev_company();
    Company_iterator* Company();
    bool isLast();
    bool isFirst();
    bool isLast_company();
    bool isFirst_company();
    void addAchievep(Achieve *val);
    void addAchievev(const char* name,const char* description);
};
#endif // NTASK2_GAME_ITERATOR
