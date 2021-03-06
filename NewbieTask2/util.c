#include "util.h"
#include <string.h>
void givestr(char* target,const char* origin)
{
    memcpy(target,origin,sizeof(char)*(strlen(origin)+1));
}
int compstr_WC(const char* origin,const char* target)
{
    int o_len=strlen(origin),t_len=strlen(target);
    int len=0,o_now,t_now=0,ppi,pst;
    char tmp[2000];
    int k_pi[2000];//kmp's pi-array
    o_now=-1;
    while(t_now<t_len)
    {
        if(o_now>=o_len) return(0);//match failed
        len=0;
        while(t_now<t_len&&target[t_now]!='*')
            tmp[++len]=target[t_now++];
        //get string between 2 *
        k_pi[0]=0;
        ppi=0;
        pst=1;
        while(pst<=len)
        {
            while(ppi>0&&tmp[ppi+1]!=tmp[pst]&&tmp[ppi+1]!='?'&&tmp[pst]!='?') ppi=k_pi[ppi];
            if((tmp[ppi+1]==tmp[pst]||tmp[ppi+1]=='?'||tmp[pst]=='?')&&ppi+1!=pst) ++ppi;
            k_pi[pst]=ppi;
            ++pst;
        }//generate pi-aray
        ppi=0;
        while(ppi<len&&o_now+1<o_len)
        {
            while(ppi!=0&&tmp[ppi+1]!=origin[o_now+1]&&tmp[ppi+1]!='?') ppi=k_pi[ppi];
            if(tmp[ppi+1]==origin[o_now+1]||tmp[ppi+1]=='?')++ppi;
            ++o_now;
        }
        if(ppi<len) return(0);//match failed
        ++t_now;//jump the '*'
    }
    return(1);
}
//string compare without wildcard
int compstr(const char* origin,const char* target)
{
    int o_len=strlen(origin),t_len=strlen(target),ppi=-1,pst=0;
    int k_pi[2000];
    while(pst<t_len)
    {
        while(ppi>-1&&target[ppi+1]!=target[pst])
        {
            ppi=k_pi[ppi];
        }
        if(target[ppi+1]==target[pst]&&ppi+1!=pst) ++ppi;
        k_pi[pst]=ppi;
        ++pst;
    }
    ppi=-1;
    pst=-1;
    while(ppi+1<t_len)
    {
        if(pst>=o_len) return(0);
        while(ppi>-1&&target[ppi+1]!=origin[pst+1]) ppi=k_pi[ppi];
        if(target[ppi+1]==origin[pst+1]) ++ppi;
        ++pst;
    }
    return(1);
}
