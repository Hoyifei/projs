#ifndef NTASK2_UTIL
#define NTASK2_UTIL
#define ALLOCATE_UNITS 314
//--------defines of memory allocating
#define GAME_IGNORENAME (0)
#define GAME_COMPARENAME (1<<8)
#define GAME_COMPARENAME_NOWILDCARD (1<<9)
#define GAME_COMPARENAME_COMPLETELYMATCH (3<<8)
#define GAME_IGNOREDESC (0)
#define GAME_COMPAREDESC (1<<6)
#define GAME_COMPAREDESC_NOWILDCARD (1<<7)
#define GAME_COMPAREDESC_COMPLETELYMATCH (3<<6)
#define GAME_IGNOREHOUR (0)
#define GAME_HOUR_MORE (1<<4)
#define GAME_HOUR_LESS (1<<5)
#define GAME_HOUR_EQUAL (3<<4)
#define GAME_IGNORETOTACHIEVE (0)
#define GAME_TOTACHIEVE_MORE (1<<2)
#define GAME_TOTACHIEVE_LESS (1<<3)
#define GAME_TOTACHIEVE_EQUAL (3<<2)
#define GAME_IGNOREMYACHIEVE (0)
#define GAME_MYACHIEVE_MORE (1)
#define GAME_MYACHIEVE_LESS (2)
#define GAME_MYACHIEVE_EQUAL (3)
#define COMP_IGNORENAME (0)
#define COMP_COMPARENAME (1<<6)
#define COMP_COMPARENAME_NOWILDCARD (1<<7)
#define COMP_COMPARENAME_COMPLETELYMATCH (3<<6)
#define COMP_IGNOREDESC (0)
#define COMP_COMPAREDESC (1<<4)
#define COMP_COMPAREDESC_NOWILDCARD (1<<5)
#define COMP_COMPAREDESC_COMPLETELYMATCH (3<<4)
#define COMP_IGNORETOTGAME (0)
#define COMP_TOTGAME_MORE (1<<2)
#define COMP_TOTGAME_LESS (1<<3)
#define COMP_TOTGAME_EQUAL (3<<2)
#define COMP_IGNOREMATCHEDGAME (0)
#define COMP_MATCHEDGAME_MORE (1)
#define COMP_MATCHEDGAME_LESS (2)
#define COMP_MATCHEDGAME_EQUAL (3)
//----------defines of matching flags--------------------
//string compare with wildcard
//if origin includs target,return 1
class Util_Funcs{
public:
    bool compstr_WC(const char*,const char*);
    bool compstr(const char*,const char*);
    void givestr(char*,const char*);
};
#endif // NTASK2_UTIL

