#include "List.h"
#include "Game_iterator.h"
#include "Company_iterator.h"
#include <string.h>
#include <stdlib.h>
Game_iterator* STList::GameFirst(){
    return(ghead->next);
}
Game_iterator* STList::GameLast(){
    return(ghead->prev);
}
Company_iterator* STList::CompanyFirst(){
    return(chead->next);
}
Company_iterator* STList::CompanyLast(){
    return(chead->prev);
}
void STList::createachi(int num){
    Achieve* tmp;
    if(aachi==0){
        aachi=(Achieve*)malloc(sizeof(Achieve));
        aachi->prev=aachi->next=aachi;
    }
    for(int i=0;i<num;++i){
        tmp=(Achieve*)malloc(sizeof(Achieve));
        tmp->prev=aachi;
        tmp->next=aachi->next;
        tmp->prev->next=tmp->next->prev=tmp;
    }
    caachi+=num;
}
void STList::createcomp(int num){
    Company_iterator* tmp;
    if(acomp==0){
        acomp=(Company_iterator*)malloc(sizeof(Company_iterator));
        acomp->prev=acomp->next=acomp;
        acomp->global=this;
        acomp->init();
        ut.givestr(acomp->content.Name,"Spare Head");
    }
    for(int i=0;i<num;++i){
        tmp=(Company_iterator*)malloc(sizeof(Company_iterator));
        tmp->prev=acomp;tmp->next=acomp->next;
        tmp->prev->next=tmp->next->prev=tmp;
        tmp->global=this;
        tmp->init();
    }
    cacomp+=num;
}
void STList::creategame(int num){
    Game_iterator* tmp;
    if(agame==0){
        agame=(Game_iterator*)malloc(sizeof(Game_iterator));
        agame->next=agame->prev=agame;
        agame->global=this;
        agame->init();
    }
    for(int i=0;i<num;++i){
        tmp=(Game_iterator*)malloc(sizeof(Game_iterator));
        tmp->prev=agame;
        tmp->next=agame->next;
        tmp->prev->next=tmp->next->prev=tmp;
        tmp->global=this;
        tmp->init();
    }
    cagame+=num;
}
Achieve* STList::GetAchieve(){
    if(aachi==0) createachi(ALLOCATE_UNITS);
    if(aachi->next==aachi) createachi(ALLOCATE_UNITS);
    Achieve* ans=aachi;
    aachi->next->prev=aachi->prev;
    aachi->prev->next=aachi->next;
    aachi=aachi->next;
    ans->next=ans->prev=ans;
    --caachi;
    return(ans);
}
Game_iterator* STList::GetGameIterator(){
    if(agame==0) creategame(ALLOCATE_UNITS);
    if(agame->next==agame) creategame(ALLOCATE_UNITS);
    Game_iterator* ans=agame;
    agame->next->prev=agame->prev;
    agame->prev->next=agame->next;
    agame=agame->next;
    ans->next=ans->prev=ans->cnext=ans->cprev=ans;
    --cagame;
    return(ans);
}
Company_iterator* STList::GetCompIterator(){
    if(acomp==0) createcomp(ALLOCATE_UNITS);
    if(acomp->next==acomp) createcomp(ALLOCATE_UNITS);
    Company_iterator* ans=acomp;
    acomp->next->prev=acomp->prev;
    acomp->prev->next=acomp->next;
    acomp=acomp->next;
    --cacomp;
    return(ans);
}
void STList::deleteAchieve(Achieve* tar){
    if(tar==0) return;
    ++caachi;
    tar->next=aachi;
    tar->prev=aachi->prev;
    aachi->prev->next=tar;
    aachi->prev=tar;
    aachi=tar;
    if(caachi>=3*ALLOCATE_UNITS){
        Achieve *now=aachi,*head=aachi->prev,*tail=aachi;
        for(int i=1;i<=ALLOCATE_UNITS;++i){
            tail=tail->next;free(now);now=tail;
        }
        head->next=tail;tail->prev=head;aachi=head;
    }
}
void STList::deleteGame(Game_iterator* tar){
    if(tar==0) return;
    deleteAchieve(tar->head);
    tar->head=0;
    ++cagame;
    tar->next=agame;
    tar->prev=agame->prev;
    agame->prev->next=tar;
    agame->prev=tar;
    agame=tar;
    if(cagame>=3*ALLOCATE_UNITS){
        Game_iterator *now=agame,*head=agame->prev,*tail=agame;
        for(int i=1;i<=ALLOCATE_UNITS;++i){
            tail=tail->next;free(now);now=tail;
        }
        head->next=tail;tail->prev=head;agame=head;
    }
}
void STList::deleteCompany(Company_iterator* tar){
    if(tar==0) return;
    deleteGame(tar->head);
    tar->head=0;
    ++cacomp;
    tar->next=acomp;
    tar->prev=acomp->prev;
    acomp->prev->next=tar;
    acomp->prev=tar;
    acomp=tar;
    if(cacomp>=3*ALLOCATE_UNITS){
        Company_iterator *now=acomp,*head=acomp->prev,*tail=acomp;
        for(int i=1;i<=ALLOCATE_UNITS;++i){
            tail=tail->next;free(now);now=tail;
        }
        head->next=tail;tail->prev=head;acomp=head;
    }
}
Achieve* STList::DelAchieve(Achieve* tar){
    if(tar==0) return(0);
    Achieve *ans=tar->next;
    tar->next->prev=tar->prev;
    tar->prev->next=tar->next;
    deleteAchieve(tar);
    return(ans);
}
Game_iterator* STList::DelGame(Game_iterator* tar){
    if(tar==0) return(0);
    Game_iterator *ans=tar->cnext;
    tar->prev->next=tar->next;
    tar->cnext->cprev=tar->cprev;
    tar->next->prev=tar->prev;
    tar->cprev->cnext=tar->cnext;
    for(Achieve* now=tar->head->next;now!=tar->head;now=DelAchieve(now));
    deleteGame(tar);
    return(ans);
}
Company_iterator* STList::DelCompany(Company_iterator* tar){
    if(tar==0) return(0);
    Company_iterator *ans=tar->next;
    tar->prev->next=tar->next;
    tar->next->prev=tar->prev;
    for(Game_iterator* now=tar->head->cnext;now!=tar->head;now=DelGame(now));
    deleteCompany(tar);
    return(ans);
}
STList::STList(){
    chead=GetCompIterator();
    chead->prev=chead->next=chead;
    ghead=GetGameIterator();
    ghead->prev=ghead->next=ghead;
}
STList::~STList(){
    for(Company_iterator* now=chead->next;now!=chead;now=DelCompany(now));
    DelCompany(chead);
    DelGame(ghead);
    Achieve *atmp,*anow;
    for(anow=aachi->next;anow!=aachi;){
        atmp=anow->next;
        free(anow);
        anow=atmp;
    }
    free(aachi);
    Game_iterator *gtmp,*gnow;
    for(gnow=agame->next;gnow!=agame;){
        gtmp=gnow->next;
        free(gnow);
        gnow=gtmp;
    }
    free(agame);
    Company_iterator *ctmp,*cnow;
    for(cnow=acomp;cnow!=acomp;){
        ctmp=cnow->next;
        free(cnow);
        cnow=ctmp;
    }
    free(acomp);
}
void STList::qsort_game(Game_iterator* l,Game_iterator* r,bool ( * cmp )(Game_iterator* a,Game_iterator* b)){
    Game_iterator *ll=l,*rr=r,*x=l,*gne,*gpr;
    int itmp=1,tot=0;
    for(;x->next!=r;x=x->next)
        ++tot;
    ++tot;
    for(x=l;itmp<=(tot>>1);++itmp) x=x->next;
    while(!(ll->order>rr->order)){
        while((*cmp)(ll,x)) ll=ll->next;
        while((*cmp)(x,rr)) rr=rr->prev;
        if(!(ll->order>rr->order)){
            gne=ll->next;
            gpr=rr->prev;
            if(ll->next!=rr){
                ll->next=rr->next;
                rr->next=gne;
                rr->prev=ll->prev;
                ll->prev=gpr;
                itmp=ll->order;ll->order=rr->order;rr->order=itmp;
                ll->next->prev=ll->prev->next=ll;
                rr->next->prev=rr->prev->next=rr;
                ll=gne;
                rr=gpr;
            }else{
                ll->next=rr->next;
                rr->prev=ll->prev;
                ll->prev=rr;rr->next=ll;
                itmp=ll->order;ll->order=rr->order;rr->order=itmp;
                ll->next->prev=ll->prev->next=ll;
                rr->next->prev=rr->prev->next=rr;
            }

        }
    }
    if(l->order<rr->order) qsort_game(l,rr,cmp);
    if(ll->order<r->order) qsort_game(ll,r,cmp);
}
void STList::qsort_game_company(Game_iterator* l,Game_iterator* r,bool ( * cmp )(Game_iterator* a,Game_iterator* b)){
    Game_iterator *ll=l,*rr=r,*x=l,*gne,*gpr,*gtmp;
    int itmp=0,tot=0;
    for(;x->cnext!=r;x=x->cnext) ++tot;
    ++tot;
    for(x=l;itmp<=(tot>>1);++itmp) x=x->cnext;
    while(!(ll->order>rr->order)){
        while((*cmp)(ll,x)) ll=ll->next;
        while((*cmp)(x,rr)) rr=rr->prev;
        if(!(ll->order>rr->order)){
            gne=ll->cnext;
            gpr=rr->cprev;
            if(ll->cnext!=rr){
                ll->cnext=rr->cnext;rr->cnext=gne;
                rr->cprev=ll->cprev;ll->cprev=gpr;
            }else{
                ll->cnext=rr->cnext;
                rr->cprev=ll->cprev;
                ll->cprev=rr;
                rr->cnext=ll;
            }
            if(ll->next!=rr){
                gtmp=ll->next;ll->next=rr->next;rr->next=gtmp;
                gtmp=ll->prev;ll->prev=rr->prev;rr->prev=gtmp;
                itmp=ll->order;ll->order=rr->order;rr->order=itmp;
                ll->cprev->cnext=ll->prev->next=ll->next->prev=ll->cnext->cprev=ll;
                rr->cprev->cnext=rr->prev->next=rr->next->prev=rr->cnext->cprev=rr;
                ll=gne;
                rr=gpr;
            }else{
                ll->next=rr->next;
                rr->prev=ll->prev;
                ll->prev=rr;
                rr->next=ll;
                itmp=ll->order;ll->order=rr->order;rr->order=itmp;
                ll->cprev->cnext=ll->prev->next=ll->next->prev=ll->cnext->cprev=ll;
                rr->cprev->cnext=rr->prev->next=rr->next->prev=rr->cnext->cprev=rr;
            }
        }
    }
    if(l->order<rr->order) qsort_game_company(l,rr,cmp);
    if(ll->order<r->order) qsort_game_company(ll,r,cmp);
}
void STList::qsort_company(Company_iterator* l,Company_iterator* r,bool ( * cmp )(Company_iterator* a,Company_iterator* b)){
    Company_iterator *ll=l,*rr=r,*x=l,*cne,*cpr;
    int itmp=0,tot=0;
    for(;x->next!=r;x=x->next) ++tot;
    ++tot;
    for(x=l;itmp<=(tot>>1);++itmp) x=x->next;
    while(!(ll->order>rr->order)){
        while((*cmp)(ll,x)) ll=ll->next;
        while((*cmp)(x,rr)) rr=rr->prev;
        if(!(ll->order>rr->order)){
            if(ll->next!=rr){
                cne=ll->next;ll->next=rr->next;rr->next=cne;
                cpr=rr->prev;rr->prev=ll->prev;ll->prev=cpr;
                itmp=ll->order;ll->order=rr->order;rr->order=itmp;
                ll->prev->next=ll->next->prev=ll;
                rr->prev->next=rr->next->prev=rr;
                ll=cne;rr=cpr;
            }else{
                ll->next=rr->next;
                rr->prev=ll->prev;
                ll->prev=rr;
                rr->next=ll;
                ll->next->prev=ll->prev->next=ll;
                rr->next->prev=rr->prev->next=rr;
            }
        }
    }
    if(l->order<rr->order) qsort_company(l,rr,cmp);
    if(ll->order<r->order) qsort_company(ll,r,cmp);
}
void STList::InsertGame_p(Game_iterator *fr,Game_iterator *tar){
    if(fr==fr->comp->head){
        tar->prev=GameLast();
        tar->next=ghead;
    }else{
        tar->prev=fr;
        tar->next=fr->next;
    }
    tar->cprev=fr;
    tar->cnext=fr->cnext;
    tar->next->prev=tar->cnext->cprev=tar->prev->next=tar->cprev->cnext=tar;
    tar->comp=fr->comp;
    fr->comp->content.TotalGame++;
}
Game_iterator* STList::InsertGame(Game_iterator *fr,Game content){
    Game_iterator *tar=GetGameIterator();
    ut.givestr(tar->content.Name,content.Name);
    ut.givestr(tar->content.Description,content.Description);
    InsertGame_p(fr,tar);
    return(tar);
}
void STList::SortGame(Game_iterator* l,Game_iterator* r,bool ( * cmp )(Game_iterator* a,Game_iterator* b))
{
    int cnt=0;
    for(Game_iterator *now=ghead->next;now!=ghead;now=now->next) now->order=++cnt;
    if(l->order>r->order) qsort_game(r,l,cmp);else qsort_game(l,r,cmp);
    for(Company_iterator *now=chead->next;now!=chead;now=now->next)
        now->head->cnext=now->head->cprev=now->head;
    for(Game_iterator *now=ghead->next;now!=ghead;now=now->next){
        Game_iterator *fr=now->comp->head->prev;
        now->cprev=fr;
        now->cnext=fr->cnext;
        fr->cnext=now;
        now->cnext->cprev=now;
    }
}
void STList::SortGame_Company(Game_iterator* l,Game_iterator* r,bool ( * cmp )(Game_iterator* a,Game_iterator* b))
{
    if(l->comp!=r->comp) return;
    int cnt=0;
    Game_iterator *he=l->comp->head;
    for(Game_iterator *now=he->cnext;now!=he;now=now->cnext)
        now->order=++cnt;
    if(l->order<r->order) qsort_game_company(l,r,cmp);else qsort_game_company(r,l,cmp);
}
void STList::SortCompany(Company_iterator* l,Company_iterator* r,bool( * cmp )(Company_iterator* a,Company_iterator* b)){
    int cnt=0;
    for(Company_iterator *now=chead->next;now!=chead;now=now->next) now->order=++cnt;
    if(l->order<r->order) qsort_company(l,r,cmp);else qsort_company(r,l,cmp);
}
void STList::InsertCompany_p(Company_iterator* fr,Company_iterator* tar){
    tar->next=fr->next;tar->prev=fr;fr->next=tar;tar->next->prev=tar;
}
Company_iterator* STList::InsertCompany(Company_iterator* fr,Company content){
    Company_iterator *tar=GetCompIterator();
    tar->content=content;
    InsertCompany_p(fr,tar);
    return(tar);
}
Game_iterator* STList::FindGame(int flag,Game model,Game_iterator *head){
    for(Game_iterator *now=head;now!=ghead;now=now->next){
        //printf("Findgame %s",now->content.Name);
        switch(flag&GAME_COMPARENAME_COMPLETELYMATCH){
            case GAME_IGNORENAME:break;
            case GAME_COMPARENAME:
                    if(!ut.compstr_WC(now->content.Name,model.Name)) continue;
                    break;
            case GAME_COMPARENAME_NOWILDCARD:
                    if(!ut.compstr(now->content.Name,model.Name)) continue;
                    break;
            case GAME_COMPARENAME_COMPLETELYMATCH:
                    if(strcmp(now->content.Name,model.Name)!=0) continue;
                    break;
        }
        switch(flag&GAME_COMPAREDESC_COMPLETELYMATCH){
            case GAME_IGNOREDESC:break;
            case GAME_COMPAREDESC:
                    if(!ut.compstr_WC(now->content.Description,model.Description)) continue;
                    break;
            case GAME_COMPAREDESC_NOWILDCARD:
                    if(!ut.compstr(now->content.Description,model.Description)) continue;
                    break;
            case GAME_COMPAREDESC_COMPLETELYMATCH:
                    if(strcmp(now->content.Description,model.Description)!=0) continue;
                    break;
        }
        switch(flag&GAME_HOUR_EQUAL){
            case GAME_IGNOREHOUR:break;
            case GAME_HOUR_MORE:
                if(now->content.Hours<model.Hours) continue;
                break;
            case GAME_HOUR_LESS:
                if(now->content.Hours>model.Hours) continue;
                break;
            case GAME_HOUR_EQUAL:
                if(now->content.Hours!=model.Hours) continue;
                break;
        }
        switch(flag&GAME_TOTACHIEVE_EQUAL){
            case GAME_IGNORETOTACHIEVE:break;
            case GAME_TOTACHIEVE_MORE:
                if(now->content.TotAchieve<model.TotAchieve) continue;
                break;
            case GAME_TOTACHIEVE_LESS:
                if(now->content.TotAchieve>model.TotAchieve) continue;
                break;
            case GAME_TOTACHIEVE_EQUAL:
                if(now->content.TotAchieve!=model.TotAchieve) continue;
                break;
        }
        switch(flag&GAME_MYACHIEVE_EQUAL){
            case GAME_IGNOREMYACHIEVE:break;
            case GAME_MYACHIEVE_MORE:
                if(now->content.MyAchieve<model.MyAchieve) continue;
                break;
            case GAME_MYACHIEVE_LESS:
                if(now->content.MyAchieve>model.MyAchieve) continue;
                break;
            case GAME_MYACHIEVE_EQUAL:
                if(now->content.MyAchieve!=model.MyAchieve) continue;
                break;
        }
        return(now);
    }
    return(0);
}
Game_iterator* STList::FindGame_c(int flag,Game model,Game_iterator *head){
    Game_iterator *he=head->comp->head;
    for(Game_iterator *now=head;now!=he;now=now->cnext){
        switch(flag&GAME_COMPARENAME_COMPLETELYMATCH){
            case GAME_IGNORENAME:break;
            case GAME_COMPARENAME:
                    if(!ut.compstr_WC(now->content.Name,model.Name)) continue;
                    break;
            case GAME_COMPARENAME_NOWILDCARD:
                    if(!ut.compstr(now->content.Name,model.Name)) continue;
                    break;
            case GAME_COMPARENAME_COMPLETELYMATCH:
                    if(strcmp(now->content.Name,model.Name)!=0) continue;
                    break;
        }
        switch(flag&GAME_COMPAREDESC_COMPLETELYMATCH){
            case GAME_IGNOREDESC:break;
            case GAME_COMPAREDESC:
                    if(!ut.compstr_WC(now->content.Description,model.Description)) continue;
                    break;
            case GAME_COMPAREDESC_NOWILDCARD:
                    if(!ut.compstr(now->content.Description,model.Description)) continue;
                    break;
            case GAME_COMPAREDESC_COMPLETELYMATCH:
                    if(strcmp(now->content.Description,model.Description)!=0) continue;
                    break;
        }
        switch(flag&GAME_HOUR_EQUAL){
            case GAME_IGNOREHOUR:break;
            case GAME_HOUR_MORE:
                if(now->content.Hours<model.Hours) continue;
                break;
            case GAME_HOUR_LESS:
                if(now->content.Hours>model.Hours) continue;
                break;
            case GAME_HOUR_EQUAL:
                if(now->content.Hours!=model.Hours) continue;
                break;
        }
        switch(flag&GAME_TOTACHIEVE_EQUAL){
            case GAME_IGNORETOTACHIEVE:break;
            case GAME_TOTACHIEVE_MORE:
                if(now->content.TotAchieve<model.TotAchieve) continue;
                break;
            case GAME_TOTACHIEVE_LESS:
                if(now->content.TotAchieve>model.TotAchieve) continue;
                break;
            case GAME_TOTACHIEVE_EQUAL:
                if(now->content.TotAchieve!=model.TotAchieve) continue;
                break;
        }
        switch(flag&GAME_MYACHIEVE_EQUAL){
            case GAME_IGNOREMYACHIEVE:break;
            case GAME_MYACHIEVE_MORE:
                if(now->content.MyAchieve<model.MyAchieve) continue;
                break;
            case GAME_MYACHIEVE_LESS:
                if(now->content.MyAchieve>model.MyAchieve) continue;
                break;
            case GAME_MYACHIEVE_EQUAL:
                if(now->content.MyAchieve!=model.MyAchieve) continue;
                break;
        }
        return(now);
    }
    return(0);
}
int STList::CountGame(int flag,Game model){
    Game_iterator* now=ghead->next;
    int ans=0;
    while(now){
        now=FindGame(flag,model,now);
        if(now) {++ans;now=now->Next();}
    }
    return(ans);
}
int STList::CountGame_c(int flag,Game model,Company_iterator* comp){
    Game_iterator* now=comp->GameFirst();
    int ans=0;
    while(now){
        now=FindGame_c(flag,model,now);
        if(now) {++ans;now=now->Next_company();}
    }
    return(ans);
}
void STList::CountGame_e(int flag,Game model){
    for(Company_iterator* now=chead->next;now!=chead;now=now->next)
        now->content.MatchedGame=CountGame_c(flag,model,now);
}
Company_iterator* STList::FindCompany(int flag,Company model,Company_iterator *head){
    for(Company_iterator *now=head;now!=chead;now=now->next){
        switch(flag&COMP_COMPARENAME_COMPLETELYMATCH){
            case COMP_IGNORENAME:break;
            case COMP_COMPARENAME:
                    if(!ut.compstr_WC(now->content.Name,model.Name)) continue;
                    break;
            case COMP_COMPARENAME_NOWILDCARD:
                    if(!ut.compstr(now->content.Name,model.Name)) continue;
                    break;
            case COMP_COMPARENAME_COMPLETELYMATCH:
                    if(strcmp(now->content.Name,model.Name)!=0) continue;
                    break;
        }
        switch(flag&COMP_COMPAREDESC_COMPLETELYMATCH){
            case COMP_IGNOREDESC:break;
            case COMP_COMPAREDESC:
                    if(!ut.compstr_WC(now->content.Description,model.Description)) continue;
                    break;
            case COMP_COMPAREDESC_NOWILDCARD:
                    if(!ut.compstr(now->content.Description,model.Description)) continue;
                    break;
            case COMP_COMPAREDESC_COMPLETELYMATCH:
                    if(strcmp(now->content.Description,model.Description)!=0) continue;
                    break;
        }
        switch(flag&COMP_TOTGAME_EQUAL){
            case COMP_IGNORETOTGAME:break;
            case COMP_TOTGAME_MORE:
                if(now->content.TotalGame<model.TotalGame) continue;
                break;
            case COMP_TOTGAME_LESS:
                if(now->content.TotalGame>model.TotalGame) continue;
                break;
            case COMP_TOTGAME_EQUAL:
                if(now->content.TotalGame!=model.TotalGame) continue;
                break;
        }
        switch(flag&COMP_MATCHEDGAME_EQUAL){
            case COMP_IGNOREMATCHEDGAME:break;
            case COMP_MATCHEDGAME_MORE:
                if(now->content.MatchedGame<model.MatchedGame) continue;
                break;
            case COMP_MATCHEDGAME_LESS:
                if(now->content.MatchedGame>model.MatchedGame) continue;
                break;
            case COMP_MATCHEDGAME_EQUAL:
                if(now->content.MatchedGame!=model.MatchedGame) continue;
                break;
        }
        return(now);
    }
    return(0);
}
