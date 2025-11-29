#include<Arduino.h>
#include"FlyingPlane.h"
int MAP[4][16];
int Pr=1,Pc=0;
int Counter=3;//倒计时，计算什么时候生成新的墙
void Initial_game(){
  randomSeed(analogRead(A0));
  for(int i=0;i<4;i++){
    for(int j=0;j<16;j++)MAP[i][j]=None;
  }
  MAP[1][0]=Plane;
  Counter=3,Pr=1,Pc=0;;
}
//返回0游戏结束，返回1加分，返回2啥也不干
int Pillar_loop(){
  //控制飞机从左到右运动
  if(Pc<4){
    MAP[Pr][Pc++]=None;
    MAP[Pr][Pc]=Plane;
  }
  //让地图往左移动（除了飞机）
  for(int i=0;i<4;i++){
    for(int j=0;j<15;j++){//注意是j<15而不是16
      if(MAP[i][j+1]==Plane){
        MAP[i][j]=None;
        continue;
      }
      if(MAP[i][j]==Plane){
        if(MAP[i][j+1]==Pillar)return 0;//MAN!
        continue;
      }
      MAP[i][j]=MAP[i][j+1];
    }
  }
  //生成新的墙
  if(Counter==0){
    int Pass=random(4);
    for(int i=0;i<4;i++){
      if(i==Pass)MAP[i][15]=None;
      else MAP[i][15]=Pillar;
    }
    Counter=random(3,5);
  }else{
    Counter--;
    for(int i=0;i<4;i++)MAP[i][15]=None;
  }
  //判断是否越过了障碍，判断加分
  if(MAP[0][Pc-1]==Pillar||MAP[1][Pc-1]==Pillar)return 1;
  return 2;
}
//返回0游戏结束，返回1表示存活。
int Plane_loop(int key){//0什么也没按，1为UP，2为DOWN
  //判断飞机是否上下移动，同时判断是否坠机
  MAP[Pr][Pc]=None;
  if(key==1&&Pr>0)Pr--;
  if(key==2&&Pr<3)Pr++;
  if(MAP[Pr][Pc]==Pillar)return 0;//MAN!
  else MAP[Pr][Pc]=Plane;
  return 1;
}