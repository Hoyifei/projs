#include "List.h"
#include "Game.h"
#include "Game_iterator.h"
#include "Company.h"
#include "Company_iterator.h"
#include <stdlib.h>
void STList::createachi(int num){
    int n=num;
    Achieve** nachis=(Achieve**)malloc(n*sizeof(Achieve));
    if(aachi==0){
        --n;aachi=nachis[n];aachi->next=aachi->prev=aachi;
    }
    for(int i=n-1;i>=0;--i){
        nachis[i]->next=aachi;
        nachis[i]->prev=aachi->prev;
        nachis[i]->prev->next=nachis[i];
        aachi->prev=nachis[i];
        aachi=nachis[i];
    }
    caachi+=n;
}
void STList::createcomp(int num){
    int n=num;
    Company_iterator** ncomp=(Company_iterator**)malloc(n*sizeof(Company_iterator));
    if(acomp==0){
        --n;acomp=ncomp[n];acomp->next=acomp->prev=acomp;
    }
    for(int i=n-1;i>=0;--i){
        ncomp[i]->next=acomp;
        ncomp[i]->prev=acomp->prev;
        ncomp[i]->prev->next=ncomp[i];
        acomp->prev=ncomp[i];
        acomp=ncomp[i];
    }
    cacomp+=n;
}
void STList::creategame(int num){
    int n=num;
    Game_iterator** ngame=(Game_iterator**)malloc(n*sizeof(Game_iterator));
    if(agame==0){
        --n;agame=ngame[n];agame->next=agame->prev=agame;
    }
    for(int i=n-1;i>=0;--i){
        ngame[i]->next=agame;
        ngame[i]->prev=agame->prev;
        ngame[i]->prev->next=ngame[i];
        agame->prev=ngame[i];
        agame=ngame[i];
    }
    cagame+=n;
}
Achieve* STList::GetAchieve(){
    if(aachi->next==aachi) createachi(ALLOCATE_UNITS);
    Achieve* ans=aachi;
    aachi->next->prev=aachi->prev;
    aachi->prev->next=aachi->next;
    aachi=aachi->next;
    ans->next=ans->prev=ans;
    return(ans);
}
Game_iterator* STList::GetGameIterator(){
    if(agame->next==agame) creategame(ALLOCATE_UNITS);
    Game_iterator* ans=agame;
    agame->next->prev=agame->prev;
    agame->prev->next=agame->next;
    agame=agame->next;
    ans->content->Head=GetAchieve();
    ans->next=ans->prev=ans;
    return(ans);
}
Company_iterator* STList::GetCompIterator(){
    if(acomp->next==acomp) createcomp(ALLOCATE_UNITS);
    Company_iterator* ans=acomp;
    acomp->next->prev=acomp->prev;
    acomp->prev->next=acomp->next;
    acomp=acomp->next;
    ans->head=GetGameIterator();
    return(ans);
}
void STList::delAchieve(Achieve* tar){
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
void STList::delGame(Game_iterator* tar){
    delAchieve(tar->content->Head);
    tar->content->Head=0;
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
void STList::delCompany(Company_iterator* tar){
    delGame(tar->head);
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
void STList::qsort_game(Game_iterator* l,Game_iterator* r,bool ( * cmp )(Game_iterator* a,Game_iterator* b)){
    Game_iterator *ll=l,*rr=r,*x=l,*gne,*gpr;
    int itmp=0,tot=0;
    for(;x->next!=r;x=x->next) ++tot;
    ++tot;
    for(x=l;itmp<=(tot>>1);++itmp) x=x->next;
    while(!(ll->order>rr->order)){
        while(*cmp(ll,x)) ll=ll->next;
        while(*cmp(x,rr)) rr=rr->prev;
        if(!(ll->order>rr->order)){
            gne=ll->next;ll->next=rr->next;rr->next=gne;
            gpr=rr->prev;rr->prev=ll->prev;ll->prev=gpr;
            itmp=ll->order;ll->order=rr->order;rr->order=itmp;
            ll->next->prev=ll->prev->next=ll;
            rr->next->prev=rr->prev->next=rr;
            ll=gne;
            rr=gpr;
        }
    }
    if(l->order<rr->order) qsort_game(l,rr,cmp);
    if(ll->order<r->order) qsort_game(ll,r,cmp);
}
void STList::qsort_game_company(Game_iterator* l,Game_iterator* r,bool ( * cmp )(Game_iterator* a,Game_iterator* b)){
    Game_iterator *ll=l,*rr=r,*x=l,*gne,*gpr;
    int itmp=0,tot=0;
    for(;x->cnext!=r;x=x->cnext) ++tot;
    ++tot;
    for(x=l;itmp<=(tot>>1);++itmp) x=x->cnext;
    while(!(ll->order>rr->order)){
        while(*cmp(ll,x)) ll=ll->next;
        while(*cmp(x,rr)) rr=rr->prev;
        if(!(ll->order>rr->order)){
            gne=ll->cnext;ll->cnext=rr->cnext;rr->cnext=gne;
            gpr=rr->cprev;rr->cprev=ll->cprev;ll->cprev=gpr;
            itmp=ll->order;ll->order=rr->order;rr->order=itmp;
            gtmp=ll->next=ll->next=rr->next;rr->next=gtmp;
            gtmp=ll->prev;ll->prev=rr->prev;rr->prev=gtmp;
            ll->cprev->cnext=ll->prev->next=ll->next->prev=ll->cnext->cprev=ll;
            rr->cprev->cnext=rr->prev->next=rr->next->prev=rr->cnext->cprev=rr;
            ll=gne;
            rr=gpr;
        }
    }
    if(l->order<rr->order) qsort_game_company(l,rr,cmp);
    if(ll->order<r->order) qsort_game_company(ll,r,cmp);
}
void STList::qsort_company(Company_iterator* l,Company_iterator* r,bool ( * cmp )(Company_iterator* a,Company_iterator* b)){
    Company_iterator *ll=l,*rr=r,*x=l,*gne,*gpr;
    int itmp=0,tot=0;
    for(;x->next!=r;x=x->next) ++tot;
    ++tot;
    for(x=l;itmp<=(tot>>1);++itmp) x=x->next;
    while(!(ll->order>rr->order)){
        while(*cmp(ll,x)) ll=ll->next;
        while(*cmp(x,rr)) rr=rr->prev;
        if(!(ll->order>rr->order)){
            gne=ll->cnext;ll->cnext=rr->cnext;rr->cnext=gne;
            gpr=rr->cprev;rr->cprev=ll->cprev;ll->cprev=gpr;
            itmp=ll->order;ll->order=rr->order;rr->order=itmp;
            gtmp=ll->next=ll->next=rr->next;rr->next=gtmp;
            gtmp=ll->prev;ll->prev=rr->prev;rr->prev=gtmp;
            ll->cprev->cnext=ll->prev->next=ll->next->prev=ll->cnext->cprev=ll;
            rr->cprev->cnext=rr->prev->next=rr->next->prev=rr->cnext->cprev=rr;
            ll=gne;
            rr=gpr;
        }
    }
    if(l->order<rr->order) qsort_company(l,rr,cmp);
    if(ll->order<r->order) qsort_company(ll,r,cmp);
}
void STList::InsertGame_p(Game_iterator *fr,Game_iterator *tar){
    tar->prev=fr;tar->cprev=fr->cprev;
    tar->next=fr->next;tar>cnext=fr->cnext;
    tar->next->prev=tar->cnext->cprev=tar;
    fr->next=fr->cnext=tar;
    tar->comp=fr->comp;
}
void STList::InsertGame_v(Game_iterator *fr,Game content){
    Game_Iterator *tar=GetGameIterator();
    tar->content=content;
    InsertGame_p(fr,tar);
}
void STList::SortGame(Game_iterator* l,Game_iterator* r,bool ( * cmp )(Game_iterator* a,Game_iterator* b))
{
    int cnt=0;
    for(Game_iterator *now=ghead->next;now!=ghead;now=now->next) now->order=++cnt;
    if(l->order>r->order) qsort_game(r,l,cmp);else qsort_game(l,r,cmp);
    for(Company_iterator *now=chead->next;now!=chead;now=now->next)
        now->head->cnext=how->head->cprev=now->head;
    for(Game_Iterator *now=ghead->next;now!=ghead;now=now->next){
        Game_Iterator *fr=now->comp->head->prev;
        tar->cprev=fr;
        tar->cnext=fr->cnext;
        fr->cnext=tar;
        tar->cnext->cprev=tar;
    }
}
void STList::SortGame_Company(Game_iterator* l,Game_iterator* r,bool ( * cmp )(Game_iterator* a,Game_iterator* b))
{
    if(l->comp!=r->comp) return;
    int cnt=0;
    Game_iterator *he=l->comp->head;
    for(Game_iterator *now=he->cnext;now!=he;now=now->cnext) now->order=++cnt;
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
void STList::InsertCompany(Company_iterator* fr,Company content){
    Company_iterator *tar=GetCompany();
    tar->content=content;
    InsertCompany_p(fr,tar);
}

