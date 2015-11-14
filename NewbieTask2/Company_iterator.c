#include "Company_iterator.h"
#include "Game_iterator.h"
#include "List.h"
Company_iterator::Company_iterator(){
    head=global->GetGameIterator();
    head->prev=head->next=head->cprev=head->cnext=head;
}
Company_iteator::~Company_iterator(){
    
}
Game_iterator* Company_iterator::GameFirst(){
    return(head->next);
}
Game_iterator* Company_iterator::GameLast(){
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
Company_iterator* Company_iterator::Prev(){
    if(this->isFirst) return(0);
    return(this->prev);
}
