#ifndef _POLYGON3D_H_
#define _POLYGON3D_H_

#include "main.h"
#include <d3dx9.h>

#define CUBE_HABA (60.0f)
#define CUBE_NUM	(5)
#define CUBE_1_X (0.0f)
#define CUBE_1_Y (200.0f)
#define CUBE_1_Z (450.0f)
#define CUBE_2_X (20.0f)
#define CUBE_2_Y (320.0f)
#define CUBE_2_Z (600.0f)
#define CUBE_3_X (-200.0f)
#define CUBE_3_Y (500.0f)
#define CUBE_3_Z (800.0f)
#define CUBE_4_X (200.0f)
#define CUBE_4_Y (430.0f)
#define CUBE_4_Z (150.0f)
#define CUBE_5_X (-500.0f)
#define CUBE_5_Y (500.0f)
#define CUBE_5_Z (130.0f)

typedef struct
{
	D3DXVECTOR3 Rot;
	D3DXVECTOR3 Normal;
	D3DXVECTOR3 Mov;
	D3DXVECTOR3 Scale;
}MATRIX_CUBE;

void InitPolygon3D(void);
void UninitPolygon3D(void);
void UpdatePolygon3D(void);
void DrawPolygon3D(void);
void Set3DTexture(int texNum);

#endif _POLYGON3D_H_