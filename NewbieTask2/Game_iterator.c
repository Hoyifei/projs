#include "Game_iterator.h"
#include "Company_iterator.h"
#include "Game.h"
#include "List.h"
#include "Achievements.h"
void init_g(T_Game *tar)
{
    tar->head=GetAchieve(tar->global);
    tar->head->prev=tar->head->next=tar->head;
    tar->head->Game=tar;
}
struct Company_iterator* Company(T_Game *tar)
{
    return(tar->comp);
}
int isFirst_g(T_Game *tar)
{
    return(tar==GameFirst(tar->global));
}
int isLast_g(T_Game *tar)
{
    return(tar==GameLast(tar->global));
}
int isFirst_company(T_Game *tar)
{
    return(tar==GameFirst_c(tar->comp));
}
int isLast_company(T_Game *tar)
{
    return(tar==GameLast_c(tar->comp));
}
struct Achieve* Achieve_First(T_Game *tar)
{
    return(tar->head->next);
}
struct Achieve* Achieve_Last(T_Game *tar)
{
    return(tar->head->prev);
}
T_Game* Next_g(T_Game *tar)
{
    if(isLast_g(tar)) return(0);
    else return(tar->next);
}
T_Game* Prev_g(T_Game *tar)
{
    if(isFirst_g(tar)) return(0);
    else return(tar->prev);
}
T_Game* Next_company(T_Game *tar)
{
    if(isLast_company(tar)) return(0);
    else return(tar->cnext);
}
T_Game* Prev_company(T_Game *tar)
{
    if(isFirst_company(tar)) return(0);
    else return(tar->cprev);
}
