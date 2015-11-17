#ifndef NTASK2_LIST
#define NTASK2_LIST
#include "Company.h"
#include "Game.h"
#define T_List struct STList
struct Company_iterator;
struct Game_iterator;
struct Achieve;
struct STList
{
    struct Achieve *aachi;
    struct Company_iterator *acomp,*chead;
    struct Game_iterator *agame,*ghead;
    int caachi,cacomp,cagame;
};
void init(T_List* source);
void clean_up(T_List* source);
void createcomp(int num,T_List* source);
void creategame(int num,T_List* source);
void createachi(int num,T_List* source);
struct Achieve* GetAchieve(T_List* source);
struct Company_iterator* GetCompIterator(T_List* source);
struct Game_iterator* GetGameIterator(T_List* source);
void deleteAchieve(struct Achieve* tar);
struct Achieve* DelAchieve(struct Achieve* tar);
void deleteCompany(struct Company_iterator* tar);
struct Company_iterator* DelCompany(struct Company_iterator* tar);
void deleteGame(struct Game_iterator* tar);
struct Game_iterator* DelGame(struct Game_iterator* tar);
struct Game_iterator* GameFirst(T_List* source);
struct Game_iterator* GameLast(T_List* source);
struct Company_iterator* CompanyFirst(T_List* source);
struct Company_iterator* CompanyLast(T_List* source);
int CountGame(int flag,struct Game model,T_List* source);
void CountGame_e(int flag,struct Game model,T_List* source);
//count games matches the condition
//count matched games in each company and write in company's matchedGames value
struct Company_iterator* FindCompany(int flag,struct Company model,struct Company_iterator* head);
//find company
//flag=binary(aabbccdd)
//aa:name bb:description cc:totGames dd:matchedGames
struct Game_iterator* FindGame(int flag,struct Game model,struct Game_iterator* head);
//find and the return the first game after head
//matched by the condition that is given by flag and model.
//flag=binary(aabbccddee)
//aa=00:do not compare name|01:compare name.|
//02:compare name without wild-card|03:compare name without wild-card,success only completely match
//bb=00:do not compare description|01|02|03
//cc:00:ignore played hours|01:played hours more than model(including)|02:less than|03:equal to
//dd:00:ignore total achievement counts|01|02|03
//ee:00:ignore achieved achievement counts|01|02|03
struct Game_iterator* FindGame_c(int flag,struct Game model,struct Game_iterator* head);
//find game in the same company of head
void addAchievep(struct Achieve *val,struct Game_iterator *tar);
//add Achieve val to game tar
struct Achieve* addAchievev(const char* name,const char* description,struct Game_iterator *tar);
//add Achieve as name&desc describes to tar,
void Get_a(struct Achieve* tar);
// set achieve as got one
void Clear_a(struct Achieve* tar);
// set achieve as not got
int CountGame_c(int flag,struct Game model,struct Company_iterator* comp);
//count games in the same company if use Count_c
void InsertGame_p(struct Game_iterator *fr,struct Game_iterator *tar);
//insert a game after fr by a pointer,with the same company as fr
struct Game_iterator* InsertGame(struct Game_iterator *fr,struct Game content);
//insert a game after fr by a description,with the same company as fr
void InsertCompany_p(struct Company_iterator *fr,struct Company_iterator *tar);
//insert a company after fr by a pointer
struct Company_iterator* InsertCompany(struct Company_iterator *fr,struct Company content);
//insert a company after fr by a description
void SortGame(struct Game_iterator* l,struct Game_iterator* r,int ( * cmp )(struct Game_iterator*,struct Game_iterator*));
//sort games by custom compare function
void SortGame_Company(struct Game_iterator* l,struct Game_iterator* r,int ( * cmp )(struct Game_iterator*,struct Game_iterator*));
//sort games in the same company by custom compare function
void SortCompany(struct Company_iterator* l,struct Company_iterator* r,int ( * cmp )(struct Company_iterator*,struct Company_iterator*));
//sort companies by custom compare function
void qsort_game(struct Game_iterator* l,struct Game_iterator* r,int ( * cmp )(struct Game_iterator*,struct Game_iterator*));
void qsort_game_company(struct Game_iterator* l,struct Game_iterator* r,int ( * cmp )(struct Game_iterator*,struct Game_iterator*));
void qsort_company(struct Company_iterator* l,struct Company_iterator* r,int ( * cmp )(struct Company_iterator*,struct Company_iterator*));
#endif
