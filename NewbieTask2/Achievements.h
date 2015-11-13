#ifndef NTASK2_ACHIEVEMENTS
#define NTASK2_ACHIEVEMENTS
struct Achieve{
    Achieve *prev,*next;
    char name[50],description[1000];
    bool got;
};
#endif // NTASK2_ACHIEVEMENTS

