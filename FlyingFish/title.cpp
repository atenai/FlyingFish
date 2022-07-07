#include <d3dx9.h>
#include "main.h"
#include "input.h"
#include "Polygon.h"
#include "Scene.h"
#include "title.h"
#include"lighting.h"
#include "UI.h"
#include "sound.h"

static float migi = -958;//
static int count = 0;//音楽用変数
static int count1 = 0;//音楽用変数

void InitTitle(void)
{
	count = 0;//音楽用変数の初期化
	count1 = 0;//音楽用変数の初期化
	InitSound(NULL);//音楽を初期化
}

void UninitTitle(void)
{
	UninitSound();//音楽の終了処理
}

void UpdateTitle(void)
{
	if (count == 0)
	{	
		PlaySound(SOUND_LABEL_BGM000);//音楽をならす
	}
	count++;

	//シーン切り替え
	if (GetKeyboardTrigger(DIK_RETURN))
	{

		if (count1 == 0)
		{
			PlaySound(SOUND_LABEL_BGM004);//音楽をならすok
		}
		count1++;
		
		ChangeScene(SCENE_GAME);
	}
	
	//爆撃機移動
	migi = migi + 3;
	if (migi > 1500)
	{
		migi = -958;
	}

}

void DrawTitle(void)
{
	//SetUIColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	DrawUI(TEX_4, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1052, 727);//タイトル画面

	
		DrawUI(
			TEX_11,//使うテクスチャ,
			migi, //x座標,
			550,//y座標,
			958,//テクスチャの表示する横の長さ,//元の画像の横の長さは32
			110,//テクスチャの表示する縦の長さ,//元の画像の縦の長さは32
			0,//元の画像の切り取り開始場所（横）（画像の左から数える）
			0,//元の画像の切り取り開始場所（縦）（画像の上から数える）
			958,//元の画像の切り取りたい幅
			110 //元の画像の切り取りたい高さ
		);


}