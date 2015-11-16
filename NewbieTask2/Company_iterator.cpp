#include "Company_iterator.h"
#include "Game_iterator.h"
#include "List.h"
void Company_iterator::init(){
    head=global->GetGameIterator();
    head->prev=head->next=head->cprev=head->cnext=head;
    head->comp=this;
    //printf("%d's head=%d,its head=%d\n",this,head,head->content.Head);
}
void Company_iterator::unins(){

}
Game_iterator* Company_iterator::GameFirst(){
    //printf("this=%d Head=%d First=%d Head'sCompany=%d\n",this,head,head->next,head->comp);
    return(head->cnext);
}
Game_iterator* Company_iterator::GameLast(){
    return(head->cprev);
}
bool Company_iterator::isFirst(){
    return(this==global->CompanyFirst());
}
bool Company_iterator::isLast(){
    return(this==global->CompanyLast());
}
Company_iterator* Company_iterator::Next(){
    if(this->isLast()) return(0);
    return(this->next);
}
Company_iterator* Company_iterator::Prev(){
    if(this->isFirst()) return(0);
    return(this->prev);
}
