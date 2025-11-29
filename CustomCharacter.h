#ifndef CUSTOMCHARACTER
#define CUSTOMCHARACTER
/*
每个格子三种占用,空,柱子,飞机。
所以一个字符有3x3-1=8种可能。（没有两个飞机）
*/
extern byte None_None[8];
extern byte None_Pillar[8];
extern byte None_Plane[8];
extern byte Pillar_None[8];
extern byte Pillar_Pillar[8];
extern byte Pillar_Plane[8];
extern byte Plane_None[8];
extern byte Plane_Pillar[8];
//上列字符的标记为0-7
#endif