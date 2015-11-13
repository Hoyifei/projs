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
    Game_iterator* gFirst();
    Game_iterator* gLast();
    Company_iterator* Next();
    Company_iterator* Prev();
    bool isLast();
    bool isFirst();
};
#endif // NTASK2_COMPANY_ITERATOR
