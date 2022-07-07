#include <d3dx9.h>
#include "main.h"
#include "input.h"
#include "Polygon.h"
#include "Scene.h"
#include "result.h"
#include "UI.h"
#include "score2.h"
#include "coin.h"
#include "title.h"
#include "sound.h"

static int coin = 0;//コイン用変数

static int count = 0;//音楽用変数
static int count1 = 0;//音楽用変数

//テクスチャピコピコ
bool g_bPress = true;//タイトルピコピコ
int g_nCnt = 0;//
//テクスチャピコピコ

void InitResult(void)
{
	count = 0;//音楽用変数の初期化
	count1 = 0;//音楽用変数の初期化
	InitSound(NULL);//音楽を初期化

}
void UninitResult(void)
{
	coin = 0;//コイン初期化
	//テクスチャピコピコ
	g_bPress = true;//タイトルピコピコ
	g_nCnt = 0;//
	//テクスチャピコピコ


	UninitSound();//音楽の終了処理
}
void UpdateResult(void)
{

	if (count == 0)
	{
		PlaySound(SOUND_LABEL_BGM008);//音楽をならす
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

		ChangeScene(SCENE_TITLE);
	}
}

void DrawResult(void)
{
	SetUIColor(D3DCOLOR_RGBA(255,255,255,255));
	DrawUI(TEX_3, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1000, 500);


	//テクスチャピコピコ
	g_nCnt++;
	if (g_nCnt % 60 == 0)
	{
		switch (g_bPress)
		{
		case false:
			g_bPress = true;
			break;
		case true:
			g_bPress = false;
			break;
		}
	}

	if (g_bPress == true)
	{
		SetUIColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		DrawScore2(coin, 3, 300.0f, 300.0f, true);
	}
	//テクスチャピコピコ
}

//coin.hから数字を持ってくる関数
void getcoin1(int coinnum)
{
	coin = coinnum;//coin.hから数字を持ってきてcoinに入れる処理
}