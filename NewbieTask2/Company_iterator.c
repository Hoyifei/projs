#include "Company_iterator.h"
#include "Game_iterator.h"
#include "List.h"
Game_iterator* Company_iterator::gFirst(){
    if(head->next==head) return head;
    return(head->next);
}
Game_iterator* Company_iterator::gLast(){
    if(head->prev==head) return head;
    return(head->prev);
}
bool Company_iterator::isFirst(){
    return(this==global->CompanyFirst);
}
bool Company_iterator::isLast(){
    return(this==global->CompanyLast);
}
Company_iterator* Company_iterator::Next(){
    if(this->isLast) return(0);
    return(this->next);
}
Company_iterator* Company_iterator::Next(){
    if(this->isFirst) return(0);
    return(this->prev);
}
