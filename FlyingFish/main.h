//━━━━━━━━━━━━━━━━━━━━━━━━
//mainのヘッダファイル
//━━━━━━━━━━━━━━━━━━━━━━━━
#ifndef _MAIN_H_
#define _MAIN_H_

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#define SCREEN_WIDTH (1366)
#define SCREEN_HEIGHT (768)

#define TEXTURE_REAL		(0)
#define TEXTURE_MIRROR		(1)


//━━━━━━━━━━━━━━━━━━━━━━━━
//プロトタイプ宣言
//━━━━━━━━━━━━━━━━━━━━━━━━
LPDIRECT3DDEVICE9 getD3DDevice(void);//いじらない

#endif