#ifndef NTASK2_COMPANY_ITERATOR
#define NTASK2_COMPANY_ITERATOR
#include "Company.h"
class Game_iterator;
class STList;
class Company_iterator{
public:
    int order;
    Game_iterator *head;
    Company_iterator *next,*prev;
    Company content;
    STList *global;
//--------------------------------------------------
	 Company_iterator();
	 ~Company_iterator();
    Game_iterator* GameFirst();
    Game_iterator* GameLast();
    Company_iterator* Next();
    Company_iterator* Prev();
    bool isLast();
    bool isFirst();
};
#endif // NTASK2_COMPANY_ITERATOR
