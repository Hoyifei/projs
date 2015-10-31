#include "ScreenItem.h"
void TScreenItem::reinit(){
    Anim=Now=0;
}
void TScreenItem::nextframe(){
    Now=Now->next;
}
TScreenItem::TScreenItem()
{
    Anim=Now=0;
    Next=0;
    texpos[0][0]=1;texpos[0][1]=1;
    texpos[1][0]=0;texpos[1][1]=1;
    texpos[2][0]=0;texpos[2][1]=0;
    texpos[3][0]=1;texpos[3][1]=0;
}
void TScreenItem::copyitem(TScreenItem* tar)
{
    Anim=tar->Anim;
    Now=tar->Now;
    hwid=tar->hwid;
    hhei=tar->hhei;
}
void TScreenItem::loadframe(TFrame* tar,const char* path)
{
    printf("%d load frame %s\n",this,path);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_BLEND);
    glGenTextures(1,&(tar->content));
    glBindTexture(GL_TEXTURE_2D,tar->content);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glfwLoadTexture2D(path,0);
    if(!Anim) Anim=tar;else Now->next=tar;
    Now=tar;
}
void TScreenItem::copyframe(TFrame* tar,TFrame* ins){
    ins->content=tar->content;
    if(!Anim) Anim=ins;else Now->next=ins;
    Now=ins;
}
void TScreenItem::init(){
    //printf("Initialize a ScrItem\n");
    Now->next=Anim;
    //printf("Step1\n");
    Now=Anim;
    //printf("Step2\n");
}
void TScreenItem::reset(){
    Now=Anim;
}
void TScreenItem::drawGL(){
    printf("%d drawgl cen@(%f,%f),size(%f,%f)",this,cen[0],cen[1],hwid,hhei);
    cor[0][0]=hwid*cos(ang)-hhei*sin(ang)+cen[0];
    cor[1][0]=cen[0]-hwid*cos(ang)-hhei*sin(ang);
    cor[2][0]=cen[0]-hwid*cos(ang)+hhei*sin(ang);
    cor[3][0]=hwid*cos(ang)+hhei*sin(ang)+cen[0];
    cor[0][1]=hwid*sin(ang)+hhei*cos(ang)+cen[1];
    cor[1][1]=cen[1]-hwid*sin(ang)+hhei*cos(ang);
    cor[2][1]=cen[1]-hwid*sin(ang)-hhei*cos(ang);
    cor[3][1]=hwid*sinf(ang)-hhei*cosf(ang)+cen[1];
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_BLEND);
    glBindTexture(GL_TEXTURE_2D,Now->content);
    glBegin(GL_POLYGON);
        for(int i=0;i<4;++i){
            glTexCoord2fv(texpos[i]);
            glVertex2fv(cor[i]);
        }
    glEnd();
    if(autoplay) Now=Now->next;
}
void TScreenItem::destroy(){
    Anim=Now=0;
}
