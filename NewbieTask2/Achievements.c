#include "Achievements.h"
#include "Game_iterator.h"
int isFirst_a(T_Achi* tar)
{
    return(tar==tar->Game->head->next);
}
int isLast_a(T_Achi* tar)
{
    return(tar==tar->Game->head->prev);
}
T_Achi* Next_a(T_Achi* tar)
{
    if(isLast_a(tar)) return(0);
    else return(tar->next);
}
T_Achi* Prev_a(T_Achi* tar)
{
    if(isFirst_a(tar)) return(0);
    else return(tar->prev);
}
