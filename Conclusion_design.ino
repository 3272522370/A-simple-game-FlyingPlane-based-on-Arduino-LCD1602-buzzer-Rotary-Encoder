#include <LiquidCrystal.h>
#include"CustomCharacter.h"
#include"FlyingPlane.h"
int interval=5,score=0,CNT=0;
// LY-51S引脚定义（4位模式仅用DB4~DB7）
const int d4 = 4;//P04
const int d5 = 5;//P05
const int d6 = 6;//P06
const int d7 = 7;//P07
const int rs = 8;    // LCD的RS接P24
//P25接地
const int enable = 9;// LCD的E接P26
// 旋转编码器引脚定义
const int clkPin = 10;
const int dtPin = 11;
const int swPin = 12;//这个可以不接
const int speakerPin = 13;
// RW引脚直接接地（仅输出功能，无需接开发板）
LiquidCrystal lcd(rs, enable, d4, d5, d6, d7);
void playTone(int frequency, int duration) {
  int period = 1000000 / frequency; // 计算方波周期（单位us）
  int halfPeriod = period / 2;      // 半周期（高低电平各占一半）
  unsigned long endTime = millis() + duration;
  
  while (millis() < endTime) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(halfPeriod);
  }
}
int readEncoderDir() {
  static int lastClkState = HIGH; // 静态变量保存上一次CLK状态（仅初始化一次）
  int currentClkState = digitalRead(clkPin);
  int dir = 0; // 默认无旋转

  // 仅当CLK状态变化时判断方向
  if (currentClkState != lastClkState) {
    if (digitalRead(dtPin) != currentClkState) {
      dir = 2; // 顺时针旋转,DOWN
    } else {
      dir = 1; // 逆时针旋转,UP
    }
    lastClkState = currentClkState; // 更新CLK状态
  }
  return dir;
}
void draw_map(){
  int ul,dl;//储存上面格子与下面格子
  for(int row=0;row<2;row++){
    for(int col=0;col<16;col++){
      ul=MAP[row*2][col];
      dl=MAP[row*2+1][col];
      lcd.setCursor(col,row);
      //None=0,Pillar=1,Plane=2
      if(ul==None&&dl==None)lcd.write((byte)0);
      else if(ul==None&&dl==Pillar)lcd.write((byte)1);
      else if(ul==None&&dl==Plane)lcd.write((byte)2);
      else if(ul==Pillar&&dl==None)lcd.write((byte)3);
      else if(ul==Pillar&&dl==Pillar)lcd.write((byte)4);
      else if(ul==Pillar&&dl==Plane)lcd.write((byte)5);
      else if(ul==Plane&&dl==None)lcd.write((byte)6);
      else if(ul==Plane&&dl==Pillar)lcd.write((byte)7);
    }
  }
}
void setup() {
  pinMode(clkPin,INPUT);
  pinMode(dtPin,INPUT);
  pinMode(speakerPin, OUTPUT);
  lcd.begin(16, 2);  // 设置LCD为16列2行
  // 将自定义像素字符写入LCD的CGRAM（编号0~3）
  lcd.createChar(0, None_None);
  lcd.createChar(1, None_Pillar);
  lcd.createChar(2, None_Plane);
  lcd.createChar(3, Pillar_None);
  lcd.createChar(4, Pillar_Pillar);
  lcd.createChar(5, Pillar_Plane);
  lcd.createChar(6, Plane_None);
  lcd.createChar(7, Plane_Pillar);
  lcd.clear();
  lcd.noCursor();
}
void loop() {
  //难度选择界面
  int difficulty=0;//Easy=0,Medium=1,Hard=2
  int interval=5,score=0,CNT;
  for(int key=0;1;){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("difficulty:");
    lcd.setCursor(0,1);
    if(difficulty==0){
      lcd.print("<Easy>");
      CNT=30;
    }else if(difficulty==1){
      lcd.print("<Medium>");
      CNT=20;
    }else{
      lcd.print("<Hard>");
      CNT=10;
    }
    key=readEncoderDir();
    if(key==1)difficulty=(difficulty+1)%3;
    else if(key==2)break;
    delay(10);
  }
  if(difficulty==0)CNT=30;
  else if(difficulty==1)CNT=20;
  else CNT=10;
  //游戏界面
  Initial_game();
  for(int cnt=30,result=0,key=0; 1 ; cnt--){
    if(cnt==0){
      result=Pillar_loop();//返回0游戏结束，返回1加分，返回2啥也不干
      if(result==0){
        playTone(1100, 1000);
        break;
      }
      else if(result==1){
        score++;
        playTone(1100, 200);
      }
      cnt=CNT;
    }
    key=readEncoderDir();
    result=Plane_loop(key);
    if(result==0){
        playTone(1100, 1000);
        break;
      }
    draw_map();
    delay(interval);
  }
  //游戏结束画面
  while(1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Score:");
    lcd.print(score);
    lcd.setCursor(0,1);
    lcd.print("Continue?");
    if(readEncoderDir()!=0)break;
    delay(10);
  }
  
}