#pragma once
#pragma once
#ifndef _TATEMONO_H_
#define _TATEMONO_H_

#include <d3dx9.h>
#include <d3d9.h>
#include"Xmodel.h"

#define TATEMONO_HABA (42.0f)
#define TATEMONO_NUM (5)//�����ŉ��񃋁[�v���邩�̐���������

void Inittatemono(void);
void Uninittatemono(void);
void Updatetatemono(void);
void Drawtatemono(void);

#endif _COIN_H_