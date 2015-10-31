#ifndef Motor_defines
#define Motor_defines
#include <stdlib.h>
#define yzl 3.14159
#define eps 1e-5
#define gener 1.41421
#define CAMERA_WIDTH 400
#define CAMERA_HEIGHT 560
#define CAMERA_THICK 20
#define GAME_FPS 600
#define abs(x) ((x)>0?(x):(-x))
#define POWERUP_FALLSPEED 0.08
#define HEROBOMB_GENTIME 600
#define HEROBOMB_FADETIME 600
#define HEROBOMB_HALFWIDTH 75
#define HERO_MAXPOWER 500
#define HERO_DIETIME 200
#define HEROAMMO_SPEED 0.75
#define HEROAMMO_GENTIME 60
#define HEROAMMO_FADETIME 60
#define HEROAMMO_RADIUS 30
inline float dist(float* a,float* b){
    return((b[1]-a[1])*(b[1]-a[1])+(b[0]-a[0])*(b[0]-a[0]));
}
inline float sqr(float a){return (a*a);}
#endif // Motor_defines
