//=============================================================================
//
// サウンド処理 [sound.h]
//
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,//タイトル
	SOUND_LABEL_BGM001,//ゲームプレイ
	SOUND_LABEL_BGM002,//壁・キューブ
	SOUND_LABEL_BGM003,//赤・青・黄コイン
	SOUND_LABEL_BGM004,//決定オン
	SOUND_LABEL_BGM005,//アイテム
	SOUND_LABEL_BGM006,//コクーンタワー
	SOUND_LABEL_BGM007,//発射音
	SOUND_LABEL_BGM008,//リザルト
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);//特定の音楽だけを止める
void StopSound(void);//全ての音楽を止める

#endif
