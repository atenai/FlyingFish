#pragma once
#ifndef _COIN10_H_
#define _COIN10_H_

#include <d3dx9.h>
#include <d3d9.h>
#include"Xmodel.h"


#define COIN10_HABA (10.0f)
#define COIN10_NUM (400)//ここで何回ループするかの数字を入れる


void Initcoin10(void);
void Uninitcoin10(void);
void Updatecoin10(void);
void Drawcoin10(void);

void getcoin(int coinnum);//コイン数を送る関数
void getcoin1(int coinnum);//コイン数を送る関数
#endif _COIN10_H_