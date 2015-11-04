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
TSmallStack <double> s_nrm;
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
    while((*now>'9'||*now<'0')&&(*now>'F'||*now<'A')&&(*now>'f'||*now<'a')){
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
                s_int.push(b-a);
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
                s_nrm.push(b-a);
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
    bool nn,rr;
    totfunc=128;totnode=0;op=&tpool[0];for(int i=0;i<26;++i) op->chi[i]=op;
    insword("last");insword("sin");insword("cos");insword("tan");
    insword("cot");insword("sec");insword("csc");insword("asin");
    insword("acos");insword("atan");insword("sinh");insword("cosh");
    insword("tanh");insword("sqrt");insword("ln");insword("lg");insword("pi");
    //......
    s_int.content[0]=0;s_nrm.content[0]=0;s_int.top=0;s_nrm.top=0;s_op.top=0;
    for(int i=0;i<argc;++i) printf("'%s'\n",argv[i]);
    for(;argp<argc;){
        if(argv[argp][0]=='/'){
            switch(argv[argp][1]){
                case 'i':case 'I':
                status|=1;++argp;break;
                case 'r':case 'R':
                    freopen(argv[argp+1],"r",stdin);argp+=2;
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
                case 'h':case 'H':
                    freopen("help.txt","r",stdin);
                    ch=getchar();
                    while(ch!='~'){printf("%c",ch);ch=getchar();}
                    return(0);
                default:break;
            }
        }else break;
    }
    printf("Argument anaylisis complete!\n");
    printf("%d %d\n",argp,argc);
    if(argp<argc){
        printf("calc");
        nn=0
        if(status&1){
            printf("INT_CALC\n");
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
                        if(s_int.empty()) break;
                        ++now;while((s_op.content[s_op.top]!=11||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_i();
                        //if(s_op.empty()) report error
                        if(!s_op.empty()) {
                            int fn=s_op.pop();
                            switch(fn){
                                case 129:s_int.pop();s_int.push(s_int.content[0]);break;
                                default://report error
                                    break;
                            }
                        }
                    break;
                    case '$':
                        int fn;
                        now=getword(&fn,now+1)+1;
                        //if(now!=129) report error
                        s_op.push(fn);
                    break;
                }
            }
            while(!s_op.empty()) cal_i();
            unsigned long long aa=s_int.pop();
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
        }else{
            printf("float calc\n");
            double tmp;nn=0;
            for(now=argv[argp];*now;){
                switch(*now){
                    case '0'...'9':nn=1;now=getfloat(&tmp,now);s_nrm.push(tmp);break;
                    case '+':
                        nn=0;
                        ++now;
                        rr=1;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 1:case 2:case 3:case 4:case 5:cal_n();break;
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
                                case 1:case 2:case 3:case 4:case 5:cal_n();break;
                                default:s_op.push(2);rr=0;break;
                            }
                        }
                    break;
                    case '*':
                        ++now;
                        rr=1;nn=0;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 3:case 4:case 5:cal_n();break;
                                default:s_op.push(3);rr=0;break;
                            }
                        }
                    break;
                    case '/':
                        ++now;
                        rr=1;nn=0;
                        while(rr){
                            switch(s_op.content[s_op.top]){
                                case 3:case 4:case 5:cal_n();break;
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
                        ++now;while((s_op.content[s_op.top]!=12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_i();
                        //if(s_op.empty()) report error
                        if(!s_op.empty()) {
                            int fn=s_op.pop();
                            switch(fn){
                                case 129:s_nrm.pop();s_int.push(s_int.content[0]);break;
                                case 130:s_nrm.push(sin(s_nrm.pop()));break;
                                case 131:s_nrm.push(cos(s_nrm.pop()));break;
                                case 132:s_nrm.push(tan(s_nrm.pop()));break;
                                case 133:s_nrm.push(1.0/cos(s_nrm.pop()));break;
                                case 134:s_nrm.push(1.0/sin(s_nrm.pop()));break;
                                case 135:s_nrm.push(asin(s_nrm.pop()));break;
                                case 136:s_nrm.push(acos(s_nrm.pop()));break;
                                case 137:s_nrm.push(atan(s_nrm.pop()));break;
                                case 138:s_nrm.push(sinh(s_nrm.pop()));break;
                                case 139:s_nrm.push(cosh(s_nrm.pop()));break;
                                case 140:s_nrm.push(tanh(s_nrm.pop()));break;
                                case 141:s_nrm.push(sqrt(s_nrm.pop()));break;
                                case 142:s_nrm.push(log(s_nrm.pop()));break;
                                case 143:s_nrm.push(log10(s_nrm.pop()));break;
                                case 144:s_nrm.pop();s_nrm.push(3.141592653589793238463);
                                default://report error
                                    break;
                            }
                        }
                    break;
                    case '|':
                        ++now;
                        if(nn){
                            while((s_op.content[s_op.top]!=12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_i();
                             //if(s_op.empty()) report error
                            if(!s_op.empty()) {
                                int fn=s_op.pop();
                                //if(fn!=12) report error
                                if(fn==12){
                                    s_op.pop();
                                    s_nrm.push(abs(s_nrm.pop()));
                                }
                            }
                        }else{
                            s_op.push(12);
                        }
                    break;
                    case '>':
                        ++now; 
                        while((s_op.content[s_op.top]!=12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_i();
                        //if(s_op.empty()) report error
                        if(!s_op.empty()) {
                            int fn=s_op.pop();
                            //if(fn!=12) report error
                            if(fn==12){
                                s_op.pop();
                                s_nrm.push(round(s_nrm.pop()));
                            }
                        }
                    break;
                    case '}':
                        ++now; 
                        while((s_op.content[s_op.top]!=12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_i();
                        //if(s_op.empty()) report error
                        if(!s_op.empty()) {
                            int fn=s_op.pop();
                            //if(fn!=12) report error
                            if(fn==12){
                                s_op.pop();
                                s_nrm.push(ceil(s_nrm.pop()));
                            }
                        }
                    break;
                    case ']':
                        ++now; 
                        while((s_op.content[s_op.top]!=12||s_op.content[s_op.top]>128)&&(!s_op.empty())) cal_i();
                        //if(s_op.empty()) report error
                        if(!s_op.empty()) {
                            int fn=s_op.pop();
                            //if(fn!=12) report error
                            if(fn==12){
                                s_op.pop();
                                s_nrm.push(floor(s_nrm.pop()));        
                            }
                        }
                    break;
                    case '$':
                        int fn;
                        now=getword(&fn,now+1)+1;
                        //if(now!=129) report error
                        s_op.push(fn);
                    break;
                }
            }
            printf("%d %d\n",s_op.top,s_nrm.top);
            while(!s_op.empty()) cal_n();
            double ans=s_nrm.pop();
            s_nrm.content[0]=ans;
            printf("%f\n",ans);
         }
    }   
    while(0);
  
}
