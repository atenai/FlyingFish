#ifndef _XMODEL_H_
#define _XMODEL_H_

#include <d3dx9.h>
#include <d3d9.h>


typedef struct
{
	char fileName[256];
	int width;
	int height;
}TEXTUREMODEL;

typedef enum
{
	TEXTUREMODEL_GANDAMU,
	TEXTUREMODEL_MAX
}TEXTUREMODEL_ENUM;

#define MAX_DECLLENGTH (32);

void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);

D3DXVECTOR3 GetTGT(void);
D3DXVECTOR3 GetPos(void);
void ataricoin(int coinsu, float ix, float iy, float iz);//Coin.cppの処理をatarihantei.cppへ数字を送る
void ataricoin10(int coinsu, float ix, float iy, float iz);//Coin.cppの処理をatarihantei.cppへ数字を送る
void atarikabe(int kabesu, float ix, float iy, float iz);//Coin.cppの処理をatarihantei.cppへ数字を送る
void atariitem(int itemsu, float ix, float iy, float iz);//Item.cppの処理をatarihantei.cppへ数字を送る
void ataritatemono(int tatemonosu, float ix, float iy, float iz);//Item.cppの処理をatarihantei.cppへ数字を送る
#endif _XMODEL_H_