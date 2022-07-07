#ifndef _COIN_H_
#define _COIN_H_

#include <d3dx9.h>
#include <d3d9.h>
#include"Xmodel.h"

#define COIN_HABA (20.0f)
#define COIN_NUM (600)//ここで何回ループするかの数字を入れる

void Initcoin(void);
void Uninitcoin(void);
void Updatecoin(void);
void Drawcoin(void);

void getcoin(int coinnum);//コイン数を送る関数
void getcoin1(int coinnum);//コイン数を送る関数
#endif _COIN_H_