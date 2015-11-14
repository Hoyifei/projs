#ifndef NTASK2_GAME
#define NTASK2_GAME
#include "Achievements.h"
struct Game{
    char Name[50],Description[1000];
    int Hours,TotalAchieve,MyAchieve;
    Achieve *Head;
};
#endif // NTASK2_GAME
