#ifndef NTASK2_GAME_ITERATOR
#define NTASK2_GAME_ITERATOR
#include "Game.h"
#define T_Game struct Game_iterator
struct Company_iterator;
struct STList;
struct Achieve;
struct Game_iterator
{
    T_Game *next,*prev,*cnext,*cprev;
    struct Company_iterator *comp;
    struct Achieve* head;
    int order;
    struct Game content;
    struct STList *global;
};
void init_g(T_Game *tar);
struct Company_iterator* Company(T_Game *tar);
int isFirst_g(T_Game *tar);
int isLast_g(T_Game *tar);
int isFirst_company(T_Game *tar);
int isLast_company(T_Game *tar);
struct Achieve* Achieve_First(T_Game *tar);
struct Achieve* Achieve_Last(T_Game *tar);
T_Game* Next_g(T_Game *tar);
T_Game* Prev_g(T_Game *tar);
T_Game* Next_company(T_Game *tar);
T_Game* Prev_company(T_Game *tar);
#endif // NTASK2_GAME_ITERATOR
