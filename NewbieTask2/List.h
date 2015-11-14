#ifndef NTASK2_LIST
#define NTASK2_LIST
#include "util.h"
class Company_iterator;
class Game_iterator;
struct Company;
struct Game;
struct Achieve;
class STList{
public:
	 STList();
	 ~STList();
    Achieve *aachi;
    Company_iterator *acomp,*chead;
    Game_iterator *agame,*ghead;
    int caachi,cacomp,cagame;
    void createcomp(int num);
    void creategame(int num);
    void createachi(int num);
    void qsort_game(Game_iterator* l,Game_iterator* r,bool ( * cmp )(Game_iterator*,Game_iterator*));
    bool sort_by_gamename(Game_iterator *a,Game_iterator *b);
    bool sort_by_companyname(Company_iterator *a,Company_iterator *b);
    Achieve* GetAchieve();
    Company_iterator* GetCompIterator();
    Game_iterator* GetGameIterator();
	 void delAchieve(Achieve* tar);
    Achieve* DelAchieve(Achieve* tar);
	 void delCompany(Achieve* tar);
    Company_iterator* DelCompany(Company_iterator* tar);
	 void delGame(Game_iterator* tar);
    Game_iterator* DelGame(Game_iterator* tar);
//--------------------------------------------------------------------------
    Game_iterator* GameFirst();
    Game_iterator* GameLast();
    Company_iterator* CompanyFirst();
    Company_iterator* CompanyLast();
    void SortGame(Game_iterator* l,Game_iterator* r,bool ( * cmp )(Game_iterator*,Game_iterator*));
    //sort games by custom compare function
    void SortGame_Company(Game_iterator* l,Game_iterator* r,bool ( * cmp )(Game_iterator*,Game_iterator*));
    //sort games in the same company by custom compare function
    void SortCompany(Comapny_iterator* l,Company_iterator* r,bool ( * cmp )(Company_iterator*,Company_iterator*));
    //sort companies by custom compare function
    void InsertGame_p(Game_iterator *fr,Game_iterator *tar);
    //insert a game after fr by a pointer,with the same company as fr
    void InsertGame(Game_iterator *fr,Game content);
    //insert a game after fr by a description,with the same company as fr
    void InsertCompany_p(Company_iterator *fr,Company_iterator *tar);
    //insert a company after fr by a pointer
    void InsertCompany(Company_iterator *fr,Company content);
    //insert a company after fr by a description
    Game_iterator* FindGame(int flag,Game model,Game_iterator* head);
    //find and the return the first game after head
    //matched by the condition that is given by flag and model.
    //flag=binary(aabbccddee)
    //aa=00:do not compare name|01:compare name.|
    //02:compare name without wild-card|03:compare name without wild-card,success only completely match
    //bb=00:do not compare description|01|02|03
    //cc:00:ignore played hours|01:played hours more than model(including)|02:less than|03:equal to
    //dd:00:ignore total achievement counts|01|02|03
    //ee:00:ignore achieved achievement counts|01|02|03
    Game_iterator* FindGame_c(int flag,Game model,Game_iterator* head);
    //find game in the same company of head
    int CountGame(int flag,Game model);
    int CountGame_c(int flag,Game model,Company_iterator* comp);
    void CountGame_e(int flag,Game model);
    //count games matches the condition
    //only count games in the same company if use Count_c
    //count matched games in each company and write in company's matchedGames value
    Company_iterator* FindCompany(int flag,Company model,Company_iterator* head);
    //find company
    //flag=binary(aabbccdd)
    //aa:name bb:description cc:totGames dd:matchedGames
}
