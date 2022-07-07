#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_
#include <d3dx9.h>
#include <d3d9.h>

#include "polygon3DTest.h"
#include "Polygon.h"

static D3DXMATRIX g_mtxfield;


bool InitMeshField(int x, int z, float Sx, float Sz);
//bool InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

#endif 