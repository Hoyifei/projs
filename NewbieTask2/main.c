#include <stdio.h>
#include "SteamList_HYF.h"
#include <string.h>
T_List fuck;
T_comp SHAlice,Mojang,Microsoft;
T_game Touhou6,Touhou7,Touhou8,Minecraft,Minesweeper,Klondlike;
T_Comp *SHA,*MOJ,*MIC;
T_Game *TH6,*TH7,*TH8,*MC,*MS,*KL;
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
    givestr(SHAlice.Name,"Shanghai Alice");
    givestr(SHAlice.Description,"A game-making group lead by ZUN");
    givestr(Mojang.Name,"Mojang");
    givestr(Mojang.Description,"Minecraft's creator");
    givestr(Microsoft.Name,"Microsoft");
    givestr(Microsoft.Description,"The well-known company");
    givestr(Touhou6.Name,"Touhou project 6");
    givestr(Touhou6.Description,"Embodiment of Scarlet Devil");
    givestr(Touhou7.Name,"Touhou project 7");
    givestr(Touhou7.Description,"Perfect Cherry Blossom");
    givestr(Touhou8.Name,"Touhou project 8");
    givestr(Touhou8.Description,"Impershable Night");
    givestr(Minecraft.Name,"Minecraft");
    givestr(Minecraft.Description,"Version 1.8.1");
    givestr(Minesweeper.Name,"Minesweeper");
    givestr(Minesweeper.Description,"XP Version");
    givestr(Klondlike.Name,"Klondlike");
    givestr(Klondlike.Description,"Warning:Dangerous Game,Be careful.");
    SHA=InsertCompany(CompanyFirst(&fuck),SHAlice);
    MOJ=InsertCompany(SHA,Mojang);
    MIC=InsertCompany(MOJ,Microsoft);
    TH8=InsertGame(GameFirst_c(SHA),Touhou8);
    TH7=InsertGame(TH8,Touhou7);
    TH6=InsertGame(TH7,Touhou6);
    MC=InsertGame(GameFirst_c(MOJ),Minecraft);
    MS=InsertGame(GameFirst_c(MIC),Minesweeper);
    KL=InsertGame(MS,Klondlike);
    T_Achi* Lunatic=addAchievev("Lunatic","Cleared the Lunatic Level",TH8);
    addAchievev("Lastword Collector","Collected All Lastword",TH8);
    Get_a(Lunatic);
    T_Game* now;
    for(now=GameFirst(&fuck); !isLast_g(now); now=Next_g(now))
    {
        printf("Game:%s\nCompany:%s\nDescription:%s\n",now->content.Name,Company(now)->content.Name,now->content.Description);
    }
    printf("Game:%s\nCompany:%s\nDescription:%s\n",now->content.Name,Company(now)->content.Name,now->content.Description);
    SortGame(GameFirst(&fuck),GameLast(&fuck),sort_by_myachieve);
    for(now=GameFirst(&fuck); now; now=Next_g(now))
    {
        printf("%s->",now->content.Name);
    }
    T_game modal;
    givestr(modal.Name,"M*e");
    CountGame_e(GAME_COMPARENAME,modal,&fuck);
    printf("Count Done!\n");
    SortCompany(CompanyFirst(&fuck),CompanyLast(&fuck),sort_by_matched);
    T_Comp *cn;
    for(cn=CompanyFirst(&fuck); cn; cn=Next_c(cn))
        printf("%s\n",cn->content.Name);
    T_comp modcl;modcl.TotalGame=1;
    cn=FindCompany(COMP_TOTGAME_EQUAL,modcl,CompanyFirst(&fuck));
    printf("%s\n",cn->content.Name);
    clean_up(&fuck);
    printf("Program exited with no error\n");
    return(0);
}
