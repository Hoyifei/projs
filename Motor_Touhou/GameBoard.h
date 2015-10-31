#ifndef MOTOR_GAMEBOARD
#define MOTOR_GAMEBOARD
#include "ScreenItem.h"
#include <math.h>
#include <stdio.h>
#include "defines.h"
enum EHeroStatus{HNormal=0,Shield,Bomb,Lastword,HDying};
enum EHeroAmmoStatus{HANormal=0,HADying};
enum EEnemyStatus{EUp=0,EDown,ELeft,Eright,Waiting,EGenerating,EDying};
enum EPowerUpType{sPower=0,mPower,lPower,fPower,PShield,Levelup,Clear,Shake};
enum BoardStatus{Nothing=0,Loading,LoadFinish,Gaming};//Maybe More
struct THero{
    float pos[2],hrad,arad,sspd,fspd,acce,mxsp;//relative pos&speed //hit radius//alert radius//acceleration//max camera speed
    int power,tshoot,tcool,dhit,dshoot,tbomb,tlast,dtime,stime,level;
    //power/shoot cooldown time/shoot cooldown counter/hit damage/shoot damage/bomb normaltime/lastword normaltime/dying time/
    //shield time/
    bool slow;
    TScreenItem *stra[2],*left[2],*righ[2],*alrt[2],*cent[2],*shie[2],*die[2];
    EHeroStatus stat;
};
struct THeroAmmo{
    int age;
    float pos[2];//relative pos&speed
    TScreenItem *ammo[2],*egen[2],*edie[2];
    EHeroAmmoStatus stat;
    THeroAmmo *next;
};
struct THeroBomb{
    float xpos;
    int age;
    TScreenItem *gen[2],*norm[2],*fade[2];
};
struct TPowerUp{
    EPowerUpType type;
    float pos[2];//relative pos
    TPowerUp *next;
    TScreenItem* cont[2];
};
struct TEnemy{//EnemyAircraft&EnemyAmmo
    int type,level,age,life,number,tgen,tdie;
    bool hard,shield;
    float radi,pos[2],vec[2],acc[2];//global pos
    TScreenItem *gen[2],*up[2],*down[2],*left[2],*right[2],*death[2];
    EEnemyStatus stat;
    TEnemy *next;
};
struct TCamera{
    float bottom,basspeed,ranspeed,actspeed,ranrange;//Global;
};
class TGameBoard{
public:
    char stagepath[300];
    void prep();
    TFrame framepool[100000],*aframe,lframe[100];
    //loading animation cannot be changed.
    TScreenItem ScrItemPool[100000],*ascritem;
    float bkimgsegement[30000];
    int bksegnow;
    GLFWmutex Locker;
    TScreenItem *EnemySkin[6][1500],*EnemyAmmoSkin[6][1500];
    TEnemy EnemyPool[5000],EnemyAmmoPool[100000];
    TEnemy *aEnemy,*aEnemyAmmo,*hEnemy,*hEnemyAmmo;
    THeroAmmo HeroAmmoPool[50],*hHeroAmmo,*aHeroAmmo,*mHeroAmmo;
    TPowerUp *aPowerUp,*hPowerUp,PowerUpPool[100000];
    THero Player;//MultiPlayer Indev
    THeroBomb HBomb,HLast;
    TCamera Camera;
    int cntShake,ageShake,updCache;
    TScreenItem* drawList;
    float rangeShake,origpos[2];
    void updateBoard();
    void drawBoard();
    THeroAmmo* getHeroAmmo();
    TEnemy* getEnemy();
    TEnemy* getEnemyAmmo();
    TPowerUp* getPowerUp();
    TScreenItem* background[2];
    TScreenItem loadinganim;
    TFrame lframes[50];
    TScreenItem* getScrItem();
    TFrame* getFrame();
    void UnloadGame();
    void GLFWCALL LoadStage();
    BoardStatus bStat;
    /*
        stage file format:
        //maybe camera information first
        //a line contains bgm path
        a line contains int bg:total numbers of background images,bg should be smaller than 30000
        then n line each contain float ed,string bgpath,means that when camera pos is larger than ed
        ,it should change backgroundimage.
        then a line:int se:numbers of enemy skins
        then se blocks contain enemyskin information
        then a line contain int sa:numbers of enemyammo skins
        then sa blocks contain enemyammoski infos

        each block contains 6 parts.each part begins witn a num n
        then n strings

        then line contain n:numbers of enemies,n<=5000
        then n lines each lines contains float imgwid imghei hitrad , int codenum int skin float xpos float ypos
        se&sa should be smaller than 1500
    */
};
#endif // MOTOR_GAMEBOARD

