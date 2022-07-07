#ifndef _SHADOW_H_
#define _SHADOW_H_


typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;  //法線
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;	//テクスチャ
}VERTEX3D;

#define MAX_SHADOW (1024)



void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);





#endif _SHADOW_H_