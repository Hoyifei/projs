#include "Company_iterator.h"
#include "Game_iterator.h"
#include "List.h"
struct Game_iterator* GameFirst_c(T_Comp* tar)
{
    return(tar->head->cnext);
}
struct Game_iterator* GameLast_c(T_Comp* tar)
{
    return(tar->head->cprev);
}
int isFirst_c(T_Comp* tar)
{
    return(tar==CompanyFirst(tar->global));
}
int isLast_c(T_Comp* tar)
{
    return(tar==CompanyLast(tar->global));
}
T_Comp* Next_c(T_Comp* tar)
{
    if(isLast_c(tar)) return(0);
    return(tar->next);
}
T_Comp* Prev_c(T_Comp* tar)
{
    if(isFirst_c(tar)) return(0);
    return(tar->prev);
}
void init_c(T_Comp* tar)
{
    tar->head=GetGameIterator(tar->global);
    tar->head->prev=tar->head->next=tar->head->cprev=tar->head->cnext=tar->head;
    tar->head->comp=tar;
}
