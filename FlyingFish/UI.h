#pragma once
#pragma once
//=============================================================================
//  
// UI描画
//
//　      
//=============================================================================

#ifndef _UI_H_
#define _UI_H_

typedef struct
{
	char fileName[256];
	int width;
	int height;
}TEX;

//テクスチャ列挙型(0)
typedef enum
{
	TEX_1 = 0,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,
	TEX_8,
	TEX_9,
	TEX_10,
	TEX_11,
	TEX_MAX
}TEXTURE_NUM;
//テクスチャの指定(1)

bool InitUI(void);//ポリゴンの初期化処理
void UninitUI(void);//ポリゴンの終了処理
void UpdateUI(void);//ポリゴンの更新処理
void SetUIColor(D3DCOLOR color);//ポリゴンカラーの設定
void DrawUI(//画像を表示
	int texNum,//使うテクスチャ,
	float dx, //描画位置X軸
	float dy, //描画位置Y軸
	int dw, //テクスチャの表示する横の長さ
	int dh, //テクスチャの表示する縦の長さ
	int tcx, //元の画像の切り取り開始場所（横）（画像の左から数える）
	int tcy, //元の画像の切り取り開始場所（縦）（画像の上から数える）
	int tcw, //元の画像の切り取りたい幅
	int tch //元の画像の切り取りたい高さ
);//ポリゴンの描画処理

#endif // _UI_H_


