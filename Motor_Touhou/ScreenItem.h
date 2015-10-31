#ifndef Motor_Frame
#define Motor_Frame
#include <math.h>
#include <stdio.h>
#include <GL/glfw.h>
struct TFrame{
    unsigned int content;
    TFrame *next;
};
class TScreenItem{
private:
    TFrame *Anim,*Now;
public:
    bool autoplay;
    void nextframe();
    TScreenItem *Next;
    float hwid,hhei,ang;
    float texpos[4][2];
    float cor[4][2],cen[2];
    TScreenItem();
    void loadframe(TFrame* tar,const char* path);
    void copyframe(TFrame* tar,TFrame* ins);
    void copyitem(TScreenItem* tar);
    void init();
    void drawGL();
    void destroy();
    void reset();
    void reinit();
};
#endif // Motor_defines
