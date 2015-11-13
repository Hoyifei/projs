#include "Game_iterator.h"
#include "Company_iterator.h"
#include "List.h"
Company_iterator* Game_iterator::Company(){
    return(this.comp);
}
bool Game_iterator::isFirst(){
    return(this==global.GameFirst());
}
bool Game_iterator::isLast(){
    return(this==global.GameLast());
}
bool Game_iterator::isFirst_company(){
    return(this==comp->gFirst());
}
bool Game_iterator::isLast()_company(){
    return(this==comp->gLast());
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
    Achieve* last=this->content.Head->prev;
    last->next=val;
    val->prev=last;
    val->next=this->content.Head;
    this.content.Head->prev=val;
}
void Game_iterator::addAchievev(const char* name,const char* description){
    Achieve* val=global->GetAchieve();
    val->got=0;
    int i;
    for(i=0;name[i];++i) val->name[i]=name[i];
    val->name[i]=0;
    for(i=0;description[i];++i) val->description[i]=description[i];
    val->description[i]=0;
    addAchievep(val);
}
