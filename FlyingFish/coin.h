#ifndef _COIN_H_
#define _COIN_H_

#include <d3dx9.h>
#include <d3d9.h>
#include"Xmodel.h"

#define COIN_HABA (20.0f)
#define COIN_NUM (600)//�����ŉ��񃋁[�v���邩�̐���������

void Initcoin(void);
void Uninitcoin(void);
void Updatecoin(void);
void Drawcoin(void);

void getcoin(int coinnum);//�R�C�����𑗂�֐�
void getcoin1(int coinnum);//�R�C�����𑗂�֐�
#endif _COIN_H_