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
 * ---------------------------------------------------------------------------------------------------------
 *  sin|cos|tan|sec|csc|asin|acos|atan|sinh|cosh|tanh|
 *  atan2:-img mode only.
 *  sqrt|ln|lg
 *  conj:img mode only
 *  pi is a function:$pi(),expression will be ignored.
 *  ---fuctions not availiable in int-only mode---
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
TSmallStack <unsigned long long> s_int;
TSmallStack <complex <double> > s_cmp;
TSmallStack <double> s_nrm;
long long modula;
struct TrieNode{
    int oper;TrieNode* chi[26];
} tpool[150],*op;
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
char* getint(unsigned long long *tar,char* sou){
    *tar=0;unsigned long long bas=10;
    char* now=sou;
    while(*now>'9'||*now<'0'){
   //     printf("1now=%c\n",*now);
        switch(*now){
            case 'b':case 'B':bas=2;break;
            case 'o':case 'O':bas=8;break;
            case 'd':case 'D':bas=10;break;
            case 'h':case 'H':bas=16;break;
        }
        ++now;
    }
    while(1){
 //       printf("now=%c\n",*now);
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
    int ex=0,fuck=0,step=1;bool fu=0,ck=0;char* now=sou;*tar=0;
    int deb;
    while((*now!='-')&&((*now<'0')||(*now>'9'))) ++now;
    if(*now=='-') {fu=1;++now;}
    while(step==1){
        //printf("2now=%c\n",*now);
        //printf("step=%d (step==1)=%d\n",step,step==1);
        //scanf("%d",&deb);
        //printf("now=='.'=%d\n",(*now)=='.');
        switch(*now){
            case '0'...'9':*tar=*tar*10+(*now)-'0';++now;break;
            case 'e':case 'E':step=3;++now;break;
            case '.':
                //printf("xxx");
                step=2;
                ++now;
                //printf("saatep=%d\n",step);
                break;
            default:step=4;break;
        }
    }
    while(step==2){
      //  printf("3now=%c\n",*now);
        switch(*now){
            case '0'...'9':*tar=*tar*10+(*now)-'0';++now;--ex;break;
            case 'e':case 'E':step=3;++now;break;
            default:step=4;break;
        }
    }
    while(step==3){
     //   printf("4now=%c\n",*now);
        switch(*now){
            case '0'...'9':fuck=fuck*10+(*now)-'0';++now;break;
            case '-':ck=1;++now;break;
            default:step=4;break;
        }
    }
    if(ck==1) fuck=-fuck;
    ex+=fuck;
    if(fu==1) *tar=-(*tar);
    *tar*=pow(10,ex);
    return(now);
}
int status;
/*1:int mode
 *2:complex enable
 *4:Bin output
 *8:Oct output
 *16:Hex output
*/
void cal_i(){
    try{
        unsigned long long a,b;int op;
        a=s_int.pop();op=s_op.pop();b=s_int.pop();
        switch(op){
            case 1://'+'
                s_int.push(a+b);
            break;
            case 2://'-'
                s_int.pusn(b-a);
            break;
            case 3://'*'
                s_int.push(a*b);
            break;
            case 4://'/'
                s_int.push(b/a);
            break;
            case 5://'%'
                s_int.push(b%a);
            break;
            case 6://'<'
                s_int.push(b<<a);
            break;
            case 7://'>'
                s_int.push(b>>a);
            break;;
            case 8://'&'
                s_int.push(b&a);
            break;
            case 9://'|'
                s_int.push(b|a);
            break;
            case 10://'^'
                s_int.push(a^b);
            break;
            case 11://'!'
                s_int.push(b);
                s_int.push(!a);
            break;
        }
    }catch(...){
        throw;
    }
}
void cal_n(){
    try{
        double a,b;int op;
        a=s_nrm.pop();op=s_op.pop();b=s_nrm.pop();
        switch(op){
            case 1://'+'
                s_nrm.push(a+b);
            break;
            case 2://'-'
                s_nrm.pusn(b-a);
            break;
            case 3://'*'
                s_nrm.push(a*b);
            break;
            case 4://'/'
                s_nrm.push(b/a);
            break;
            case 5://'^'
                s_nrm.push(pow(b,a));
            break;
        }
    }catch(...){
        throw;
    }
}
void cal_c(){
    try{
        complex<double> a,b;int op;
        a=s_cmp.pop();op=s_op.pop();b=s_cmp.pop();
        switch(op){
            case 1://'+'
                s_cmp.push(a+b);
            break;
            case 2://'-'
                s_cmp.pusn(b-a);
            break;
            case 3://'*'
                s_cmp.push(a*b);
            break;
            case 4://'/'
                s_cmp.push(b/a);
            break;
            case 5://'^'
                s_cmp.push(pow(b,a));
            break;
            case 6://'@'
                complex<double> ans=(a.real()*b.real()+a.imag()*b.imag(),0);
                push(ans);
            break;
        }
    }catch(...){
        throw;
    }
}
char* getword(int* tar,char* ori){
    TrieNode* tn=op;char* cn=ori;
    while(((*cn>='A')&&(*cn<='Z'))||((*cn>='a')&&(cn<='z'))){
        if(*cn>='a') tn=tn->chi[*cn-'a'];else tn=tn->chi[*cn-'A'];
        ++cn;
    }
    *tar=tn->oper;
    return(cn);
}
int main(int argc,char** argv){
    int argp=0;
    char ch,*now;
    bool nn,rr;
    status=0;
    for(;argp<argc;){
        if(argv[argp][0]=='/'){
            switch(argv[argp][1]){
                case 'i':case 'I':
                status|=1;++argp;break;
                case 'c':case 'C':
                status=status&(~1);status|=2;++argp;break;
                case 'r':case 'R':
                    freopen(argv[argp+1],"r",stdin);argp+=2;
                break;
                case 'w':case 'W':
                    freopen(argv[argp+1],"w",stdout);argp+=2;
                break;
                case 'o':case 'O':
                    switch (argv[argp][2]){
                        case b:case B:status=(status&(~24))4;break;
                        case o:case O:status=(status&(~20))|8;break;
                        case h:case H:status=(status&(~12))|16;break;
                    }
                   status=(status&(~2))|1; 
                   ++argp;
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
    if(argp<argc){
        nn=0;
        if(status&1){
            unsigned long long tmp;
            for(now=argv[argp];*now;){
                switch(*now){
                    case 'H':case 'h':case 'D':case 'd':case 'O':case 'o':case 'B':case 'b':
                    case '0'...'9':nn=1;now=getint(&tmp,now);push(tmp);break;
                    case '+':
                        ++now;
                        rr=1;
                        while(rr){
                            if(s_op.empty()) break;
                            switch(s_op.content[s_op.top]){
                                case 1:case 2:case 3:case 4:case 5:case 11:cal_i();break;
                                default:s_op.push(1);rr=0;break;
                            }
                        }
                    break;
                    case '-':
                        ++now;
                        rr=1;
                        while(rr){
                            if(s_op.empty()) break;
                            switch(s_op.content[s_op.top]){
                                case 1:case 2:case 3:case 4:case 5:case 11:cal_i();break;
                                default:s_op.push(2);rr=0;break;
                            }
                        }
                    break;
                    case '*':
                        ++now;
                        rr=1;
                        while(rr){
                            if(s_op.empty()) break;
                            switch(s_op.content[s_op.top]){
                                case 3:case 4:case 5:case 11:cal_i();break;
                                default:s_op.push(3);rr=0;break;
                            }
                        }
                    break;
                    case '/':
                        ++now;
                        rr=1;
                        while(rr){
                            if(s_op.empty()) break;
                            switch(s_op.content[s_op.top]){
                                case 3:case 4:case 5:case 11:cal_i();break;
                                default:s_op.push(4);rr=0;break;
                            }
                        }
                    break;
                    case '%':
                        ++now;
                        rr=1;
                        while(rr){
                            if(s_op.empty()) break;
                            switch(s_op.content[s_op.top]){
                                case 3:case 4:case 5:case 11:cal_i();break;
                                default:s_op.push(5);rr=0;break;
                            }
                        }
                    break;
                    case '&':
                        ++now;
                        rr=1;
                        while(rr){
                            if(s_op.empty()) break;
                            switch(s_op.content[s_op.top]){
                                case 1:case 2:case 3:case 4:case 5:case 11:cal_i();break;
                                default:s_op.push(8);rr=0;break;
                            }
                        }
                    break;
                    case '|':
                        ++now;
                        rr=1;
                        while(rr){
                            if(s_op.empty()) break;
                            switch(s_op.content[s_op.top]){
                                case 1:case 2:case 3:case 4:case 5:case 8:case 10:case 11:cal_i();break;
                                default:s_op.push(9);rr=0;break;
                            }
                        }   
                    break;
                    case '^':
                        ++now;
                        rr=1;
                        while(rr){
                            if(s_op.empty()) break;
                            switch(s_op.content[s_op.top]){
                                case 1:case 2:case 3:case 4:case 5:case 8:case 11:cal_i();break;
                                default:s_op.push(9);rr=0;break;
                            }
                        }
                    break;
                    case '<':
                    break;
                    case '>':
                    break;
                    case '!':
                    break;
                    case '(':
                    break;
                    case ')':
                    break;
                    case '$':
                    break;
                }
            }    
        }else{
        }
    }   
    while(0);
  
}
