#include "Achievements.h"
#include "Game_iterator.h"
void Achieve::GetAchieve(){
    Got=1;
    ++Game->content.MyAchieve;
}
void Achieve::ClearAchieve(){
    Got=0;
    --Game->content.MyAchieve;
}
bool Achieve::isFirst(){
    return(this==Game->head->next);
}
bool Achieve::isLast(){
    return(this==Game->head->prev);
}
