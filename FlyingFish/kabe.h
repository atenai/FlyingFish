#pragma once
#ifndef _KABE_H_
#define _KABE_H_

#include <d3dx9.h>
#include <d3d9.h>
#include"Xmodel.h"

#define KABE_HABA (5.0f)
#define KABE_NUM (200)//�����ŉ��񃋁[�v���邩�̐���������

void Initkabe(void);
void Uninitkabe(void);
void Updatekabe(void);
void Drawkabe(void);

#endif _COIN_H_