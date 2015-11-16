#ifndef NTASK2_ACHIEVEMENTS
#define NTASK2_ACHIEVEMENTS
class Game_iterator;
class Achieve{
public:
    Achieve *prev,*next;
    Game_iterator* Game;
    char Name[50],Description[1000];
    bool Got;
    bool isFirst();
    bool isLast();
    void GetAchieve();
    void ClearAchieve();
};
#endif // NTASK2_ACHIEVEMENTS

