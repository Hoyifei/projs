#include <cstdio>
#include <cmath>
#include <complex>
using namespace std;
/*
 * -i :Unsigned Integer only mode.Input should only contain integer numbers,Bin/Oct/Dec/Hex numbers can be mixed.
 *  B100010:binary int 100010;O72510:Oct int 72510;D12345 or 12345:Demical int 12345;HA3FF2:Hex int A3FF2
 * -O X or -o X:Output control.X could be B/O/D/H.Auto regard as Integer-only mode.
 * -r <path>:read input from file
 * -w <path>:write output to file
 * normal:use double as result;% wil be regard as percent
 * -b: Integer only.^ & | is availiable
 * -c :Imaginary number availiable;operator @ #;
 * -h:Show help.
 * -----------------------------------------------------------------------------------------------------------
 * Operators:
 * '+':normal mode could be used as sign
 * '-':normal mode could be used as sign
 * '*'
 * '/'
 * '^':If -bit is enabled,it will be regard as xor operation.Or will be regard as power operation
 * '&':-bit mode only.Operator and;
 * '|':-bit mode only.Operator or;
 * '!':-bit mode ony.Operator not
 * '<':-bit mode only,left shift
 * '>':-bit mode only,right shift
 * '|xxx|':normal mode.Absolute value;
 * '%':-int mode:Operator mod.normal mode:percent
 * '$funcname(':call functions such as sin,cos.e.g.$sin(3.14)
 * '()'
 * '[]':ceil,normal only
 * '{}':floor,normal only
 * '<>':ceil(x+0.5),normal only
 * '@':-img mode only.regard two imgnums as vector and return point product
 * '#':-img mode only.cross product
 * ---------------------------------------------------------------------------------------------------------
 *  sin|cos|tan|sec|csc|asin|acos|atan|sinh|cosh|tanh|
 *  atan2:-img mode only.
 *  sqrt|ln|lg
 *  conj:img mode only
 *  pi is a function:$pi(),expression will be ignored.
 *  last:the last answer.will be cleared as 0 after error or mode changing
 * ---------------------------------------------------------------------------------------------------------
 *  assume that modes will be put in the front and be read from left to right.
 *  uncompatible mods will cause program to exit
 * */
template <typename TY>
struct TSmallStack{
    TY content[10000];
    int top;
    inline void push(TY const& ele){
        content[++top]=ele;
    }
    inline TY pop(){
        --top;return(content[top+1]);
    }
    inline bool empty(){
        return(top<=0);
    }
};
TSmallStack <int> s_op;
TSmallStack <long long> s_int;
TSmallStack <complex <double> > s_cmp;
TSmallStack <double> s_nm;
long long modula;
struct TrieNode{
    int oper;TrieNode* chi[26];
} tpool[150],*op,*now;
int totnode,totfunc;
TrieNode* getnode(){
    ++totnode;
    tpool[totnode].oper=0;
    for(int i=0;i<26;++i) tpool[totnode].chi[i]=op;
    return(&tpool[totnode]);
}
void insword(const char* cont){
    now=op;
    for(int i=0;cont[i];++i){
        if(now->chi[cont[i]-'a']==op) now->chi[cont[i]-'a']=getnode();
        now=now->chi[cont[i]-'a'];     
    }
    now->oper=++totfunc;    
}
char* getint(long long *tar,char* sou){
    *tar=0;long long bas=10;
    char* now=sou;
    while(*now>'9'||*now<'0'){
        switch(*now){
            case 'b':case 'B':bas=2;break;
            case 'o':case 'O':bas=8;break;
            case 'd':case 'D':bas=10;break;
            case 'h':case 'H':bas=16;break;
        }
        ++now;
    }
    while(1){
        switch(*now){
            case '0'...'9':(*tar)=(*tar)*bas+(*now)-'0';break;
            case 'a'...'f':(*tar)=(*tar)*bas+(*now)-'a'+10;break;
            case 'A'...'F':(*tar)=(*tar)*bas+(*now)-'A'+10;break;
            default:return(now);
        }
        ++now;
    }
}
char* getfloat(double *tar,char* sou){
    int ex=0;bool fu=0,step=1;char* now=sou;*tar=0;
    while((*now!='-')&&((*now<'0')||(*now>'9'))) ++now;
    if(*now=='-') {fu=1;++now;}
    while(step==1){
        switch(*now){
            case '0'...'9':*tar=*tar*10+(*now)-'0';++now;break;
            case 'e':case 'E':step=3;++now;break;
            case '.':step=2;++now;break;
            default:step=4;break;
        }
    }
    while(step==2){
        switch(*now){
        }
    }
    while(step==3){
    }
}
int status;
/*1:int mode
 *2:complex enable
 *4:Bin output
 *8:Oct output
 *16:Dec output
 *32:Hex output
*/
int main(int argc,char** argv){
    int argp=0;
    char ch;
    for(;argp<argc;++argp){
        if(argv[argp][0]=='-'){
            switch(argv[argp][1]){
                case 'i':case 'I':
                break;
                case 'c':case 'C':
                break;
                case 'r':case 'R':
                    freopen(argv[argp+1],"r",stdin);
                break;
                case 'w':case 'W':
                    freopen(argv[argp+1],"w",stdout);
                break;
                case 'o':case 'O':
                break;
                case 'h':case 'H':
                    freopen("help.txt","r",stdin);
                    ch=getchar();
                    while(ch!='~'){printf("%c",ch);ch=getchar();}
                    return(0);
                default:break;
            }
        }else break;
    }
    return(0);
}
