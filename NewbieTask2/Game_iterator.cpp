#include "Game_iterator.h"
#include "Company_iterator.h"
#include "Game.h"
#include "List.h"
Company_iterator* Game_iterator::Company(){
    return(this->comp);
}
bool Game_iterator::isFirst(){
    return(this==global->GameFirst());
}
bool Game_iterator::isLast(){
    return(this==global->GameLast());
}
bool Game_iterator::isFirst_company(){
    return(this==comp->GameFirst());
}
bool Game_iterator::isLast_company(){
    return(this==comp->GameLast());
}
Achieve* Game_iterator::Achieve_First(){
    return(this->head->next);
}
Achieve* Game_iterator::Achieve_Last(){
    return(this->head->prev);
}
Game_iterator* Game_iterator::Next(){
    if(this->isLast()) return(0);else return(this->next);
}
Game_iterator* Game_iterator::Prev(){
    if(this->isFirst()) return(0);else return(this->prev);
}
Game_iterator* Game_iterator::Next_company(){
    if(this->isLast_company()) return(0);else return(this->cnext);
}
Game_iterator* Game_iterator::Prev_company(){
    if(this->isFirst_company()) return(0);else return(this->cprev);
}
void Game_iterator::addAchievep(Achieve* val){
    Achieve* last=this->head->prev;
    last->next=val;
    val->prev=last;
    val->next=this->head;
    this->head->prev=val;
    ++content.TotAchieve;
    val->Game=this;
}
Achieve* Game_iterator::addAchievev(const char* name,const char* description){
    Achieve* val=global->GetAchieve();
    val->Got=0;
    int i;
    for(i=0;name[i];++i) val->Name[i]=name[i];
    val->Name[i]=0;
    for(i=0;description[i];++i) val->Description[i]=description[i];
    val->Description[i]=0;
    addAchievep(val);
    return(val);
}
void Game_iterator::init(){
    head=global->GetAchieve();
    head->prev=head->next=head;
    //printf("aG%d's head=%d,head.next=%d\n",this,content.Head,content.Head->next);
}
void Game_iterator::unins(){

}
