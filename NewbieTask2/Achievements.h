#ifndef NTASK2_ACHIEVEMENTS
#define NTASK2_ACHIEVEMENTS
#define T_Achi struct Achieve
struct Game_iterator;
struct Achieve
{
    T_Achi *prev,*next;
    struct Game_iterator* Game;
    char Name[50],Description[1000];
    int Got;
};
int isFirst_a(T_Achi *tar);
int isLast_a(T_Achi *tar);
T_Achi *Next_a(T_Achi *tar);
T_Achi *Prev_a(T_Achi *tar);
#endif // NTASK2_ACHIEVEMENTS

