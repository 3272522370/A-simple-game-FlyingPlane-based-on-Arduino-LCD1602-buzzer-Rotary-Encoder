#ifndef FLYINGPLANE_H
#define FLYINGPLANE_H
/*
地图大小4x16,液晶屏大小2x16
*/
extern int MAP[4][16];
enum{
  None=0,Pillar=1,Plane=2
};
void Initial_game();
//初始化地图，飞机从左到右飞，然后出现墙。
int Pillar_loop();//返回0游戏结束，返回1加分，返回2啥也不干
int Plane_loop(int key);//返回0游戏结束，返回1表示存活。
#endif