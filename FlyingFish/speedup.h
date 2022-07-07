#ifndef _SPEEDUP_H_
#define _SPEEDUP_H_

#include <d3dx9.h>
#include <d3d9.h>
#include"Xmodel.h"

#define ITEM_HABA (20.0f)
#define SPEEDUP_NUM (100)//ここで何回ループするかの数字を入れる

void InitSpeedUp(void);
void UninitSpeedUp(void);
void UpdateSpeedUp(void);
void DrawSpeedUp(void);

#endif _SPEEDUP_H_
