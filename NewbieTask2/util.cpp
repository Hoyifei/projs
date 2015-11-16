#include "util.h"
#include <string.h>
void Util_Funcs::givestr(char* target,const char* origin){
    memcpy(target,origin,sizeof(char)*(strlen(origin)+1));
}
bool Util_Funcs::compstr_WC(const char* origin,const char* target){
    int o_len=strlen(origin),t_len=strlen(target);
    int len=0,o_now,t_now=0,ppi,pst;
    char tmp[2000];
    int k_pi[2000];//kmp's pi-array
    o_now=-1;
    while(t_now<t_len){
        if(o_now>=o_len) return(0);//match failed
        len=0;
        while(t_now<t_len&&target[t_now]!='*')
            tmp[++len]=target[t_now++];
        //get string between 2 *
        //printf("'%s'\n",tmp+1);
        k_pi[0]=0;
        ppi=0;pst=1;
        while(pst<=len){
            while(ppi>0&&tmp[ppi+1]!=tmp[pst]&&tmp[ppi+1]!='?'&&tmp[pst]!='?') ppi=k_pi[ppi];
            if((tmp[ppi+1]==tmp[pst]||tmp[ppi+1]=='?'||tmp[pst]=='?')&&ppi+1!=pst) ++ppi;
            k_pi[pst]=ppi;
            ++pst;
        }//generate pi-aray
        ppi=0;
        while(ppi<len&&o_now+1<o_len){
            //printf("%d[%c] %d[%c]\n",ppi,tmp[ppi+1],o_now+1,origin[o_now+1]);
            while(ppi!=0&&tmp[ppi+1]!=origin[o_now+1]&&tmp[ppi+1]!='?') ppi=k_pi[ppi];
            if(tmp[ppi+1]==origin[o_now+1]||tmp[ppi+1]=='?')++ppi;
            ++o_now;
        }
        //printf("%d %d\n",ppi,len);
        if(ppi<len) return(0);//match failed
        ++t_now;//jump the '*'
    }
    return(1);
}
//string compare without wildcard
bool Util_Funcs::compstr(const char* origin,const char* target){
    //printf("Compare '%s','%s'\n",origin,target);
    int o_len=strlen(origin),t_len=strlen(target),ppi=-1,pst=0;
    int k_pi[2000];
    while(pst<t_len){
        while(ppi>-1&&target[ppi+1]!=target[pst]) {
            ppi=k_pi[ppi];
        }
        if(target[ppi+1]==target[pst]&&ppi+1!=pst) ++ppi;
        k_pi[pst]=ppi;
        ++pst;
        //printf("(%d %d)",pst,ppi);
        //scanf("%d",&fuck);
    }
    ppi=-1;pst=-1;
    while(ppi+1<t_len){
        //printf("%d %d\n",ppi,t_len);
        //printf("%d[%c] %d[%c]",ppi+1,target[ppi+1],pst+1,origin[pst+1]);
        if(pst>=o_len) return(0);
        while(ppi>-1&&target[ppi+1]!=origin[pst+1]) ppi=k_pi[ppi];
        if(target[ppi+1]==origin[pst+1]) ++ppi;
        ++pst;
    }
    return(1);
}
