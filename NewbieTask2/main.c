#include <stdio.h>
#include "SteamList_HYF.h"
#include <string.h>
T_List fuck;
int sort_by_companyname(T_Comp *a,T_Comp *b)
{
    return(strcmp(a->content.Name,b->content.Name)<0);
}
int sort_by_gamename(T_Game *a,T_Game *b)
{
    return(strcmp(a->content.Name,b->content.Name)<0);
}
int sort_by_myachieve(T_Game *a,T_Game *b)
{
    return(a->content.MyAchieve>b->content.MyAchieve);
}
int sort_by_matched(T_Comp *a,T_Comp *b)
{
    return(a->content.MatchedGame>b->content.MatchedGame);
}
int main()
{
    init(&fuck);
    char* ch;
    T_game gm;T_comp cp;T_Achi ac;
    int i,j,k;
    for(i=1;i<=100;++i){
        sprintf(cp.Name,"%d",i);
        sprintf(cp.Description,"%d",i);
        T_Comp *ci=InsertCompany(CompanyFirst(&fuck),cp);
        for(j=1;j<=100;++j){
            sprintf(gm.Name,"%d,%d",i,j);
            sprintf(gm.Description,"%d,%d",j,i);
            T_Game *gi=InsertGame(GameFirst_c(ci),gm);
            for(k=1;k<=100;++k){
                sprintf(ac.Name,"%d,%d,%d",i,j,k);
                sprintf(ac.Description,"%d,%d,%d",k,j,i);
                addAchievev(ac.Name,ac.Description,gi);
            }
        }
        printf("Insert Company %d\n",i);
    }
    printf("aaa\n");
    for(i=1;i<=50;++i) DelCompany(CompanyLast(&fuck));
    clean_up(&fuck);
    printf("Program exited with no error\n");
    return(0);
}
