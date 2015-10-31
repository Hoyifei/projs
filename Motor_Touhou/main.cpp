#ifndef Motor_Main
#define Motor_Main
#include <GL/glfw.h>
#include <stdio.h>
#include "GameBoard.h"
int width,height;
bool running;
TGameBoard gmMain;
TScreenItem bkg;
TFrame frm;
int winclose(){
    running=0;
    return(0);
}
void GLFWCALL drawGame(void* arg){
    double time_b,time_e;
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE,GL_TRUE);
    if( !glfwOpenWindow( width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) )
    {
        glfwTerminate();
        running=0;
        return;
    }
    glfwSetWindowTitle("Indev");
    glfwSetWindowCloseCallback(winclose);
    glOrtho(0,800,0,600,1,-1);//left_down is (0,0)
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    gmMain.LoadStage();
    while(running){
        time_b=glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        gmMain.drawBoard();
        printf("DrawBoard returned\n");
        glfwSwapBuffers();
        printf("BufferSwap End\n");
        time_e=glfwGetTime();
        if(time_e-time_b<=0.01667) glfwSleep(0.01667-time_e+time_b);
        //else record as a uncorrect frame;
    }
}
void GLFWCALL loadGame(void* arg){
    //printf("Game-loading thread created!\n");
    gmMain.LoadStage();
}
void GLFWCALL updateGame(void* arg){
    double time_b,time_e;
    glfwInit();
    while(running){
        time_b=glfwGetTime();
        gmMain.updateBoard();
        time_e=glfwGetTime();
        if(time_e-time_b<=0.00333) glfwSleep(0.00333-time_e+time_b);
        //printf("Frame Update Finished,time %lf\n",time_e-time_b);
        //else record as a uncorrect frame;
    }
}
int main()
{
    freopen("fuck.log","w",stdout);
    running = 1;
    glfwInit();
    width=800;height=600;
    gmMain.Locker=glfwCreateMutex();
    gmMain.prep();
    /*bkg.loadframe(&frm,"img/stage/test/test1.tga");
    bkg.init();bkg.cen[0]=400;bkg.cen[1]=300;bkg.hwid=200;bkg.hhei=300;
    char path1[18]="etc/stagetest.txt";*/
    while(running)
    {
        switch (gmMain.bStat){
            case Nothing:
                printf("Creat Threads to load game\n");
                sprintf(gmMain.stagepath,"%s","etc/stagetest.txt");
                //glfwCreateThread(loadGame,0);
                glfwCreateThread(drawGame,0);
                gmMain.bStat=Loading;
                printf("Load-game Threads were created\n");
                break;
            case LoadFinish:
                printf("Loading complete,Create a thread to update Game\n");
                glfwCreateThread(updateGame,0);
                gmMain.bStat=Gaming;
                printf("Game-updating thread creted\n");
            break;
            case Loading:
                //drawGame(0);break;
            case Gaming:
                //updateGame(0);drawGame(0);
            break;
        }
        //glfwPollEvents();
        /*glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        bkg.drawGL();
        glfwSwapBuffers();
        glfwSleep(0.01667);*/
    }

    glfwTerminate();

    return 0;
}
#endif // Motor_Main
