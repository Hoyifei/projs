#ifndef NTASK2_COMPANY_ITERATOR
#define NTASK2_COMPANY_ITERATOR
#include "Company.h"
#define T_Comp struct Company_iterator
struct Game_iterator;
struct STList;
struct Company_iterator
{
    int order;
    struct Game_iterator *head;
    T_Comp *next,*prev;
    struct Company content;
    struct STList *global;
//--------------------------------------------------
};
struct Game_iterator* GameFirst_c(T_Comp* tar);
struct Game_iterator* GameLast_c(T_Comp* tar);
int isFirst_c(T_Comp* tar);
int isLast_c(T_Comp* tar);
T_Comp* Next_c(T_Comp* tar);
T_Comp* Prev_c(T_Comp* tar);
void init_c(T_Comp* tar);
#endif // NTASK2_COMPANY_ITERATOR
