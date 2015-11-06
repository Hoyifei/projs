#include <cstdio>
#include <cmath>
#include <complex>
#define PiE 3.141592653589793239463
using namespace std;
/*
 * -i :Unsigned Integer only mode.Input should only contain integer numbers,Bin/Oct/Dec/Hex numbers can be mixed.
 *  B100010:binary int 100010;O72510:Oct int 72510;D12345 or 12345:Demical int 12345;HA3FF2:Hex int A3FF2
 * -O X or -o X:Output control.X could be B/O/D/H.Auto regard as Integer-only mode.
 * -r <path>:read input from file
 * -w <path>:write output to file
 * normal:use double as result;% wil be regard as percent
 * -b: Integer only.^ & | is availiable
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
 * '[]':floor,normal only
 * '{}':ceil,normal only
 * '<>':round,normal only
 * ---------------------------------------------------------------------------------------------------------
 *  sin|cos|tan|cot|sec|csc|asin|acos|atan|sinh|cosh|tanh|
 *  sqrt|ln|lg
 *  pi is a function:$pi(),expression will be ignored.
 *  ---fuctions not availiable in int-only mode---
 *  last:the last answer.will be cleared as 0 after error or mode changing
 * ---------------------------------------------------------------------------------------------------------
 *  assume that modes will be put in the front and be read from left to right.
 *  uncompatible mods will cause program to exit
 * */
template <typename TY>
struct TSmallStack{
    TY content[100000];
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
TSmallStack <long double> s_nrm;
char comm[100000];bool erro;
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
    TrieNode* now=op;
    for(int i=0;cont[i];++i){
        if(now->chi[cont[i]-'a']==op) now->chi[cont[i]-'a']=getnode();
        now=now->chi[cont[i]-'a'];     
    }
    now->oper=++totfunc;    
}
char* getint(unsigned long long *tar,char* sou){
    *tar=0;unsigned long long bas=10;
    char* now=sou;
    switch (*now){
        case 'b':case 'B':bas=2;++now;break;
        case 'o':case 'O':bas=8;++now;break;
        case 'd':case 'D':bas=10;++now;break;
        case 'h':case 'H':bas=16;++now;break;
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
char* getfloat(long double *tar,char* sou){
    int ex=0,fuck=0,step=1;bool fu=0,ck=0;char* now=sou;*tar=0;
    long double base=10;
    int deb;
    switch(*now){
        case '-':fu=1;++now;break;
        case 'b':case 'B':base=2;++now;break;
        case 'o':case 'O':base=8;++now;break;
        case 'd':case 'D':base=10;++now;break;
        case 'h':case 'H':base=16;++now;break;
    } 
    switch(*now){
        case '-':fu=1;++now;break;
        case 'b':case 'B':if(base!=16){base=2;++now;}break;
        case 'o':case 'O':base=8;++now;break;
        case 'd':case 'D':if(base!=16){base=10;++now;}break;
        case 'h':case 'H':base=16;++now;break;
    }
    while(step==1){
        //printf("2now=%c\n",*now);
        //printf("step=%d (step==1)=%d\n",step,step==1);
        //scanf("%d",&deb);
        //printf("now=='.'=%d\n",(*now)=='.');
        switch(*now){
            case '0'...'9':*tar=*tar*base+(*now)-'0';++now;break;
            case 'A'...'F':*tar=*tar*base+(*now)-'A'+10;++now;break;
            case 'a'...'f':*tar=*tar*base+(*now)-'a'+10;++now;break;
            case 'p':case 'P':step=3;++now;break;
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
            case 'p':case 'P':step=3;++now;break;
            case '0'...'9':*tar=*tar*base+(*now)-'0';++now;--ex;break;
            case 'a'...'f':*tar=*tar*base+(*now)-'a'+10;++now;--ex;break;
            case 'A'...'F':*tar=*tar*base+(*now)-'A'+10;++now;--ex;break;
            default:step=4;break;
        }
    }
    while(step==3){
     //   printf("4now=%c\n",*now);
        switch(*now){
            case '0'...'9':fuck=fuck*base+(*now)-'0';++now;break;
            case 'a'...'h':fuck=fuck*base+(*now)-'a'+10;++now;break;
            case 'A'...'H':fuck=fuck*base+(*now)-'A'+10;++now;break;
            case '-':ck=1;++now;break;
            default:step=4;break;
        }
    }
    if(ck==1) fuck=-fuck;
    ex+=fuck;
    if(fu==1) *tar=-(*tar);
    *tar*=pow(base,ex);
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
        unsigned long long a,b;int op;
        a=s_int.pop();op=s_op.pop();
        if(op!=11&&s_int.empty()){
            printf("FuckYou");
            erro=1;
            s_int.push(a);
            return;
        }else b=s_int.pop();
        switch(op){
            case 1://'+'
                s_int.push(a+b);
            break;
            case 2://'-'
                s_int.push(b-a);
            break;
            case 3://'*'
                s_int.push(a*b);
            break;
            case 4://'/'
                if(a==0) {printf("FuckYou");s_int.push(b);erro=1;}else s_int.push(b/a);
            break;
            case 5://'%'
                if(a==0) {printf("FuckYou");s_int.push(b);erro=1;}else s_int.push(b%a);
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
            default:
                erro=1;
                printf("FuckYou");
                s_int.push(b);
            break;
        }
}
void cal_n(){
        long double a,b;int op;
        a=s_nrm.pop();op=s_op.pop();
        if(s_nrm.empty()){
            printf("FuckYou");erro=1;s_nrm.push(a);return;
        }else{    
            b=s_nrm.pop();
        }
        switch(op){
            case 1://'+'
                s_nrm.push(a+b);
            break;
            case 2://'-'
                s_nrm.push(b-a);
            break;
            case 3://'*'
                s_nrm.push(a*b);
            break;
            case 4://'/'
                if(abs(a)<1e-290){printf("FuckYou1");s_nrm.push(b);erro=1;}else s_nrm.push(b/a);
            break;
            case 10://'^'
                if(b<0||(abs(a)<1e-290&&abs(b)<1e-290)){printf("FuckYou2\n");erro=1;s_nrm.push(a);}
                else s_nrm.push(pow(b,a));
            break;
            default:
                printf("FuckYou3");erro=1;s_nrm.push(a);
                break;
        }
}
char* getword(int* tar,char* ori){
    TrieNode* tn=op;char* cn=ori;
    while(((*cn>='A')&&(*cn<='Z'))||((*cn>='a')&&(*cn<='z'))){
        if(*cn>='a') tn=tn->chi[*cn-'a'];else tn=tn->chi[*cn-'A'];
        ++cn;
    }
    *tar=tn->oper;
    return(cn);
}
int main(int argc,char** argv){
    int argp=1;
    char ch,*now;
    bool nn,rr,stay=0;
    totfunc=128;totnode=0;op=&tpool[0];for(int i=0;i<26;++i) op->chi[i]=op;
    insword("last");insword("sin");insword("cos");insword("tan");
    insword("sec");insword("csc");insword("asin");
    insword("acos");insword("atan");insword("sinh");insword("cosh");
    insword("tanh");insword("sqrt");insword("ln");insword("lg");insword("pi");insword("e");
    //......
    s_int.content[0]=0;s_nrm.content[0]=0;s_int.top=0;s_nrm.top=0;s_op.top=0;
    if(argc==1) stay=1;
    for(;argp<argc;){
        if(argv[argp][0]=='/'){
            switch(argv[argp][1]){
                case 's':case 'S':stay=1;++argp;break;
                case 'i':case 'I':
                status|=1;++argp;break;
                case 'f':case 'F':
                status&=(~1);++argp;break;
                case 'r':case 'R':
                    freopen(argv[argp+1],"r",stdin);argp+=2;stay=1;
                break;
                case 'w':case 'W':
                    freopen(argv[argp+1],"w",stdout);argp+=2;
                break;
                case 'o':case 'O':
                    switch (argv[argp][2]){
                        case 'b':case 'B':status=(status&(~24))|4;break;
                        case 'o':case 'O':status=(status&(~20))|8;break;
                        case 'h':case 'H':status=(status&(~12))|16;break;
                    }
                   status|=1; 
                   ++argp;
                break;
                case '?':case 'h':case 'H':
                    freopen("help.txt","r",stdin);
                    ch=getchar();
                    while(ch!='~'){printf("%c",ch);ch=getchar();}
                    return(0);
                default:++argp;break;
            }
        }else break;
    }
    if(argp<argc){
        nn=0;erro=0;
        if(status&1){
            unsigned long long tmp;
            for(now=argv[argp];*now;){
                switch(*now){
                    case 'H':case 'h':case 'D':case 'd':case 'O':case 'o':case 'B':case 'b':
                    case '0'...'9':now=getint(&tmp,now);s_int.push(tmp);break;
                    case '+':
                        ++now;
                        rr=1;
                        while(rr){
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
                            switch(s_op.content[s_op.top]){
                                case 6:case 7:case 1:case 2:case 3:case 4:case 5:case 11:cal_i();break;
                                default:s_op.push(8);rr=0;break;
                            }
                        }
                    break;
                    case '|':
                        ++now;
                        rr=1;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 6:case 7:case 1:case 2:case 3:case 4:case 5:case 8:case 10:case 11:cal_i();break;
                                default:s_op.push(9);rr=0;break;
                            }
                        }   
                    break;
                    case '^':
                        ++now;
                        rr=1;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 6:case 7:case 1:case 2:case 3:case 4:case 5:case 8:case 11:cal_i();break;
                                default:s_op.push(10);rr=0;break;
                            }
                        }
                    break;
                    case '<':
                        ++now;rr=1;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 1:case 2:case 3:case 4:case 5:case 6:case 7:cal_i();break;
                                default:s_op.push(6);rr=0;break;
                            }
                        }
                    break;
                    case '>':
                        ++now;rr=1;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 1:case 2:case 3:case 4:case 5:case 6:case 7:cal_i();break;
                                default:s_op.push(7);rr=0;break;
                            }
                        }
                    break;
                    case '!':
                        ++now;s_op.push(11);
                    break;
                    case '(':
                        ++now;s_op.push(12);
                    break;
                    case ')':
                        ++now;while(!(s_op.content[s_op.top]==12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_i();
                         if(s_op.empty()){
                           printf("FuckYou");erro=1;
                        }else{
                           int fn=s_op.pop();
                            switch(fn){
                                case 12:break;
                                case 129:if(!s_int.empty())s_int.pop();s_int.push(s_int.content[0]);break;
                                default://report error
                                    printf("WHAT THE HELL IS THIS");
                                    erro=1;
                                    break;
                            }
                        }
                    break;
                    case '$':
                        int fn;
                        now=getword(&fn,now+1)+1;
                        s_op.push(fn);
                    break;
                    default:++now;break;
                }
            }
            while(!s_op.empty()) cal_i();
            unsigned long long aa=s_int.pop();
            if(!s_int.empty()) {printf("FuckYou");erro=1;}
            if(erro){
                erro=0;
                s_int.top=0;
                s_int.content[0]=0; 
                printf("\n");
            }else{
                s_int.content[0]=aa;  
                if(status&4){
                    unsigned long long bb=(unsigned long long)1<<63;
                    while((!(bb&aa))&&(bb)) bb>>=1;
                    if(!bb) printf("0\n");else{
                        for(;bb;bb>>=1) printf("%d",(aa&bb)>0);
                        printf("\n");
                    }
                }else if(status&8){
                    printf("%o\n",aa);
                }else if(status&16){
                    printf("%x\n",aa);
                }else{
                    printf("%llu\n",aa);
                }
            }
        }else{
            long double tmp;nn=0;
            for(now=argv[argp];*now;){
                switch(*now){
                    case 'b':case 'B':case 'o':case 'O':case 'd':case 'D':case 'h':case 'H':
                    case '0'...'9':nn=1;now=getfloat(&tmp,now);s_nrm.push(tmp);break;
                    case '+':
                        nn=0;++now;rr=1;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 10:case 1:case 2:case 3:case 4:case 5:cal_n();break;
                                default:s_op.push(1);rr=0;break;
                            }
                        }
                    break;
                    case '-':
                        if(!nn){nn=1;now=getfloat(&tmp,now);s_nrm.push(tmp);break;}
                        ++now;
                        rr=1;nn=0;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 10:case 1:case 2:case 3:case 4:case 5:cal_n();break;
                                default:s_op.push(2);rr=0;break;
                            }
                        }
                    break;
                    case '*':
                        ++now;
                        rr=1;nn=0;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 10:case 3:case 4:case 5:cal_n();break;
                                default:s_op.push(3);rr=0;break;
                            }
                        }
                    break;
                    case '/':
                        ++now;
                        rr=1;nn=0;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 10:case 3:case 4:case 5:cal_n();break;
                                default:s_op.push(4);rr=0;break;
                            }
                        }
                    break;
                    case '%':
                        ++now;
                        if(nn!=0){
                            nn=0;s_nrm.push(s_nrm.pop()/100.0);
                        } 
                    break;
                    case '^':
                        ++now;
                        s_op.push(10);rr=0;break;
                    break;
                    case '<':case '{':case '[':case '(':
                        nn=0;++now;s_op.push(12);
                    break;
                    case ')':
                        nn=1;
                        ++now;while(!(s_op.content[s_op.top]==12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_n();
                        if(s_op.empty()){
                            printf("FuckYou");
                            erro=1;s_op.pop();
                        }else{
                            int fn=s_op.pop();
                            long double aaa;
                            switch(fn){
                                case 12:break;
                                case 129:if(!s_nrm.empty())s_nrm.pop();s_nrm.push(s_nrm.content[0]);break;
                                case 130:s_nrm.push(sin(s_nrm.pop()));break;
                                case 131:s_nrm.push(cos(s_nrm.pop()));break;
                                case 132:
                                         aaa=s_nrm.pop();
                                         if(abs(((aaa+0.5*PiE)/PiE)-round((aaa+0.5*PiE)/PiE))<1e-10){
                                             printf("FuckYou");erro=1;
                                         }else s_nrm.push(tan(aaa));break;
                                case 133:
                                         aaa=s_nrm.pop();
                                         if(abs(aaa)<1e-290){printf("FuckYou");erro=1;s_nrm.push(aaa);}else s_nrm.push(1.0/aaa);break;
                                case 134:
                                         aaa=s_nrm.pop();
                                         if(abs(aaa)<1e-290){printf("FuckYou");erro=1;s_nrm.push(aaa);}else s_nrm.push(1.0/aaa);break;
                                case 135:
                                         aaa=s_nrm.pop();
                                         if(abs(aaa)>1){printf("FuckYou");erro=1;s_nrm.push(aaa);}else s_nrm.push(asin(aaa));break;
                                case 136:
                                         aaa=s_nrm.pop();
                                         if(abs(aaa)>1){printf("FuckYou");erro=1;s_nrm.push(aaa);}else s_nrm.push(acos(aaa));break;
                                case 137:s_nrm.push(atan(s_nrm.pop()));break;
                                case 138:s_nrm.push(sinh(s_nrm.pop()));break;
                                case 139:s_nrm.push(cosh(s_nrm.pop()));break;
                                case 140:s_nrm.push(tanh(s_nrm.pop()));break;
                                case 141:
                                         aaa=s_nrm.pop();
                                         if(aaa<0){printf("FuckYou");erro=1;s_nrm.push(aaa);}else s_nrm.push(sqrt(aaa));break;
                                case 142:
                                         aaa=s_nrm.pop();
                                         if(aaa<1e-30){printf("FuckYou");erro=1;s_nrm.push(aaa);}else s_nrm.push(log(aaa));break;
                                case 143:
                                         aaa=s_nrm.pop();
                                         if(aaa<1e-30){printf("FuckYou");erro=1;s_nrm.push(aaa);}else s_nrm.push(log10(aaa));break;
                                case 144:if(!s_nrm.empty())s_nrm.pop();s_nrm.push(PiE);
                                case 145:if(!s_nrm.empty())s_nrm.pop();s_nrm.push(exp(1));
                                default://report error
                                    printf("WHAT THE HELL IS THIS");
                                    erro=1;
                                    break;
                            }
                        }
                    break;
                    case '|':
                        ++now;
                        if(nn){
                            while(!(s_op.content[s_op.top]==12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_n();
                            if(s_op.empty()){
                                printf("FuckYou");
                                erro=1;s_op.pop();
                           }else{
                                int fn=s_op.pop();
                                if(fn!=12){
                                    printf("FuckYou");
                                    erro=1;
                                }else{
                                   s_nrm.push(abs(s_nrm.pop()));
                                }
                            }
                        }else{
                            s_op.push(12);
                        }
                    break;
                    case '>':
                        ++now; 
                        while(!(s_op.content[s_op.top]==12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_n();
                        if(s_op.empty()){
                            printf("FuckYou");
                            erro=1;s_op.pop();
                        }else{
                            int fn=s_op.pop();
                            if(fn!=12){
                                printf("FuckYou");
                                erro=1;
                            }else{
                               s_nrm.push(round(s_nrm.pop()));
                            }
                        }
                    break;
                    case '}':
                        ++now; 
                        while(!(s_op.content[s_op.top]==12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_n();
                         if(s_op.empty()){
                            printf("FuckYou");
                            erro=1;s_op.pop();
                        }else{
                           int fn=s_op.pop();
                            if(fn!=12){
                                printf("FuckYou");
                                erro=1;
                            }else{
                               s_nrm.push(ceil(s_nrm.pop()));
                            }
                        }
                    break;
                    case ']':
                        ++now; 
                        while(!(s_op.content[s_op.top]==12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_n();
                        if(s_op.empty()){
                            printf("FuckYou");
                            erro=1;s_op.pop();
                        }else{
                            int fn=s_op.pop();
                            if(fn!=12){
                                printf("FuckYou");
                                erro=1;
                            }else{
                                s_nrm.push(floor(s_nrm.pop()));        
                            }
                        }
                    break;
                    case '$':
                        int fn;
                        now=getword(&fn,now+1)+1;
                        s_op.push(fn);
                    break;
                    default:++now;break;
                }
            }
            while(!s_op.empty()) cal_n();
            long double ans=s_nrm.pop();
            if(!s_nrm.empty()){
                printf("FuckYou");
                erro=1;
            }
            if(erro){
                erro=0;
                printf("\n");
                s_nrm.top=0;
                s_nrm.content[0]=0;
            }else{
                s_nrm.content[0]=ans;
                printf("%.20e\n",(double)ans);
            }
         }
    }
    comm[0]='q';  
    if(stay) scanf("%s",comm); 
    while(comm[0]!='q'&&comm[0]!='Q'){
        if(comm[0]=='/'){
            switch(comm[1]){
                case 'i':case 'I':
                status|=1;break;
                case 'f':case 'F':
                status&=(~1);break;
                case 'r':case 'R':
                    scanf("%s",comm);
                    fclose(stdin);
                    freopen(comm,"r",stdin);
                break;
                case 'w':case 'W':
                    scanf("%s",comm);
                    freopen(comm,"w",stdout);argp+=2;
                break;
                case 'o':case 'O':
                    switch (comm[2]){
                        case 'b':case 'B':status=(status&(~24))|4;break;
                        case 'o':case 'O':status=(status&(~20))|8;break;
                        case 'h':case 'H':status=(status&(~12))|16;break;
                    }
                   status|=1; 
                break;
                default:break;
            }
        }else{
            nn=0;erro=0;
            if(status&1){
            unsigned long long tmp;
            for(now=comm;*now;){
                switch(*now){
                    case 'H':case 'h':case 'D':case 'd':case 'O':case 'o':case 'B':case 'b':
                    case '0'...'9':now=getint(&tmp,now);s_int.push(tmp);break;
                    case '+':
                        ++now;
                        rr=1;
                        while(rr){
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
                            switch(s_op.content[s_op.top]){
                                case 6:case 7:case 1:case 2:case 3:case 4:case 5:case 11:cal_i();break;
                                default:s_op.push(8);rr=0;break;
                            }
                        }
                    break;
                    case '|':
                        ++now;
                        rr=1;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 6:case 7:case 1:case 2:case 3:case 4:case 5:case 8:case 10:case 11:cal_i();break;
                                default:s_op.push(9);rr=0;break;
                            }
                        }   
                    break;
                    case '^':
                        ++now;
                        rr=1;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 6:case 7:case 1:case 2:case 3:case 4:case 5:case 8:case 11:cal_i();break;
                                default:s_op.push(10);rr=0;break;
                            }
                        }
                    break;
                    case '<':
                        ++now;rr=1;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 1:case 2:case 3:case 4:case 5:case 6:case 7:cal_i();break;
                                default:s_op.push(6);rr=0;break;
                            }
                        }
                    break;
                    case '>':
                        ++now;rr=1;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 1:case 2:case 3:case 4:case 5:case 6:case 7:cal_i();break;
                                default:s_op.push(7);rr=0;break;
                            }
                        }
                    break;
                    case '!':
                        ++now;s_op.push(11);
                    break;
                    case '(':
                        ++now;s_op.push(12);
                    break;
                    case ')':
                        ++now;while(!(s_op.content[s_op.top]==12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_i();
                        if(s_op.empty()){
                           printf("FuckYou");erro=1;
                        }else{
                            int fn=s_op.pop();
                            switch(fn){
                                case 12:break;
                                case 129:if(!s_int.empty())s_int.pop();s_int.push(s_int.content[0]);break;
                                default://report error
                                    printf("WHAT THE HELL IS THIS");
                                    erro=1;
                                    break;
                            }
                        }
                    break;
                    case '$':
                        int fn;
                        now=getword(&fn,now+1)+1;
                        s_op.push(fn);
                    break;
                    default:++now;break;
                }
            }
            while(!s_op.empty()) cal_i();
            unsigned long long aa=s_int.pop();
            if(!s_int.empty()) {printf("FuckYou");erro=1;}
            if(erro){
                erro=0;
                s_int.top=0;
                s_int.content[0]=0; 
                printf("\n");
            }else{
                s_int.content[0]=aa;  
                if(status&4){
                    unsigned long long bb=(unsigned long long)1<<63;
                    while((!(bb&aa))&&(bb)) bb>>=1;
                    if(!bb) printf("0\n");else{
                        for(;bb;bb>>=1) printf("%d",(aa&bb)>0);
                        printf("\n");
                    }
                }else if(status&8){
                    printf("%o\n",aa);
                }else if(status&16){
                    printf("%x\n",aa);
                }else{
                    printf("%llu\n",aa);
                }
            }
        }else{
            long double tmp;nn=0;
            for(now=comm;*now;){
                switch(*now){
                    case 'b':case 'B':case 'o':case 'O':case 'd':case 'D':case 'h':case 'H':
                    case '0'...'9':nn=1;now=getfloat(&tmp,now);s_nrm.push(tmp);break;
                    case '+':
                        nn=0;++now;rr=1;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 10:case 1:case 2:case 3:case 4:case 5:cal_n();break;
                                default:s_op.push(1);rr=0;break;
                            }
                        }
                    break;
                    case '-':
                        if(!nn){nn=1;now=getfloat(&tmp,now);s_nrm.push(tmp);break;}
                        ++now;
                        rr=1;nn=0;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 10:case 1:case 2:case 3:case 4:case 5:cal_n();break;
                                default:s_op.push(2);rr=0;break;
                            }
                        }
                    break;
                    case '*':
                        ++now;
                        rr=1;nn=0;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 10:case 3:case 4:case 5:cal_n();break;
                                default:s_op.push(3);rr=0;break;
                            }
                        }
                    break;
                    case '/':
                        ++now;
                        rr=1;nn=0;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 10:case 3:case 4:case 5:cal_n();break;
                                default:s_op.push(4);rr=0;break;
                            }
                        }
                    break;
                    case '%':
                        ++now;
                        if(nn!=0){
                            nn=0;s_nrm.push(s_nrm.pop()/100.0);
                        } 
                    break;
                    case '^':
                        ++now;
                        s_op.push(10);rr=0;break;
                    break;
                    case '<':case '{':case '[':case '(':
                        nn=0;++now;s_op.push(12);
                    break;
                    case ')':
                        nn=1;
                        ++now;
                        while(!(s_op.content[s_op.top]==12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_n();
                        if(s_op.empty()){
                           printf("FuckYou");erro=1;
                        }else{
                            int fn=s_op.pop();
                            long double aaa;
                            switch(fn){
                                case 12:break;
                                case 129:if(!s_nrm.empty()) s_nrm.pop();s_nrm.push(s_nrm.content[0]);break;
                                case 130:s_nrm.push(sin(s_nrm.pop()));break;
                                case 131:s_nrm.push(cos(s_nrm.pop()));break;
                                case 132:
                                         aaa=s_nrm.pop();
                                         if(abs(((aaa+0.5*PiE)/PiE)-round((aaa+0.5*PiE)/PiE))<1e-10){
                                             printf("FuckYou");erro=1;
                                         }else s_nrm.push(tan(aaa));break;
                                case 133:
                                         aaa=s_nrm.pop();
                                         if(abs(aaa)<1e-290){printf("FuckYou");erro=1;s_nrm.push(aaa);}else s_nrm.push(1.0/aaa);break;
                                case 134:
                                         aaa=s_nrm.pop();
                                         if(abs(aaa)<1e-290){printf("FuckYou");erro=1;s_nrm.push(aaa);}else s_nrm.push(1.0/aaa);break;
                                case 135:
                                         aaa=s_nrm.pop();
                                         if(abs(aaa)>1){printf("FuckYou");erro=1;s_nrm.push(aaa);}else s_nrm.push(asin(aaa));break;
                                case 136:
                                         aaa=s_nrm.pop();
                                         if(abs(aaa)>1){printf("FuckYou");erro=1;s_nrm.push(aaa);}else s_nrm.push(acos(aaa));break;
                                case 137:s_nrm.push(atan(s_nrm.pop()));break;
                                case 138:s_nrm.push(sinh(s_nrm.pop()));break;
                                case 139:s_nrm.push(cosh(s_nrm.pop()));break;
                                case 140:s_nrm.push(tanh(s_nrm.pop()));break;
                                case 141:
                                         aaa=s_nrm.pop();
                                         if(aaa<0){printf("FuckYou");erro=1;s_nrm.push(aaa);}else s_nrm.push(sqrt(aaa));break;
                                case 142:
                                         aaa=s_nrm.pop();
                                         if(aaa<1e-30){printf("FuckYou");erro=1;s_nrm.push(aaa);}else s_nrm.push(log(aaa));break;
                                case 143:
                                         aaa=s_nrm.pop();
                                         if(aaa<1e-30){printf("FuckYou");erro=1;s_nrm.push(aaa);}else s_nrm.push(log10(aaa));break;
                                case 144:if(!s_nrm.empty())s_nrm.pop();s_nrm.push(PiE);
                                case 145:if(!s_nrm.empty())s_nrm.pop();s_nrm.push(exp(1));
                                default://report error
                                    printf("WHAT THE HELL IS THIS");
                                    erro=1;
                                    break;
                            }
                        }
                    break;
                    case '|':
                        ++now;
                        if(nn){
                            while(!(s_op.content[s_op.top]==12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_n();
                            if(s_op.empty()){
                                printf("FuckYou");
                                erro=1;s_op.pop();
                           }else{
                                int fn=s_op.pop();
                                if(fn!=12){
                                    printf("FuckYou");
                                    erro=1;
                                }else{
                                   s_nrm.push(abs(s_nrm.pop()));
                                }
                            }
                        }else{
                            s_op.push(12);
                        }
                    break;
                    case '>':
                        ++now; 
                        while(!(s_op.content[s_op.top]==12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_n();
                        if(s_op.empty()){
                            printf("FuckYou");
                            erro=1;s_op.pop();
                        }else{
                            int fn=s_op.pop();
                            if(fn!=12){
                                printf("FuckYou");
                                erro=1;
                            }else{
                               s_nrm.push(round(s_nrm.pop()));
                            }
                        }
                    break;
                    case '}':
                        ++now; 
                        while(!(s_op.content[s_op.top]==12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_n();
                        if(s_op.empty()){
                            printf("FuckYou");
                            erro=1;s_op.pop();
                        }else{
                            int fn=s_op.pop();
                            if(fn!=12){
                                printf("FuckYou");
                                erro=1;
                            }else{
                               s_nrm.push(ceil(s_nrm.pop()));
                            }
                        }
                    break;
                    case ']':
                        ++now; 
                        while(!(s_op.content[s_op.top]==12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_n();
                        if(s_op.empty()){
                            printf("FuckYou");
                            erro=1;s_op.pop();
                        }else{
                            int fn=s_op.pop();
                            if(fn!=12){
                                printf("FuckYou");
                                erro=1;
                            }else{
                                s_nrm.push(floor(s_nrm.pop()));        
                            }
                        }
                    break;
                    case '$':
                        int fn;
                        now=getword(&fn,now+1)+1;
                        s_op.push(fn);
                    break;
                    default:++now;break;
                }
            }
            while(!s_op.empty()) cal_n();
            long double ans=s_nrm.pop();
            if(!s_nrm.empty()){
                printf("FuckYou");
                erro=1;
            }
            if(erro){
                printf("\n");
                s_nrm.top=0;
                s_nrm.content[0]=0;
                erro=0;
            }else{
                s_nrm.content[0]=ans;
                //printf("%.17f\n",ans);
                printf("%.20e\n",(double)ans);
            }
         }
        }
        comm[0]='q';
        scanf("%s",comm);
    }
    fclose(stdout);
    return(0);
}
