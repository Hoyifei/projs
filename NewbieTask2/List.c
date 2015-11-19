#include "List.h"
#include "Game_iterator.h"
#include "Company_iterator.h"
#include "util.h"
#include "Achievements.h"
#include <string.h>
#include <stdlib.h>
#define GETELEM_CODE(TYPE,POOLHEAD,CREATEFUNC,CNT,CIRCSENT) \
        if(source->POOLHEAD==0) CREATEFUNC(ALLOCATE_UNITS,source);\
        if(source->POOLHEAD->next==source->POOLHEAD) CREATEFUNC(ALLOCATE_UNITS,source);\
        TYPE *ans=source->POOLHEAD;\
        source->POOLHEAD->next->prev=source->POOLHEAD->prev;\
        source->POOLHEAD->prev->next=source->POOLHEAD->next;\
        source->POOLHEAD=source->POOLHEAD->next;\
        CIRCSENT\
        --source->CNT;\
        return(ans);
#define CREATEELEM_CODE(TYPE,POOLHEAD,CNT,INIT_SENT1,INIT_SENT2) \
    TYPE* tmp;\
    int i;\
    if(source->POOLHEAD==0)\
    {\
                source->POOLHEAD=(TYPE*)malloc(sizeof(TYPE));\
                source->POOLHEAD->prev=source->POOLHEAD->next=source->POOLHEAD;\
                INIT_SENT1\
            }\
    for(i=0; i<num; ++i)\
    {\
                tmp=(TYPE*)malloc(sizeof(TYPE));\
                tmp->prev=source->POOLHEAD;\
                tmp->next=source->POOLHEAD->next;\
                tmp->prev->next=tmp->next->prev=tmp;\
                INIT_SENT2\
            }\
    source->CNT+=num;
#define DELETEELEM_CODE(TYPE,GLOBAL,CNT,POOLHEAD,ILLEGAL_SENT,CLEAN_SENT) \
        if(tar==0) return;\
    ILLEGAL_SENT\
    T_List *source=GLOBAL;\
    ++source->CNT;\
    tar->next=source->POOLHEAD;\
    tar->prev=source->POOLHEAD->prev;\
    tar->next->prev=tar->prev->next=tar;\
    if(source->CNT>=3*ALLOCATE_UNITS)\
    {\
                TYPE *now=source->POOLHEAD,*head=source->POOLHEAD->prev,*tail=source->POOLHEAD;\
                int i;\
                for(i=1;i<=ALLOCATE_UNITS;++i)\
                {\
                                tail=tail->next;\
                                CLEAN_SENT\
                                free(now);\
                                now=tail;\
                            }\
                head->next=tail;\
                tail->prev=head;\
                source->POOLHEAD=head;\
                source->CNT-=ALLOCATE_UNITS;\
    }
#define CLEANELEM_CODE(TYPE,PNOW,PNEXT,POOLHEAD) \
	TYPE *PNOW,*PNEXT;\
	for(PNOW=source->POOLHEAD->next;PNOW!=source->POOLHEAD;)\
	{\
		PNEXT=PNOW->next;\
		free(PNOW);\
		PNOW=PNEXT;\
	}\
	free(source->POOLHEAD);
#define STRING_COMPARE_CODE(FLAG,STR1,STR2,COND_I,COND_C,COND_CN,COND_CC) \
	switch(FLAG)\
	{\
		case COND_I:\
			break;\
		case COND_C:\
			if(!compstr_WC(STR1,STR2)) continue;\
			break;\
		case COND_CN:\
			if(!compstr(STR1,STR2)) continue;\
			break;\
		case COND_CC:\
			if(strcmp(STR1,STR2)!=0) continue;\
	}
#define INT_COMPARE_CODE(FLAG,INT1,INT2,COND_I,COND_M,COND_L,COND_E) \
	switch(FLAG)\
	{\
		case COND_I:\
			break;\
		case COND_M:\
			if(INT1<INT2) continue;\
			break;\
		case COND_L:\
			if(INT1>INT2) continue;\
			break;\
		case COND_E:\
			if(INT1!=INT2) continue;\
	}
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
    CREATEELEM_CODE(T_Achi,aachi,caachi,;,;)
}
void createcomp(int num,T_List* source)
{
    CREATEELEM_CODE(T_Comp,acomp,cacomp,source->acomp->global=source;init_c(source->acomp);,tmp->global=source;init_c(tmp);)
}
void creategame(int num,T_List* source)
{
    CREATEELEM_CODE(T_Game,agame,cagame,source->agame->global=source;init_g(source->agame);,tmp->global=source;init_g(tmp);)
}
T_Achi *GetAchieve(T_List* source)
{
    GETELEM_CODE(T_Achi,aachi,createachi,caachi,ans->next=ans->prev=ans;)
}
T_Game* GetGameIterator(T_List* source)
{
    GETELEM_CODE(T_Game,agame,creategame,cagame,ans->next=ans->prev=ans->cnext=ans->cprev=ans;)
}
T_Comp* GetCompIterator(T_List* source)
{
    GETELEM_CODE(T_Comp,acomp,createcomp,cacomp,ans->next=ans->prev=ans;)
}
void deleteAchieve(T_Achi* tar)
{
    DELETEELEM_CODE(T_Achi,tar->Game->global,caachi,aachi,if(tar->Game==0||tar->Game->global==0)return;,;)    
}
void deleteGame(T_Game* tar)
{
    DELETEELEM_CODE(T_Game,tar->global,cagame,agame,if(tar->global==0) return;,deleteAchieve(now->head);)
}
void deleteCompany(T_Comp* tar)
{
    DELETEELEM_CODE(T_Comp,tar->global,cacomp,acomp,if(tar->global==0) return;,deleteGame(now->head);)
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
    CLEANELEM_CODE(T_Achi,atmp,anow,aachi)
    CLEANELEM_CODE(T_Game,gtmp,gnow,agame)
    CLEANELEM_CODE(T_Comp,ctmp,cnow,acomp)
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
        STRING_COMPARE_CODE(flag&GAME_COMPARENAME_COMPLETELYMATCH,now->content.Name,model.Name,GAME_IGNORENAME,GAME_COMPARENAME,GAME_COMPARENAME_NOWILDCARD,GAME_COMPARENAME_COMPLETELYMATCH)
        STRING_COMPARE_CODE(flag&GAME_COMPAREDESC_COMPLETELYMATCH,now->content.Description,model.Description,GAME_IGNOREDESC,GAME_COMPAREDESC,GAME_COMPAREDESC_NOWILDCARD,GAME_COMPAREDESC_COMPLETELYMATCH)
        INT_COMPARE_CODE(flag&GAME_HOUR_EQUAL,now->content.Hours,model.Hours,GAME_IGNOREHOUR,GAME_HOUR_MORE,GAME_HOUR_LESS,GAME_HOUR_EQUAL)
        INT_COMPARE_CODE(flag&GAME_TOTACHIEVE_EQUAL,now->content.TotAchieve,model.TotAchieve,GAME_IGNORETOTACHIEVE,GAME_TOTACHIEVE_MORE,GAME_TOTACHIEVE_LESS,GAME_TOTACHIEVE_EQUAL)
        INT_COMPARE_CODE(flag&GAME_MYACHIEVE_EQUAL,now->content.MyAchieve,model.MyAchieve,GAME_IGNOREMYACHIEVE,GAME_MYACHIEVE_MORE,GAME_MYACHIEVE_LESS,GAME_MYACHIEVE_EQUAL)
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
        STRING_COMPARE_CODE(flag&GAME_COMPARENAME_COMPLETELYMATCH,now->content.Name,model.Name,GAME_IGNORENAME,GAME_COMPARENAME,GAME_COMPARENAME_NOWILDCARD,GAME_COMPARENAME_COMPLETELYMATCH)
        STRING_COMPARE_CODE(flag&GAME_COMPAREDESC_COMPLETELYMATCH,now->content.Description,model.Description,GAME_IGNOREDESC,GAME_COMPAREDESC,GAME_COMPAREDESC_NOWILDCARD,GAME_COMPAREDESC_COMPLETELYMATCH)
        INT_COMPARE_CODE(flag&GAME_HOUR_EQUAL,now->content.Hours,model.Hours,GAME_IGNOREHOUR,GAME_HOUR_MORE,GAME_HOUR_LESS,GAME_HOUR_EQUAL)
        INT_COMPARE_CODE(flag&GAME_TOTACHIEVE_EQUAL,now->content.TotAchieve,model.TotAchieve,GAME_IGNORETOTACHIEVE,GAME_TOTACHIEVE_MORE,GAME_TOTACHIEVE_LESS,GAME_TOTACHIEVE_EQUAL)
        INT_COMPARE_CODE(flag&GAME_MYACHIEVE_EQUAL,now->content.MyAchieve,model.MyAchieve,GAME_IGNOREMYACHIEVE,GAME_MYACHIEVE_MORE,GAME_MYACHIEVE_LESS,GAME_MYACHIEVE_EQUAL)
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
        STRING_COMPARE_CODE(flag&COMP_COMPARENAME_COMPLETELYMATCH,now->content.Name,model.Name,COMP_IGNORENAME,COMP_COMPARENAME,COMP_COMPARENAME_NOWILDCARD,COMP_COMPARENAME_COMPLETELYMATCH)
        STRING_COMPARE_CODE(flag&COMP_COMPAREDESC_COMPLETELYMATCH,now->content.Description,model.Description,COMP_IGNOREDESC,COMP_COMPAREDESC,COMP_COMPAREDESC_NOWILDCARD,COMP_COMPAREDESC_COMPLETELYMATCH)
        INT_COMPARE_CODE(flag&COMP_TOTGAME_EQUAL,now->content.TotalGame,model.TotalGame,COMP_IGNORETOTGAME,COMP_TOTGAME_MORE,COMP_TOTGAME_LESS,COMP_TOTGAME_EQUAL)
        INT_COMPARE_CODE(flag&COMP_MATCHEDGAME_EQUAL,now->content.MatchedGame,model.MatchedGame,COMP_IGNOREMATCHEDGAME,COMP_MATCHEDGAME_MORE,COMP_MATCHEDGAME_LESS,COMP_MATCHEDGAME_EQUAL)
        return(now);
    }
    return(0);
}

