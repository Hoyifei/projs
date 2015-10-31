#include "GameBoard.h"
THeroAmmo* TGameBoard::getHeroAmmo(){
    if(aHeroAmmo){
        THeroAmmo* ans=aHeroAmmo;
        aHeroAmmo=aHeroAmmo->next;
        return(ans);
    }else return(0);
}
TScreenItem* TGameBoard::getScrItem(){
    if(ascritem){
        TScreenItem* ans=ascritem;
        ascritem=ascritem->Next;
        return(ans);
    }else {printf("getScrItem failed\n");return(0);}
}
TFrame* TGameBoard::getFrame(){
    if(aframe){
        TFrame* ans=aframe;
        aframe=aframe->next;
        return(ans);
    }else return(0);
}
TEnemy* TGameBoard::getEnemyAmmo(){
    if(aEnemyAmmo){
        TEnemy* ans=aEnemyAmmo;
        aEnemyAmmo=aEnemyAmmo->next;
        return(ans);
    }else return(0);
}
TEnemy* TGameBoard::getEnemy(){
    if(aEnemy){
        TEnemy* ans=aEnemy;
        aEnemy=aEnemy->next;
        return(ans);
    }else return(0);
}
TPowerUp* TGameBoard::getPowerUp(){
    if(aPowerUp){
        TPowerUp* ans=aPowerUp;
        aPowerUp=aPowerUp->next;
        return(ans);
    }else return(0);
}
void TGameBoard::updateBoard(){
    printf("Update begin\n");
    float hxmove=0,hymove=0;
    //background image update
    while(bkimgsegement[bksegnow]<Camera.bottom){
        ++bksegnow;background[0]->nextframe();background[1]->nextframe();
    }
    printf("BackgroundUpdate end\n");
    //printf("123/0=%d\n",123/(456-456));
    //Hero update
    if(Player.stat==Shield){
            --Player.stime;
            if(Player.stime==0) Player.stat=HNormal;
    }
    if(Player.stat!=HDying){
        if(glfwGetKey(GLFW_KEY_LSHIFT)==GLFW_PRESS||glfwGetKey(GLFW_KEY_RSHIFT)==GLFW_PRESS) Player.slow=1;else Player.slow=0;
        if(glfwGetKey(GLFW_KEY_UP)) hymove+=1;
        if(glfwGetKey(GLFW_KEY_DOWN)) hymove-=1;
        if(glfwGetKey(GLFW_KEY_LEFT)) hxmove-=1;
        if(glfwGetKey(GLFW_KEY_RIGHT)) hxmove+=1;
        hxmove*=(Player.slow?Player.sspd:Player.fspd);hymove*=(Player.slow?Player.sspd:Player.fspd);
        if(abs(hxmove)<eps&&abs(hymove)<eps) {hxmove*=(0.5*gener);hymove*=(0.5*gener);}
        Player.pos[0]+=hxmove;Player.pos[1]+=hymove;
        if(Player.pos[0]<0) Player.pos[0]=0;if(Player.pos[1]<0) Player.pos[1]=0;
        if(Player.pos[0]>CAMERA_WIDTH) Player.pos[0]=CAMERA_WIDTH;
        if(Player.pos[1]>CAMERA_HEIGHT) Player.pos[1]=CAMERA_HEIGHT;
        if(Player.tcool>0) --Player.tcool;
        if(glfwGetKey('Z')==GLFW_PRESS&&Player.tcool<=0){
                Player.tcool=Player.tshoot;
                THeroAmmo* newHAmmo=getHeroAmmo();
                if(newHAmmo){
                    newHAmmo->next=hHeroAmmo;
                    hHeroAmmo=newHAmmo;
                    //copy module
                    newHAmmo->pos[0]=Player.pos[0];
                    newHAmmo->pos[1]=Player.pos[1];
                    newHAmmo->stat=HANormal;
                    newHAmmo->age=0;
                    newHAmmo->egen[0]=getScrItem();
                    newHAmmo->egen[0]->copyitem(mHeroAmmo->egen[0]);
                    newHAmmo->edie[0]=getScrItem();
                    newHAmmo->edie[0]->copyitem(mHeroAmmo->edie[0]);
                    newHAmmo->ammo[0]=getScrItem();
                    newHAmmo->ammo[0]->copyitem(mHeroAmmo->ammo[0]);
                    newHAmmo->egen[1]=getScrItem();
                    newHAmmo->egen[1]->copyitem(mHeroAmmo->egen[0]);
                    newHAmmo->edie[1]=getScrItem();
                    newHAmmo->edie[1]->copyitem(mHeroAmmo->edie[0]);
                    newHAmmo->ammo[1]=getScrItem();
                    newHAmmo->ammo[1]->copyitem(mHeroAmmo->ammo[0]);
                }
        }
        if(glfwGetKey('X')==GLFW_PRESS){
            if(Player.stat==HNormal||Player.stat==Shield){
                if(Player.power>=150){
                    Player.stat=Bomb;
                    HBomb.age=0;
                    HBomb.xpos=Player.pos[0];
                    Player.power-=150;
                }
            }
            if(Player.stat==Bomb&&HBomb.age>=HEROBOMB_GENTIME&&Player.power>=200){
                Player.stat=Lastword;
                Player.power-=200;
                HBomb.age=-1;
                HLast.age=0;
                HLast.xpos=200;
            }
        }
    }else{
        --Player.dtime;
        if(Player.dtime==0){
            Player.pos[0]=200;
            Player.pos[1]=100;
            Player.stat=Shield;
            Player.stime=1200;
        }
    }
    printf("PlayerUpdate end\n");
    //printf("123/0=%d\n",123/(456-456));
    //HeroAmmo update
    THeroAmmo* HAlast=0;
    for(THeroAmmo* now=hHeroAmmo;now;){
        ++now->age;
        if(now->stat==HADying){
            if(now->age>=HEROAMMO_FADETIME){
                if(HAlast){
                    HAlast->next=now->next;
                    now->next=aHeroAmmo;
                    aHeroAmmo=now;
                    now=HAlast->next;
                }else{
                    hHeroAmmo=now->next;
                    now->next=aHeroAmmo;
                    aHeroAmmo=now;
                    now=hHeroAmmo;
                }
            }
        }else{
            now->pos[1]+=HEROAMMO_SPEED;
            HAlast=now;
            now=now->next;
        }
    }
    printf("HAmmoUpdate end\n");
    //printf("123/0=%d\n",123/(456-456));
    //Enemy update
    TEnemy* Elast=0;
    for(TEnemy* now=hEnemy;now;){
        if(now->stat==Waiting) {now=now->next;continue;}
        ++now->age;
        if(now->stat==EDying){
            if(now->age>=now->tdie){
                if(Elast){
                    Elast->next=now->next;
                    now->next=aEnemy;
                    aEnemy=now;
                    now=Elast->next;
                }else{
                    hEnemy=now->next;
                    now->next=aEnemy;
                    aEnemy=now;
                    now=hEnemy;
                }
            }else{
                Elast=now;now=now->next;
            }
            continue;
        }
        switch(now->type){
            /*case 0://straight with acceleration
                now->pos[0]+=now->vec[0];now->pos[1]+=now->vec[1];
                now->vec[0]+=now->acc[0];now->vec[1]+=now->acc[1];
                break;*/
            default://Appear and stop without action
                break;
        }
        Elast=now;
        now=now->next;
    }
    printf("EnemyUpdate end\n");
    //printf("123/0=%d\n",123/(456-456));
    //EnemyAmmo update
    Elast=0;
    for(TEnemy* now=hEnemyAmmo;now;){
        ++now->age;
        if(now->stat==EDying){
            if(now->age>=now->tdie){
                if(Elast){
                    Elast->next=now->next;
                    now->next=aEnemyAmmo;
                    aEnemyAmmo=now;
                    now=Elast->next;
                }else{
                    hEnemyAmmo=now->next;
                    now->next=aEnemyAmmo;
                    aEnemyAmmo=now;
                    now=hEnemyAmmo;
                }
            }else{
                Elast=now;now=now->next;
            }
            continue;
        }
        switch(now->type){
            /*case 0://straight with acceleration
                now->pos[0]+=now->vec[0];now->pos[1]+=now->vec[1];
                now->vec[0]+=now->acc[0];now->vec[1]+=now->acc[1];
                break;*/
            default://Appear and stop without action
                break;
        }
        Elast=now;
        now=now->next;
    }
    //PowerUp update
    TPowerUp* Plast=0;
    for(TPowerUp* now=hPowerUp;now;){
        if(now->pos[1]>-CAMERA_THICK){
            now->pos[1]-=POWERUP_FALLSPEED;
            Plast=now;
            now=now->next;
        }else{
            if(Plast){
                Plast->next=now->next;
                now->next=aPowerUp;
                aPowerUp=now;
                now=Plast->next;
            }else{
                hPowerUp=now->next;
                now->next=aPowerUp;
                aPowerUp=now;
                now=hPowerUp;
            }
        }
    }
    //Hittest
    Camera.bottom+=Camera.actspeed;
    if(Camera.actspeed<Camera.basspeed+Camera.ranspeed) Camera.actspeed+=0.05;else Camera.actspeed-=0.05;
    for(THeroAmmo* anow=hHeroAmmo;anow;anow=anow->next){
        if(anow->pos[0]<-CAMERA_THICK||anow->pos[0]>CAMERA_WIDTH+CAMERA_THICK||anow->pos[1]<-CAMERA_THICK||anow->pos[1]>CAMERA_HEIGHT+CAMERA_THICK){
            anow->stat=HADying;anow->age=HEROAMMO_FADETIME;
        }
    }
    for(TEnemy* anow=hEnemyAmmo;anow;anow=anow->next){
        if(anow->pos[0]<-CAMERA_THICK||anow->pos[0]>CAMERA_WIDTH+CAMERA_THICK||anow->pos[1]<Camera.bottom-CAMERA_THICK||anow->pos[1]>Camera.bottom+CAMERA_HEIGHT+CAMERA_THICK){
            anow->stat=EDying;anow->age=anow->tdie;
        }
    }
    for(TEnemy* anow=hEnemy;anow;anow=anow->next){
        if(anow->pos[0]<-CAMERA_THICK||anow->pos[0]>CAMERA_WIDTH+CAMERA_THICK||anow->pos[1]<Camera.bottom-CAMERA_THICK){
            anow->stat=EDying;anow->age=anow->tdie;
        }
        if(anow->stat==Waiting){
            if(anow->pos[1]<=Camera.bottom+CAMERA_HEIGHT+CAMERA_THICK) anow->stat=EGenerating;
            continue;
        }
        if(anow->pos[1]>Camera.bottom+CAMERA_HEIGHT+CAMERA_THICK){
            anow->stat=EDying;anow->age=anow->tdie;
        }
    }
    for(THeroAmmo* anow=hHeroAmmo;anow;anow=anow->next){
        if(anow->pos[1]>Camera.bottom+CAMERA_HEIGHT+CAMERA_THICK) {anow->stat=HADying;anow->age=0;}
        if(anow->stat==HADying) continue;
        anow->pos[1]+=Camera.bottom;
        for(TEnemy* enow=hEnemy;enow;enow=enow->next){
            if(enow->stat==EDying) continue;
            if(dist(anow->pos,enow->pos)<=sqr(HEROAMMO_RADIUS+enow->radi)){
                enow->life-=Player.dshoot;
                if(enow->life>0){
                    anow->stat=HADying;
                    anow->age=0;
                }
            }
        }
        anow->pos[1]-=Camera.bottom;
    }
    if(Player.stat==Bomb){
        for(TEnemy* enow=hEnemy;enow;enow=enow->next){
            if(enow->stat==EDying||enow->stat==EGenerating) continue;
            if(abs(enow->pos[0]-HBomb.xpos)<HEROBOMB_HALFWIDTH) {
                enow->life-=1;
                if(enow->life<=0){enow->stat=EDying;enow->age=0;}
            }
        }
        for(TEnemy* enow=hEnemyAmmo;enow;enow=enow->next){
            if(enow->stat==EDying||enow->stat==EGenerating) continue;
            if(abs(enow->pos[0]-HBomb.xpos)<HEROBOMB_HALFWIDTH) {
                enow->stat=EDying;enow->age=0;
            }
        }
    }
    if(Player.stat!=HDying){
        bool HHurt=(Player.stat==HNormal)||(Player.stat==Bomb&&abs(Player.pos[0]-HBomb.xpos)>HEROBOMB_HALFWIDTH);
        Player.pos[1]+=Camera.bottom;
        for(TEnemy* enow=hEnemy;enow;enow=enow->next){
            if(enow->stat==EDying||enow->stat==EGenerating) continue;
            if(dist(enow->pos,Player.pos)>=sqr(Player.hrad+enow->radi)) continue;
            if(enow->hard&&HHurt){
                Player.stat=HDying;
                Player.dtime=HERO_DIETIME;
                break;
            }else if(!enow->shield){
                enow->life-=1;
                if(enow->life<=0){enow->stat=EDying;enow->age=0;}
            }
        }
        for(TEnemy* enow=hEnemyAmmo;enow;enow=enow->next){
            if(enow->stat==EDying||enow->stat==EGenerating) continue;
            if(dist(enow->pos,Player.pos)>=sqr(Player.arad+enow->radi)) continue;
            if(enow->life>1){
                enow->life-=1;
                TPowerUp* bon=getPowerUp();
                if(bon){
                    bon->type=sPower;
                    bon->pos[0]=enow->pos[0];bon->pos[1]=enow->pos[1];
                    bon->next=hPowerUp;
                    hPowerUp=bon;
                }
            }
            if(dist(enow->pos,Player.pos)>=sqr(Player.hrad+enow->radi)) continue;
            if(HHurt){
                Player.stat=HDying;
                Player.dtime=HERO_DIETIME;
                enow->stat=EDying;enow->age=0;break;
            }
            enow->stat=EDying;enow->age=0;
        }
        Player.pos[1]-=Camera.bottom;
    }
    for(TPowerUp* now=hPowerUp;now;now=now->next){
        if(dist(now->pos,Player.pos)<=sqr(Player.arad)){
            now->pos[1]=-CAMERA_THICK;
            switch(now->type){
                case sPower:++Player.power;if(Player.power>HERO_MAXPOWER) Player.power=HERO_MAXPOWER;break;
                case mPower:Player.power+=10;if(Player.power>HERO_MAXPOWER) Player.power=HERO_MAXPOWER;break;
                case lPower:Player.power+=100;if(Player.power>HERO_MAXPOWER) Player.power=HERO_MAXPOWER;break;
                case fPower:Player.power=HERO_MAXPOWER;break;
                case PShield:Player.stat=Shield;Player.stime+=3*GAME_FPS;break;
                case Levelup:
                    switch(Player.level){
                        case 1:Player.tshoot-=60;break;
                        case 2:Player.tshoot-=45;Player.dhit+=1;break;
                        case 3:Player.dshoot+=1;Player.tbomb+=600;break;
                        case 4:Player.tbomb+=200;Player.tlast+=600;break;
                        default:Camera.ranrange*=0.7;break;
                    }
                    ++Player.level;
                    break;
                case Clear:
                    for(TEnemy* enow=hEnemyAmmo;enow;enow=enow->next){
                        if(enow->stat==EDying||enow->stat==EGenerating) continue;
                        enow->stat=EDying;enow->age=0;
                    }
                    break;
                case Shake:
                    ++cntShake;
                    break;
            }
        }
    }
    //Make DisplayList
    updCache^=1;
    TScreenItem *scrhead=background[updCache],*scrptr;
    scrptr=scrhead;
    if(Player.stat==Bomb){
        if(HBomb.age==0){
            HBomb.gen[0]->reset();
            HBomb.gen[1]->reset() ;
            HBomb.norm[0]->reset();
            HBomb.norm[1]->reset();
            HBomb.fade[0]->reset();
            HBomb.fade[1]->reset();
            HBomb.gen[0]->cen[0]=HBomb.gen[1]->cen[0]=HBomb.norm[0]->cen[0]=HBomb.xpos;
            HBomb.gen[0]->cen[1]=HBomb.gen[1]->cen[1]=HBomb.norm[0]->cen[1]=280;
            HBomb.norm[1]->cen[0]=HBomb.fade[1]->cen[0]=HBomb.fade[0]->cen[0]=HBomb.xpos;
            HBomb.norm[1]->cen[1]=HBomb.fade[1]->cen[1]=HBomb.fade[0]->cen[1]=280;
        }
        if(HBomb.age<=HEROBOMB_GENTIME){
            scrptr->Next=HBomb.gen[updCache];
            scrptr=scrptr->Next;
        }
        if(HBomb.age>HEROBOMB_GENTIME&&HBomb.age<=HEROBOMB_GENTIME+Player.tbomb){
            scrptr->Next=HBomb.norm[updCache];
            scrptr=scrptr->Next;
        }
        if(HBomb.age>HEROBOMB_GENTIME+Player.tbomb&&HBomb.age<=HEROBOMB_GENTIME+Player.tbomb+HEROBOMB_FADETIME){
            scrptr->Next=HBomb.fade[updCache];
            scrptr=scrptr->Next;
        }
        ++HBomb.age;
    }
    if(Player.stat==Lastword){
        if(HLast.age==0){
            HLast.gen[0]->reset();
            HLast.gen[1]->reset() ;
            HLast.norm[0]->reset();
            HLast.norm[1]->reset();
            HLast.fade[0]->reset();
            HLast.fade[1]->reset();
            HLast.gen[0]->cen[0]=HLast.gen[1]->cen[0]=HLast.norm[0]->cen[0]=200;
            HLast.gen[0]->cen[1]=HLast.gen[1]->cen[1]=HLast.norm[0]->cen[1]=280;
            HLast.norm[1]->cen[0]=HLast.fade[1]->cen[0]=HLast.fade[0]->cen[0]=200;
            HLast.norm[1]->cen[1]=HLast.fade[1]->cen[1]=HLast.fade[0]->cen[1]=280;
        }
        if(HLast.age<=HEROBOMB_GENTIME){
            scrptr->Next=HLast.gen[updCache];
            scrptr=scrptr->Next;
        }
        if(HLast.age>HEROBOMB_GENTIME&&HLast.age<=HEROBOMB_GENTIME+Player.tlast){
            scrptr->Next=HLast.norm[updCache];
            scrptr=scrptr->Next;
        }
        if(HLast.age>HEROBOMB_GENTIME+Player.tlast&&HLast.age<=HEROBOMB_GENTIME+Player.tbomb+HEROBOMB_FADETIME){
            scrptr->Next=HLast.fade[updCache];
            scrptr=scrptr->Next;
        }
        ++HLast.age;
    }
    for(THeroAmmo* anow=hHeroAmmo;anow;anow=anow->next){
        switch(anow->stat){
            case HANormal:
                scrptr->Next=(anow->age<=HEROAMMO_GENTIME?anow->egen[updCache]:anow->ammo[updCache]);
                scrptr=scrptr->Next;
                scrptr->cen[0]=anow->pos[0];scrptr->cen[1]=anow->pos[1];
                break;
            case HADying:
                scrptr->Next=anow->edie[updCache];
                scrptr=scrptr->Next;
                scrptr->cen[0]=anow->pos[0];scrptr->cen[1]=anow->pos[1];
                break;
        }
    }
    if(Player.stat==HDying){
        scrptr->Next=Player.die[updCache];
        scrptr=scrptr->Next;
        scrptr->cen[0]=Player.pos[0];scrptr->cen[1]=Player.pos[1];
    }else{
        if(Player.slow){
            scrptr->Next=Player.alrt[updCache];
            scrptr=scrptr->Next;
            scrptr->cen[0]=Player.pos[0];scrptr->cen[1]=Player.pos[1];
        }
        if(abs(hxmove)>eps){
            scrptr->Next=(hxmove>0?Player.righ[updCache]:Player.left[updCache]);
            scrptr=scrptr->Next;
            scrptr->cen[0]=Player.pos[0];scrptr->cen[1]=Player.pos[1];
        }else{
            scrptr->Next=Player.stra[updCache];
            scrptr=scrptr->Next;
            scrptr->cen[0]=Player.pos[0];scrptr->cen[1]=Player.pos[1];
        }
        if(Player.slow){
            scrptr->Next=Player.cent[updCache];
            scrptr=scrptr->Next;
            scrptr->cen[0]=Player.pos[0];scrptr->cen[1]=Player.pos[1];
        }
        if(Player.stat==Lastword||(Player.stat==Bomb&&abs(Player.pos[0]-HBomb.xpos)<HEROBOMB_HALFWIDTH)){
            scrptr->Next=Player.shie[updCache];
            scrptr=scrptr->Next;
            scrptr->cen[0]=Player.pos[0];scrptr->cen[1]=Player.pos[1];
        }
    }
    for(TEnemy* enow=hEnemy;enow;enow=enow->next){
        switch(enow->stat){
            case Waiting:break;
            case EGenerating:
                scrptr->Next=enow->gen[updCache];break;
            case EUp:
                scrptr->Next=enow->up[updCache];break;
            case EDown:
                scrptr->Next=enow->down[updCache];break;
            case ELeft:
                scrptr->Next=enow->left[updCache];break;
            case Eright:
                scrptr->Next=enow->right[updCache];break;
            case EDying:
                scrptr->Next=enow->death[updCache];break;
        }
        if(enow->stat==Waiting) continue;
        scrptr->cen[0]=enow->pos[0];scrptr->cen[1]=enow->pos[1]-Camera.bottom;
        scrptr=scrptr->Next;
    }
    for(TEnemy* enow=hEnemyAmmo;enow;enow=enow->next){
        switch(enow->stat){
            case EGenerating:
                scrptr->Next=enow->gen[updCache];break;
            case EUp:
                scrptr->Next=enow->up[updCache];break;
            case EDown:
                scrptr->Next=enow->down[updCache];break;
            case ELeft:
                scrptr->Next=enow->left[updCache];break;
            case Eright:
                scrptr->Next=enow->right[updCache];break;
            case EDying:
                scrptr->Next=enow->death[updCache];break;
            default:break;
        }
        scrptr=scrptr->Next;
        scrptr->cen[0]=enow->pos[0];scrptr->cen[1]=enow->pos[1]-Camera.bottom;
    }
    for(TPowerUp* pnow=hPowerUp;pnow;pnow=pnow->next){
        scrptr->Next=pnow->cont[updCache];
        scrptr=scrptr->Next;
        scrptr->cen[0]=pnow->pos[0];scrptr->cen[1]=pnow->pos[1];
    }
    scrptr->Next=0;
    for(scrptr=scrhead->Next;scrptr;scrptr=scrptr->Next){
        scrptr->cen[0]+=origpos[0];scrptr->cen[1]+=origpos[1];
    }
    glfwLockMutex(Locker);
    drawList=scrhead;
    glfwUnlockMutex(Locker);
    printf("Update end\n");
    //printf("123/0=%d\n",123/(456-456));
}
void TGameBoard::drawBoard(){
    //printf("Draw111\n");
    while(!drawList);
    //printf("Draw Begin\n");
    glfwLockMutex(Locker);
    int i=0;
    for(TScreenItem* now=drawList;now;now=now->Next) {now->drawGL();printf("Draw %d\n",now);++i;}
    printf("%d Scritems have been painted\n",i);
    glfwUnlockMutex(Locker);
    //printf("Draw End\n");
}
void TGameBoard::LoadStage(){
    glfwLockMutex(Locker);
    drawList=&loadinganim;
    loadinganim.reset();
    glfwUnlockMutex(Locker);
    freopen(stagepath,"r",stdin);
    //printf("!!!!!\n");
    //freopen("/home/test.log","w",stdout);
    //printf("!!!!!\n");
    int bgim=6513,esk,eask,ene,frc;
    char pat[300];
    //Load background images
    scanf("%d",&bgim);
    background[0]=getScrItem();background[1]=getScrItem();
    for(int i=0;i<bgim;++i){
        scanf("%f%s",&bkimgsegement[i],pat);
        background[0]->loadframe(getFrame(),pat);
    }
    //printf("111111\n");
    background[0]->init();
    //printf("#####\n");
    background[1]->copyitem(background[0]);
    background[0]->cen[0]=background[1]->cen[0]=400;background[0]->cen[1]=background[1]->cen[1]=300;
    background[0]->hwid=background[1]->hwid=200;background[0]->hhei=background[1]->hhei=300;
    bksegnow=0;
    printf("%d %d\n",background[0],background[1]);
    //printf("22222\n");
    //Load Hero Images
    Player.alrt[0]=getScrItem();
    Player.alrt[0]->loadframe(getFrame(),"img/hero/heroalarm.tga");

    Player.alrt[0]->init();
    Player.alrt[1]=getScrItem();
    Player.alrt[1]->copyitem(Player.alrt[0]);
    Player.cent[0]=getScrItem();
    Player.cent[0]->loadframe(getFrame(),"img/hero/heroheart.tga");
    Player.cent[0]->init();
    Player.cent[1]=getScrItem();
    Player.cent[1]->copyitem(Player.cent[0]);
    Player.die[0]=getScrItem();
    Player.die[0]->loadframe(getFrame(),"img/hero/herodeath.tga");
    Player.die[0]->init();
    Player.die[1]=getScrItem();
    Player.die[1]->copyitem(Player.die[0]);
    Player.left[0]=getScrItem();
    Player.left[0]->loadframe(getFrame(),"img/hero/herobody.tga");
    Player.left[0]->init();
    Player.left[1]=getScrItem();
    Player.left[1]->copyitem(Player.left[0]);
    Player.righ[0]=getScrItem();
    Player.righ[0]->copyitem(Player.left[0]);
    Player.righ[1]=getScrItem();
    Player.righ[1]->copyitem(Player.righ[0]);
    Player.stra[0]=getScrItem();
    Player.stra[0]->copyitem(Player.left[0]);
    Player.stra[1]=getScrItem();
    Player.stra[1]->copyitem(Player.stra[0]);
    Player.shie[0]=getScrItem();
    Player.shie[0]->loadframe(getFrame(),"img/hero/heroshield.tga");
    Player.shie[0]->init();
    Player.shie[1]=getScrItem();
    Player.shie[1]->copyitem(Player.shie[0]);
    Player.alrt[0]->hwid=Player.alrt[1]->hwid=25;Player.alrt[0]->hhei=Player.alrt[1]->hhei=25;
    Player.cent[0]->hwid=Player.cent[1]->hwid=2;Player.cent[0]->hhei=Player.cent[1]->hhei=2;
    Player.stra[0]->hwid=Player.stra[1]->hwid=13;Player.stra[0]->hhei=Player.stra[1]->hhei=24;
    Player.righ[0]->hwid=Player.righ[1]->hwid=13;Player.righ[0]->hhei=Player.righ[1]->hhei=24;
    Player.left[0]->hwid=Player.left[1]->hwid=13;Player.left[0]->hhei=Player.left[1]->hhei=24;
    mHeroAmmo=getHeroAmmo();
    mHeroAmmo->ammo[0]=getScrItem();
    mHeroAmmo->ammo[0]->loadframe(getFrame(),"img/hero/ha.tga");
    mHeroAmmo->ammo[0]->init();
    mHeroAmmo->ammo[1]=getScrItem();
    mHeroAmmo->ammo[1]->copyitem(mHeroAmmo->ammo[0]);
    mHeroAmmo->egen[0]=getScrItem();
    mHeroAmmo->egen[0]->copyitem(mHeroAmmo->ammo[0]);
    mHeroAmmo->egen[1]=getScrItem();
    mHeroAmmo->egen[1]->copyitem(mHeroAmmo->egen[0]);
    mHeroAmmo->edie[0]=getScrItem();
    mHeroAmmo->edie[0]->loadframe(getFrame(),"img/hero/had.tga");
    mHeroAmmo->edie[0]->init();
    mHeroAmmo->edie[1]=getScrItem();
    mHeroAmmo->edie[1]->copyitem(mHeroAmmo->edie[0]);
    HBomb.gen[0]=getScrItem();
    HBomb.gen[0]->loadframe(getFrame(),"img/hero/bomb.tga");
    HBomb.gen[0]->init();
    HBomb.gen[1]=getScrItem();
    HBomb.gen[1]->copyitem(HBomb.gen[0]);
    HBomb.norm[0]=getScrItem();
    HBomb.norm[0]->copyitem(HBomb.gen[0]);
    HBomb.norm[1]=getScrItem();
    HBomb.norm[1]->copyitem(HBomb.norm[0]);
    HBomb.fade[0]=getScrItem();
    HBomb.fade[0]->copyitem(HBomb.gen[0]);
    HBomb.fade[1]=getScrItem();
    HBomb.fade[1]->copyitem(HBomb.fade[0]);
    HLast.gen[0]=getScrItem();
    HLast.gen[0]->loadframe(getFrame(),"img/hero/last.tga");
    HLast.gen[0]->init();
    HLast.gen[1]=getScrItem();
    HLast.gen[1]->copyitem(HLast.gen[0]);
    HLast.norm[0]=getScrItem();
    HLast.norm[0]->copyitem(HLast.gen[0]);
    HLast.norm[1]=getScrItem();
    HLast.norm[1]->copyitem(HLast.norm[0]);
    HLast.fade[0]=getScrItem();
    HLast.fade[0]->copyitem(HLast.gen[0]);
    HLast.fade[1]=getScrItem();
    HLast.fade[1]->copyitem(HLast.fade[0]);
    Camera.actspeed=1;
    //printf("33333\n");
    //Load Enemy Skins
    scanf("%d",&esk);
    for(int i=0;i<esk;++i){
        for(int j=0;j<6;++j){
            EnemySkin[j][i]=getScrItem();
            scanf("%d",&frc);
            for(int k=1;k<=frc;++k){
                scanf("%s",pat);
                EnemySkin[j][i]->loadframe(getFrame(),pat);
            }
            EnemySkin[j][i]->init();
        }
    }
    //Load EnemyAmmo Skin
    scanf("%d",&eask);
    for(int i=0;i<eask;++i){
        for(int j=0;j<6;++j){
            EnemyAmmoSkin[j][i]=getScrItem();
            scanf("%d",&frc);
            for(int k=1;k<=frc;++k){
                scanf("%s",pat);
                EnemyAmmoSkin[j][i]->loadframe(getFrame(),pat);
            }
            EnemyAmmoSkin[j][i]->init();
        }
    }
    origpos[0]=200;origpos[1]=0;
    //Load&Create Enemy
    float ehw,ehh,er;
    scanf("%d",&ene);
    TEnemy *nowEnemy;
    for(int i=0;i<ene;++i){
        if(!hEnemy) {
        nowEnemy=hEnemy=getEnemy();
        }else{
            nowEnemy->next=getEnemy();
            nowEnemy=nowEnemy->next;
        }
        nowEnemy->gen[0]=getScrItem();
        nowEnemy->gen[1]=getScrItem();
        nowEnemy->up[0]=getScrItem();
        nowEnemy->up[1]=getScrItem();
        if(nowEnemy->up[0]==0||nowEnemy->up[1]==0) {printf("Fuck!!!!!\n");printf("%d",123/(456-456));}
        nowEnemy->down[0]=getScrItem();
        nowEnemy->down[1]=getScrItem();
        nowEnemy->left[0]=getScrItem();
        nowEnemy->left[1]=getScrItem();
        nowEnemy->right[0]=getScrItem();
        nowEnemy->right[1]=getScrItem();
        nowEnemy->death[0]=getScrItem();
        nowEnemy->death[1]=getScrItem();
        nowEnemy->stat=Waiting;
        scanf("%f%f%f%d%d%f%f",&ehw,&ehh,&er,&nowEnemy->type,&frc,&nowEnemy->pos[0],&nowEnemy->pos[1]);
        nowEnemy->gen[0]->copyitem(EnemySkin[0][frc]);
        nowEnemy->gen[1]->copyitem(EnemySkin[0][frc]);
        nowEnemy->up[0]->copyitem(EnemySkin[1][frc]);
        nowEnemy->up[1]->copyitem(EnemySkin[1][frc]);
        nowEnemy->down[0]->copyitem(EnemySkin[2][frc]);
        nowEnemy->down[1]->copyitem(EnemySkin[2][frc]);
        nowEnemy->left[0]->copyitem(EnemySkin[3][frc]);
        nowEnemy->left[1]->copyitem(EnemySkin[3][frc]);
        nowEnemy->right[0]->copyitem(EnemySkin[4][frc]);
        nowEnemy->right[1]->copyitem(EnemySkin[4][frc]);
        nowEnemy->death[0]->copyitem(EnemySkin[5][frc]);
        nowEnemy->death[1]->copyitem(EnemySkin[5][frc]);
        nowEnemy->radi=er;
        nowEnemy->gen[0]->hwid=nowEnemy->gen[1]->hwid=ehw;
        nowEnemy->up[0]->hwid=nowEnemy->up[1]->hwid=ehw;
        nowEnemy->down[0]->hwid=nowEnemy->down[1]->hwid=ehw;
        nowEnemy->left[0]->hwid=nowEnemy->left[1]->hwid=ehw;
        nowEnemy->right[0]->hwid=nowEnemy->right[1]->hwid=ehw;
        nowEnemy->death[0]->hwid=nowEnemy->death[1]->hwid=ehw;
        nowEnemy->gen[0]->hhei=nowEnemy->gen[1]->hhei=ehh;
        nowEnemy->up[0]->hhei=nowEnemy->up[1]->hhei=ehh;
        nowEnemy->down[0]->hhei=nowEnemy->down[1]->hhei=ehh;
        nowEnemy->left[0]->hhei=nowEnemy->left[1]->hhei=ehh;
        nowEnemy->right[0]->hhei=nowEnemy->right[1]->hhei=ehh;
        nowEnemy->death[0]->hhei=nowEnemy->death[1]->hhei=ehh;
    }
    nowEnemy->next=0;
    fclose(stdin);
    bStat=LoadFinish;
}
void TGameBoard::UnloadGame(){
    glfwLockMutex(Locker);
    drawList=&loadinganim;
    loadinganim.reset();
    glfwUnlockMutex(Locker);
    hEnemy=hEnemyAmmo=0;hHeroAmmo=0;hPowerUp=0;
    aframe=&framepool[0];
    aEnemy=&EnemyPool[0];
    aEnemyAmmo=&EnemyAmmoPool[0];
    aHeroAmmo=&HeroAmmoPool[0];
    ascritem=&ScrItemPool[0];
    aPowerUp=&PowerUpPool[0];
    for(int i=0;i<99999;++i){
        glDeleteTextures(1,&framepool[i].content);
        framepool[i].next=&framepool[i+1];
        ScrItemPool[i].Next=&ScrItemPool[i+1];
        ScrItemPool[i].reinit();
        EnemyAmmoPool[i].next=&EnemyAmmoPool[i+1];
        PowerUpPool[i].next=&PowerUpPool[i+1];
    }
    framepool[99999].next=0;
    ScrItemPool[99999].Next=0;
    PowerUpPool[99999].next=0;
    EnemyAmmoPool[99999].next=EnemyPool[4999].next=0;
    HeroAmmoPool[49].next=0;
    for(int i=0;i<4999;++i)
        EnemyPool[i].next=&EnemyPool[i+1];
    for(int i=0;i<49;++i)
        HeroAmmoPool[i].next=&HeroAmmoPool[i+1];
}
void TGameBoard::prep(){
    //printf("Prepare\n");
    glfwLockMutex(Locker);
    TFrame* frmnow=&lframes[0];
    loadinganim.loadframe(frmnow,"img/loading/loading1.tga");
    loadinganim.init();
    loadinganim.cen[0]=600;loadinganim.cen[1]=100;loadinganim.hwid=100;loadinganim.hhei=50;
    glfwUnlockMutex(Locker);
    //printf("!!!!!\n");
    UnloadGame();
    //printf("#####\n");

    bStat=Nothing;
    drawList=0;
    //printf("Prepare Done!\n");
}
