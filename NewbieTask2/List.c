#include "List.h"
#include "Game_iterator.h"
#include "Company_iterator.h"
#include "util.h"
#include "Achievements.h"
#include <string.h>
#include <stdlib.h>
T_Game* GameFirst(T_List* source)
{
    return(source->ghead->next);
}
T_Game* GameLast(T_List* source)
{
    return(source->ghead->prev);
}
T_Comp* CompanyFirst(T_List* source)
{
    return(source->chead->next);
}
T_Comp* CompanyLast(T_List* source)
{
    return(source->chead->prev);
}
void addAchievep(T_Achi* val,T_Game *tar)
{
    T_Achi* last=tar->head->prev;
    last->next=val;
    val->prev=last;
    val->next=tar->head;
    tar->head->prev=val;
    ++tar->content.TotAchieve;
    val->Game=tar;
    val->Got=0;
}
T_Achi* addAchievev(const char* name,const char* description,T_Game *tar)
{
    if(tar->global==0) return;
    T_Achi* val=GetAchieve(tar->global);
    int i;
    for(i=0; name[i]; ++i) val->Name[i]=name[i];
    val->Name[i]=0;
    for(i=0; description[i]; ++i) val->Description[i]=description[i];
    val->Description[i]=0;
    addAchievep(val,tar);
    return(val);
}

void Get_a(T_Achi* tar)
{
    if(tar->Got==1) return;
    ++tar->Game->content.MyAchieve;
    tar->Got=1;
}
void Clear_a(T_Achi* tar)
{
    if(tar->Got==0) return;
    tar->Got=0;
    --tar->Game->content.MyAchieve;
}
void createachi(int num,T_List* source)
{
    T_Achi* tmp;
    int i;
    if(source->aachi==0)
    {
        source->aachi=(T_Achi*)malloc(sizeof(T_Achi));
        source->aachi->prev=source->aachi->next=source->aachi;
    }
    for(i=0; i<num; ++i)
    {
        tmp=(T_Achi*)malloc(sizeof(T_Achi));
        tmp->prev=source->aachi;
        tmp->next=source->aachi->next;
        tmp->prev->next=tmp->next->prev=tmp;
    }
    source->caachi+=num;
}
void createcomp(int num,T_List* source)
{
    T_Comp* tmp;
    int i;
    if(source->acomp==0)
    {
        source->acomp=(T_Comp*)malloc(sizeof(T_Comp));
        source->acomp->prev=source->acomp->next=source->acomp;
        source->acomp->global=source;
        init_c(source->acomp);
    }
    for(i=0; i<num; ++i)
    {
        tmp=(T_Comp*)malloc(sizeof(T_Comp));
        tmp->prev=source->acomp;
        tmp->next=source->acomp->next;
        tmp->prev->next=tmp->next->prev=tmp;
        tmp->global=source;
        init_c(tmp);
    }
    source->cacomp+=num;
}
void creategame(int num,T_List* source)
{
    T_Game* tmp;
    int i;
    if(source->agame==0)
    {
        source->agame=(T_Game*)malloc(sizeof(T_Game));
        source->agame->next=source->agame->prev=source->agame;
        source->agame->global=source;
        init_g(source->agame);
    }
    for(i=0; i<num; ++i)
    {
        tmp=(T_Game*)malloc(sizeof(T_Game));
        tmp->prev=source->agame;
        tmp->next=source->agame->next;
        tmp->prev->next=tmp->next->prev=tmp;
        tmp->global=source;
        init_g(tmp);
    }
    source->cagame+=num;
}
T_Achi *GetAchieve(T_List *source)
{
    if(source->aachi==0) createachi(ALLOCATE_UNITS,source);
    if(source->aachi->next==source->aachi) createachi(ALLOCATE_UNITS,source);
    T_Achi* ans=source->aachi;
    source->aachi->next->prev=source->aachi->prev;
    source->aachi->prev->next=source->aachi->next;
    source->aachi=source->aachi->next;
    ans->next=ans->prev=ans;
    --source->caachi;
    return(ans);
}
T_Game* GetGameIterator(T_List* source)
{
    if(source->agame==0) creategame(ALLOCATE_UNITS,source);
    if(source->agame->next==source->agame) creategame(ALLOCATE_UNITS,source);
    T_Game* ans=source->agame;
    source->agame->next->prev=source->agame->prev;
    source->agame->prev->next=source->agame->next;
    source->agame=source->agame->next;
    ans->next=ans->prev=ans->cnext=ans->cprev=ans;
    --source->cagame;
    return(ans);
}
T_Comp* GetCompIterator(T_List* source)
{
    if(source->acomp==0) createcomp(ALLOCATE_UNITS,source);
    if(source->acomp->next==source->acomp) createcomp(ALLOCATE_UNITS,source);
    T_Comp* ans=source->acomp;
    source->acomp->next->prev=source->acomp->prev;
    source->acomp->prev->next=source->acomp->next;
    source->acomp=source->acomp->next;
    --source->cacomp;
    return(ans);
}
void deleteAchieve(T_Achi* tar)
{
    if(tar==0) return;
    if(tar->Game==0||tar->Game->global==0) return;
    T_List *source=tar->Game->global;
    ++source->caachi;
    tar->next=source->aachi;
    tar->prev=source->aachi->prev;
    source->aachi->prev->next=tar;
    source->aachi->prev=tar;
    source->aachi=tar;
    if(source->caachi>=3*ALLOCATE_UNITS)
    {
        int i;
        T_Achi *now=source->aachi,*head=source->aachi->prev,*tail=source->aachi;
        for(i=1; i<=ALLOCATE_UNITS; ++i)
        {
            tail=tail->next;
            free(now);
            now=tail;
        }
        head->next=tail;
        tail->prev=head;
        source->aachi=head;
        source->caachi-=ALLOCATE_UNITS;
    }
}
void deleteGame(T_Game* tar)
{
    if(tar==0) return;
    if(tar->global==0) return;
    T_List *source=tar->global;
    ++source->cagame;
    tar->next=source->agame;
    tar->prev=source->agame->prev;
    source->agame->prev->next=tar;
    source->agame->prev=tar;
    if(source->cagame>=3*ALLOCATE_UNITS)
    {
        T_Game *now=source->agame,*head=source->agame->prev,*tail=source->agame;
        int i;
        for(i=1; i<=ALLOCATE_UNITS; ++i)
        {
            tail=tail->next;
            deleteAchieve(now->head);
            free(now);
            now=tail;
        }
        head->next=tail;
        tail->prev=head;
        source->agame=head;
        source->cagame-=ALLOCATE_UNITS;
    }
}
void deleteCompany(T_Comp* tar)
{
    if(tar==0) return;
    if(tar->global==0) return;
    T_List *source=tar->global;
    ++source->cacomp;
    tar->next=source->acomp;
    tar->prev=source->acomp->prev;
    source->acomp->prev->next=tar;
    source->acomp->prev=tar;
    if(source->cacomp>=3*ALLOCATE_UNITS)
    {
        int i;
        T_Comp *now=source->acomp,*head=source->acomp->prev,*tail=source->acomp;
        for(i=1; i<=ALLOCATE_UNITS; ++i)
        {
            tail=tail->next;
            deleteGame(now->head);
            free(now);
            now=tail;
        }
        head->next=tail;
        tail->prev=head;
        source->acomp=head;
        source->cacomp-=ALLOCATE_UNITS;
    }
}
T_Achi* DelAchieve(T_Achi* tar)
{
    if(tar==0) return(0);
    T_Achi *ans=tar->next;
    if(tar->next){
    	tar->next->prev=tar->prev;
    	tar->prev->next=tar->next;
    }
    deleteAchieve(tar);
    return(ans);
}
T_Game* DelGame(T_Game* tar)
{
    if(tar==0) return(0);
    T_Game *ans=tar->cnext;
    if(tar->next){
        tar->prev->next=tar->next;
        tar->next->prev=tar->prev;
    }
    if(tar->cnext){
        tar->cnext->cprev=tar->cprev;
        tar->cprev->cnext=tar->cnext;
    }
    T_Achi* now;
    for(now=tar->head->next; now!=tar->head; now=DelAchieve(now));
    deleteGame(tar);
    return(ans);
}
T_Comp* DelCompany(T_Comp* tar)
{
    if(tar==0) return(0);
    T_Comp *ans=tar->next;
    if(tar->next){
        tar->prev->next=tar->next;
        tar->next->prev=tar->prev;
    }
    T_Game* now;
    for(now=tar->head->cnext; now!=tar->head; now=DelGame(now));
    deleteCompany(tar);
    return(ans);
}
void init(T_List* source)
{
    source->chead=GetCompIterator(source);
    source->chead->prev=source->chead->next=source->chead;
    source->ghead=GetGameIterator(source);
    source->ghead->prev=source->ghead->next=source->ghead;
}
void clean_up(T_List* source)
{
    T_Comp* now;
    for(now=source->chead->next; now!=source->chead; now=DelCompany(now));
    DelCompany(source->chead);
    DelGame(source->ghead);
    T_Achi *atmp,*anow;
    for(anow=source->aachi->next; anow!=source->aachi;)
    {
        atmp=anow->next;
        free(anow);
        anow=atmp;
    }
    free(source->aachi);
    T_Game *gtmp,*gnow;
    for(gnow=source->agame->next; gnow!=source->agame;)
    {
        gtmp=gnow->next;
        free(gnow);
        gnow=gtmp;
    }
    free(source->agame);
    T_Comp *ctmp,*cnow;
    for(cnow=source->acomp->next; cnow!=source->acomp;)
    {
        ctmp=cnow->next;
        free(cnow);
        cnow=ctmp;
    }
    free(source->acomp);
}
void qsort_game(T_Game* l,T_Game* r,int ( * cmp )(T_Game* a,T_Game* b))
{
    T_Game *ll=l,*rr=r,*x=l,*gne,*gpr;
    int itmp=1,tot=0;
    for(; x->next!=r; x=x->next)
        ++tot;
    ++tot;
    for(x=l; itmp<=(tot>>1); ++itmp) x=x->next;
    while(!(ll->order>rr->order))
    {
        while((*cmp)(ll,x)) ll=ll->next;
        while((*cmp)(x,rr)) rr=rr->prev;
        if(!(ll->order>rr->order))
        {
            gne=ll->next;
            gpr=rr->prev;
            if(ll->next!=rr)
            {
                ll->next=rr->next;
                rr->next=gne;
                rr->prev=ll->prev;
                ll->prev=gpr;
                itmp=ll->order;
                ll->order=rr->order;
                rr->order=itmp;
                ll->next->prev=ll->prev->next=ll;
                rr->next->prev=rr->prev->next=rr;
                ll=gne;
                rr=gpr;
            }
            else
            {
                ll->next=rr->next;
                rr->prev=ll->prev;
                ll->prev=rr;
                rr->next=ll;
                itmp=ll->order;
                ll->order=rr->order;
                rr->order=itmp;
                ll->next->prev=ll->prev->next=ll;
                rr->next->prev=rr->prev->next=rr;
            }

        }
    }
    if(l->order<rr->order) qsort_game(l,rr,cmp);
    if(ll->order<r->order) qsort_game(ll,r,cmp);
}
void qsort_game_company(T_Game* l,T_Game* r,int ( * cmp )(T_Game* a,T_Game* b))
{
    T_Game *ll=l,*rr=r,*x=l,*gne,*gpr,*gtmp;
    int itmp=0,tot=0;
    for(; x->cnext!=r; x=x->cnext) ++tot;
    ++tot;
    for(x=l; itmp<=(tot>>1); ++itmp) x=x->cnext;
    while(!(ll->order>rr->order))
    {
        while((*cmp)(ll,x)) ll=ll->next;
        while((*cmp)(x,rr)) rr=rr->prev;
        if(!(ll->order>rr->order))
        {
            gne=ll->cnext;
            gpr=rr->cprev;
            if(ll->cnext!=rr)
            {
                ll->cnext=rr->cnext;
                rr->cnext=gne;
                rr->cprev=ll->cprev;
                ll->cprev=gpr;
            }
            else
            {
                ll->cnext=rr->cnext;
                rr->cprev=ll->cprev;
                ll->cprev=rr;
                rr->cnext=ll;
            }
            if(ll->next!=rr)
            {
                gtmp=ll->next;
                ll->next=rr->next;
                rr->next=gtmp;
                gtmp=ll->prev;
                ll->prev=rr->prev;
                rr->prev=gtmp;
                itmp=ll->order;
                ll->order=rr->order;
                rr->order=itmp;
                ll->cprev->cnext=ll->prev->next=ll->next->prev=ll->cnext->cprev=ll;
                rr->cprev->cnext=rr->prev->next=rr->next->prev=rr->cnext->cprev=rr;
                ll=gne;
                rr=gpr;
            }
            else
            {
                ll->next=rr->next;
                rr->prev=ll->prev;
                ll->prev=rr;
                rr->next=ll;
                itmp=ll->order;
                ll->order=rr->order;
                rr->order=itmp;
                ll->cprev->cnext=ll->prev->next=ll->next->prev=ll->cnext->cprev=ll;
                rr->cprev->cnext=rr->prev->next=rr->next->prev=rr->cnext->cprev=rr;
            }
        }
    }
    if(l->order<rr->order) qsort_game_company(l,rr,cmp);
    if(ll->order<r->order) qsort_game_company(ll,r,cmp);
}
void qsort_company(T_Comp* l,T_Comp* r,int ( * cmp )(T_Comp* a,T_Comp* b))
{
    T_Comp *ll=l,*rr=r,*x=l,*cne,*cpr;
    int itmp=0,tot=0;
    for(; x->next!=r; x=x->next) ++tot;
    ++tot;
    for(x=l; itmp<=(tot>>1); ++itmp) x=x->next;
    while(!(ll->order>rr->order))
    {
        while((*cmp)(ll,x)) ll=ll->next;
        while((*cmp)(x,rr)) rr=rr->prev;
        if(!(ll->order>rr->order))
        {
            if(ll->next!=rr)
            {
                cne=ll->next;
                ll->next=rr->next;
                rr->next=cne;
                cpr=rr->prev;
                rr->prev=ll->prev;
                ll->prev=cpr;
                itmp=ll->order;
                ll->order=rr->order;
                rr->order=itmp;
                ll->prev->next=ll->next->prev=ll;
                rr->prev->next=rr->next->prev=rr;
                ll=cne;
                rr=cpr;
            }
            else
            {
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
void InsertGame_p(T_Game *fr,T_Game *tar)
{
    if(fr->global==0) return;
    if(fr->comp==0) return;
    if(fr==fr->comp->head)
    {
        tar->prev=GameLast(fr->global);
        tar->next=fr->global->ghead;
        tar->cprev=fr->cprev;
        tar->cnext=fr;
    }
    else
    {
        tar->prev=fr;
        tar->next=fr->next;
        tar->cprev=fr;
        tar->cnext=fr->cnext;
    }
    tar->next->prev=tar->cnext->cprev=tar->prev->next=tar->cprev->cnext=tar;
    tar->comp=fr->comp;
    fr->comp->content.TotalGame++;
    tar->global=fr->global;
}
T_Game* InsertGame(T_Game *fr,struct Game content)
{
    if(fr->global==0) return(0);
    if(fr->comp==0) return(0);
    T_Game *tar=GetGameIterator(fr->global);
    givestr(tar->content.Name,content.Name);
    givestr(tar->content.Description,content.Description);
    InsertGame_p(fr,tar);
    return(tar);
}
void SortGame(T_Game* l,T_Game* r,int ( * cmp )(T_Game* a,T_Game* b))
{
    int cnt=0;
    if(l->global!=r->global) return;
    T_List* source=l->global;
    if(source==0) return;
    T_Game* gnow;
    T_Comp* cnow;
    for(gnow=source->ghead->next; gnow!=source->ghead; gnow=gnow->next)
        gnow->order=++cnt;
    if(l->order>r->order) qsort_game(r,l,cmp);
    else qsort_game(l,r,cmp);
    for(cnow=source->chead->next; cnow!=source->chead; cnow=cnow->next)
        cnow->head->cnext=cnow->head->cprev=cnow->head;
    for(gnow=source->ghead->next; gnow!=source->ghead; gnow=gnow->next)
    {
        T_Game *fr=gnow->comp->head->prev;
        gnow->cprev=fr;
        gnow->cnext=fr->cnext;
        fr->cnext=gnow;
        gnow->cnext->cprev=gnow;
    }
}
void SortGame_Company(T_Game* l,T_Game* r,int ( * cmp )(T_Game* a,T_Game* b))
{
    if(l->comp!=r->comp||l->comp==0) return;
    int cnt=0;
    T_Game *he=l->comp->head,*now;
    for(now=he->cnext; now!=he; now=now->cnext)
        now->order=++cnt;
    if(l->order<r->order) qsort_game_company(l,r,cmp);
    else qsort_game_company(r,l,cmp);
}
void SortCompany(T_Comp* l,T_Comp* r,int( * cmp )(T_Comp* a,T_Comp* b))
{
    int cnt=0;
    if(l->global!=r->global) return;
    T_List *source=l->global;
    if(source==0) return;
    T_Comp *now;
    for(now=source->chead->next; now!=source->chead; now=now->next) now->order=++cnt;
    if(l->order<r->order) qsort_company(l,r,cmp);
    else qsort_company(r,l,cmp);
}
void InsertCompany_p(T_Comp* fr,T_Comp* tar)
{
    if(fr->global==0) return;
    tar->next=fr->next;
    tar->prev=fr;
    fr->next=tar;
    tar->next->prev=tar;
    tar->global=fr->global;
}
T_Comp* InsertCompany(T_Comp* fr,struct Company content)
{
    if(fr->global==0) return(0);
    T_Comp *tar=GetCompIterator(fr->global);
    tar->content=content;
    InsertCompany_p(fr,tar);
    return(tar);
}
T_Game* FindGame(int flag,struct Game model,T_Game *head)
{
    T_Game *now;
    if(head->global==0) return;
    for(now=head; now!=head->global->ghead; now=now->next)
    {
        switch(flag&GAME_COMPARENAME_COMPLETELYMATCH)
        {
        case GAME_IGNORENAME:
            break;
        case GAME_COMPARENAME:
            if(!compstr_WC(now->content.Name,model.Name)) continue;
            break;
        case GAME_COMPARENAME_NOWILDCARD:
            if(!compstr(now->content.Name,model.Name)) continue;
            break;
        case GAME_COMPARENAME_COMPLETELYMATCH:
            if(strcmp(now->content.Name,model.Name)!=0) continue;
            break;
        }
        switch(flag&GAME_COMPAREDESC_COMPLETELYMATCH)
        {
        case GAME_IGNOREDESC:
            break;
        case GAME_COMPAREDESC:
            if(!compstr_WC(now->content.Description,model.Description)) continue;
            break;
        case GAME_COMPAREDESC_NOWILDCARD:
            if(!compstr(now->content.Description,model.Description)) continue;
            break;
        case GAME_COMPAREDESC_COMPLETELYMATCH:
            if(strcmp(now->content.Description,model.Description)!=0) continue;
            break;
        }
        switch(flag&GAME_HOUR_EQUAL)
        {
        case GAME_IGNOREHOUR:
            break;
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
        switch(flag&GAME_TOTACHIEVE_EQUAL)
        {
        case GAME_IGNORETOTACHIEVE:
            break;
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
        switch(flag&GAME_MYACHIEVE_EQUAL)
        {
        case GAME_IGNOREMYACHIEVE:
            break;
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
T_Game* FindGame_c(int flag,struct Game model,T_Game *head)
{
    if(head->comp==0) return;
    T_Game *he=head->comp->head,*now;
    for(now=head; now!=he; now=now->cnext)
    {
        switch(flag&GAME_COMPARENAME_COMPLETELYMATCH)
        {
        case GAME_IGNORENAME:
            break;
        case GAME_COMPARENAME:
            if(!compstr_WC(now->content.Name,model.Name)) continue;
            break;
        case GAME_COMPARENAME_NOWILDCARD:
            if(!compstr(now->content.Name,model.Name)) continue;
            break;
        case GAME_COMPARENAME_COMPLETELYMATCH:
            if(strcmp(now->content.Name,model.Name)!=0) continue;
            break;
        }
        switch(flag&GAME_COMPAREDESC_COMPLETELYMATCH)
        {
        case GAME_IGNOREDESC:
            break;
        case GAME_COMPAREDESC:
            if(!compstr_WC(now->content.Description,model.Description)) continue;
            break;
        case GAME_COMPAREDESC_NOWILDCARD:
            if(!compstr(now->content.Description,model.Description)) continue;
            break;
        case GAME_COMPAREDESC_COMPLETELYMATCH:
            if(strcmp(now->content.Description,model.Description)!=0) continue;
            break;
        }
        switch(flag&GAME_HOUR_EQUAL)
        {
        case GAME_IGNOREHOUR:
            break;
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
        switch(flag&GAME_TOTACHIEVE_EQUAL)
        {
        case GAME_IGNORETOTACHIEVE:
            break;
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
        switch(flag&GAME_MYACHIEVE_EQUAL)
        {
        case GAME_IGNOREMYACHIEVE:
            break;
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
int CountGame(int flag,struct Game model,T_List* source)
{
    T_Game* now=source->ghead->next;
    int ans=0;
    while(now)
    {
        now=FindGame(flag,model,now);
        if(now)
        {
            ++ans;
            now=Next_g(now);
        }
    }
    return(ans);
}
int CountGame_c(int flag,struct Game model,T_Comp* comp)
{
    T_Game* now=GameFirst_c(comp);
    int ans=0;
    while(now)
    {
        now=FindGame_c(flag,model,now);
        if(now)
        {
            ++ans;
            now=Next_company(now);
        }
    }
    return(ans);
}
void CountGame_e(int flag,struct Game model,T_List* source)
{
    T_Comp *now;
    for(now=source->chead->next; now!=source->chead; now=now->next)
        now->content.MatchedGame=CountGame_c(flag,model,now);
}
T_Comp* FindCompany(int flag,struct Company model,T_Comp *head)
{
    T_Comp *now;
    if(head->global==0) return(0);
    for(now=head; now!=head->global->chead; now=now->next)
    {
        switch(flag&COMP_COMPARENAME_COMPLETELYMATCH)
        {
        case COMP_IGNORENAME:
            break;
        case COMP_COMPARENAME:
            if(!compstr_WC(now->content.Name,model.Name)) continue;
            break;
        case COMP_COMPARENAME_NOWILDCARD:
            if(!compstr(now->content.Name,model.Name)) continue;
            break;
        case COMP_COMPARENAME_COMPLETELYMATCH:
            if(strcmp(now->content.Name,model.Name)!=0) continue;
            break;
        }
        switch(flag&COMP_COMPAREDESC_COMPLETELYMATCH)
        {
        case COMP_IGNOREDESC:
            break;
        case COMP_COMPAREDESC:
            if(!compstr_WC(now->content.Description,model.Description)) continue;
            break;
        case COMP_COMPAREDESC_NOWILDCARD:
            if(!compstr(now->content.Description,model.Description)) continue;
            break;
        case COMP_COMPAREDESC_COMPLETELYMATCH:
            if(strcmp(now->content.Description,model.Description)!=0) continue;
            break;
        }
        switch(flag&COMP_TOTGAME_EQUAL)
        {
        case COMP_IGNORETOTGAME:
            break;
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
        switch(flag&COMP_MATCHEDGAME_EQUAL)
        {
        case COMP_IGNOREMATCHEDGAME:
            break;
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

