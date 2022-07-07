#include "Xmodel.h"
#include "main.h"
#include <d3dx9.h>
#include <d3d9.h>
#include <math.h>
#include "polygon.h"
#include "camera.h"
#include "input.h"
#include "polygon.h"
#include "judge.h"
#include "polygon3DTest.h"
#include "speedup.h"
#include "coin.h"
#include "coin10.h"
#include "kabe.h"
#include "target.h"
#include "Scene.h"
#include "title.h"
#include "tatemono.h"
#include "UI.h"
#include "sound.h"

#define INIT_POSY	(0.8f)		//最初のY座標
#define INIT_POSZ	(1.0f)		//最初のZ座標
#define GRAVITY		(-0.015f)	//重力
#define FRICTION	(0.1f)		//摩擦係数
#define REBOUND		(0.15f)		//地面で跳ねる係数	
#define JUMP_FORCE	(0.016f)	//溜める力の速さ
#define JUMP_LIMIT	(80)
#define JUMP_Y		(3.4f)		//Y方向係数
#define JUMP_Z		(2.7f)		//Z方向係数
#define PLAYER_R	(5.0f)		//Playerの球体半径
#define CUBE_JUDGE_R (CUBE_HABA*0.7f)	//CUBEの外接円半径
#define KAITEN_KAISUU	(7*360)
#define MITIGATE_SPEED	(2.0f)
#define HUKUSEI_NUM (390)//ビルあたり判定の複製数

//Xモデル表示用変数
static LPD3DXMESH pMesh = NULL;
static DWORD dwNumMaterials = 0;
static D3DMATERIAL9* pMeshMaterials = NULL;
static LPDIRECT3DTEXTURE9* pMeshTextures = NULL;


static D3DXMATRIX g_mtxWorld;
static D3DXMATRIX g_mtxWorldS;
static D3DXMATRIX g_mtxWorldT;
static D3DXMATRIX g_mtxWorldR;
static D3DXMATRIX g_mtxWorldRY;
static D3DXMATRIX g_mtxWorldRZ;

static D3DXVECTOR3 g_fieldMin(-5.0f, 0, -5.0f);

static D3DXVECTOR3 g_Pos(0.0f, INIT_POSY, INIT_POSZ);   //xmod pos

static float g_fKaitenZ = 0;//Z回転用数値を入れる箱
static float g_fKaitenSpeed = 32.0f;//回転のスピード
static bool g_bKaitenZON = false;//回転スイッチ
static int g_nCheckZ = 0;	//回転終了時に元に戻るために必要な変数。スピードアップ終了時点の角度を取る。

static float Rot = 0.0f;
static float NewRot = 0.0f;
static float OldRot = 0.0f;


static float g_fTransSpeed = 0.0f;
static float g_fTestSpeedX = 0.0f;
static float g_fTestSpeedY = 0.0f;


static float g_fPressSP = 0.0f;
static float g_fAngle = 0.0f;	//発射時点の左右角度

static float g_fGravity = GRAVITY;
static bool g_bLaunch = false;
static int g_nChargeCnt = 0;
static bool g_bCharge = false;

static bool g_bPressH = false;

static int oto0;//音楽用変数
static int oto1;//音楽用変数
static int oto2;//音楽用変数
static int oto3;//音楽用変数
static int oto4;//音楽用変数
static int oto5;//音楽用変数
static int oto6;//音楽用変数
static int oto7;//音楽用変数
static int oto8;//音楽用変数

//構造体を回す（ループ）させるための変数
static int Ui = 0;//構造体をループさせるための変数

//コイン用変数
static float coin = 0;//コイン用変数

//コイン///////////////////////////////////////
typedef struct
{
	float ataricoinx;//ボンベの座標x
	float ataricoiny;//ボンベの座標y
	float ataricoinz;//ボンベの座標z
	bool tadancoin;//多段ヒット修正
}COIN_ATARI;

static COIN_ATARI Coinatari[COIN_NUM];//構造体でアイテムをいくつ作るかを宣言

//コイン///////////////////////////////////////

//コイン10///////////////////////////////////////
typedef struct
{
	float ataricoin10x;//ボンベの座標x
	float ataricoin10y;//ボンベの座標y
	float ataricoin10z;//ボンベの座標z
	bool tadancoin10;//多段ヒット修正
}COIN10_ATARI;

static COIN10_ATARI Coin10atari[COIN10_NUM];//構造体でアイテムをいくつ作るかを宣言

//コイン10///////////////////////////////////////

//壁///////////////////////////////////////
typedef struct
{
	float atarikabex;//ボンベの座標x
	float atarikabey;//ボンベの座標y
	float atarikabez;//ボンベの座標z
	bool tadankabe;//多段ヒット修正
}KABE_ATARI;

static KABE_ATARI Kabeatari[KABE_NUM];//構造体でアイテムをいくつ作るかを宣言
//壁///////////////////////////////////////

//アイテム///////////////////////////////////////
typedef struct
{
	float atariitemx;//ボンベの座標x
	float atariitemy;//ボンベの座標y
	float atariitemz;//ボンベの座標z
	bool tadanitem;//多段ヒット修正
	bool g_bKaitenZON;//回転スイッチ
}ITEM_ATARI;

static ITEM_ATARI Itematari[SPEEDUP_NUM];//構造体でアイテムをいくつ作るかを宣言
//アイテム///////////////////////////////////////

//コクーンタワー/////////////////////////////////
static bool tadankokun = true;
//コクーンタワー

//キューブ
static bool tadancube = true;
//キューブ

//建物///////////////////////////////////////
typedef struct
{
	float ataritatemonox;//ボンベの座標x
	float ataritatemonoy;//ボンベの座標y
	float ataritatemonoz;//ボンベの座標z
	bool tadantatemono;//多段ヒット修正
}TATEMONO_ATARI;

static TATEMONO_ATARI Tatemonoatari[TATEMONO_NUM];//構造体でアイテムをいくつ作るかを宣言
//建物///////////////////////////////////////

//あたり判定複数化//////////////////////////
typedef struct {
	float atarisu;
}HUKUSEI_ATARI;

static HUKUSEI_ATARI Atarihukusei[HUKUSEI_NUM];//構造体でアイテムをいくつ作るかを宣言

int kuri = 0;
//あたり判定複数化//////////////////////////

//障害物の座標
static D3DXVECTOR3 v3PosCube[CUBE_NUM] = 
{ 
	{ CUBE_1_X, CUBE_1_Y ,CUBE_1_Z },
	{ CUBE_2_X ,CUBE_2_Y ,CUBE_2_Z },
	{ CUBE_3_X, CUBE_3_Y ,CUBE_3_Z },
	{ CUBE_4_X ,CUBE_4_Y ,CUBE_4_Z },
	{ CUBE_5_X, CUBE_5_Y ,CUBE_5_Z } 
};

//テクスチャ
static LPDIRECT3DTEXTURE9 g_pTextures[] = { NULL };

void InitModel(void)
{
	oto0 = 0;//音楽用変数初期化
	oto1 = 0;//音楽用変数初期化
	oto2 = 0;//音楽用変数初期化
	oto3 = 0;//音楽用変数初期化
	oto4 = 0;//音楽用変数初期化
	oto5 = 0;//音楽用変数初期化
	oto6 = 0;//音楽用変数初期化
	oto7 = 0;//音楽用変数初期化
	oto8 = 0;//音楽用変数初期化

	coin = 0;//コイン初期化

	for (Ui = 0; Ui < COIN_NUM; Ui++)//コイン構造体の数だけ回す 
	{
		//	Coinatari[Ui].ataricoinx = 0;
		//	Coinatari[Ui].ataricoiny = 0;
		//	Coinatari[Ui].ataricoinz = 0;
		Coinatari[Ui].tadancoin = true;//多段ヒット修正
	}

	for (Ui = 0; Ui < COIN10_NUM; Ui++)//コイン10構造体の数だけ回す 
	{
		//	Coinatari[Ui].ataricoinx = 0;
		//	Coinatari[Ui].ataricoiny = 0;
		//	Coinatari[Ui].ataricoinz = 0;
		Coin10atari[Ui].tadancoin10 = true;//多段ヒット修正
	}

	for (Ui = 0; Ui < KABE_NUM; Ui++)//壁構造体の数だけ回す 
	{
		//	Itematari[Ui].atariitemx = 0;
		//	Itematari[Ui].atariitemy = 0;
		//	Itematari[Ui].atariitemz = 0;
		Kabeatari[Ui].tadankabe = true;//多段ヒット修正
	}

	for (Ui = 0; Ui < SPEEDUP_NUM; Ui++)//アイテム構造体の数だけ回す 
	{
	//	Itematari[Ui].atariitemx = 0;
	//	Itematari[Ui].atariitemy = 0;
	//	Itematari[Ui].atariitemz = 0;
		Itematari[Ui].tadanitem = true;//多段ヒット修正
		Itematari[Ui].g_bKaitenZON = false;
	}
	//コクーンタワー
	tadankokun = true;//多段ヒット修正
	//建物
	for (Ui = 0; Ui < TATEMONO_NUM; Ui++)//壁構造体の数だけ回す 
	{
		//	Itematari[Ui].atariitemx = 0;
		//	Itematari[Ui].atariitemy = 0;
		//	Itematari[Ui].atariitemz = 0;
		Tatemonoatari[Ui].tadantatemono = true;//多段ヒット修正
	}

	//あたり判定複製
	for (Ui = 0; Ui < HUKUSEI_NUM; Ui++)//あたり判定複製構造体の数だけ回す 
	{
		//	Itematari[Ui].atariitemx = 0;
		//	Itematari[Ui].atariitemy = 0;
		//	Itematari[Ui].atariitemz = 0;
		Atarihukusei[Ui].atarisu = Atarihukusei[Ui - 1].atarisu + 1;//前のあたり判定の位置から+1する
	}

	//キューブ
	tadancube = true;//多段ヒット修正

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();


	//Xファイルからメッシュをロードする
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX("./resource/xmodel/fish_plus.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&dwNumMaterials, &pMesh)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", NULL, MB_OK);
		return;
	}

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];//マテリアルのメモリ領域をマテリアルの数だけ確保しpMeshMaterialsに入れる
	pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];//テクスチャに必要なメモリ領域を確保しpMeshTexturesに入れる

														   //上で確保したメモリ領域に中身を入れていく
	for (DWORD i = 0; i < dwNumMaterials; i++) //マテリアルの数だけループする
	{
		pMeshMaterials[i] = d3dxMaterials[i].MatD3D;//マテリアルデータを入れる
		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;//AmbientをDiffuseと同じ値にする

															  //pTextureFilenameをもとにテクスチャオブジェクトの生成
		pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL && lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				d3dxMaterials[i].pTextureFilename,
				&pMeshTextures[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
			}
		}
	}
	pD3DXMtrlBuffer->Release();//メモリの開放
};

void UninitModel(void)
{
	if (pMesh != NULL)
	{
		pMesh->Release();//これで開放される
		pMesh = NULL;//アドレスが残っているのでNULLで上書き
	}

	g_Pos = { 0.0f, INIT_POSY, INIT_POSZ };
	g_fTransSpeed = 0.0f;
	g_fTestSpeedX = 0.0f;
	g_fTestSpeedY = 0.0f;
	g_fPressSP = 0.0f;
	g_fAngle = 0.0f;
	g_bLaunch = false;
	g_nChargeCnt = 0;
	g_bCharge = false;
	
	g_fKaitenZ = 0;//Z回転用数値を入れる箱
	g_fKaitenSpeed = 32.0f;//回転のスピード
};

void UpdateModel(void)
{
	//カメラが高い所から望む
	if (g_bLaunch == false)
	{
		if (GetKeyboardPress(DIK_H))
		{
			if (g_bPressH == false)
			{
				g_bPressH = true;
				g_Pos.y = 840.0f;
			}

		}
		else
		{
			g_bPressH = false;
			g_Pos.y = INIT_POSY;
		}
	}

	//発射時点の左右操作
	if (GetKeyboardPress(DIK_Q) && g_bLaunch == false)
	{
		g_fAngle -= 0.4f;

		if (g_fAngle <= -10)
		{
			g_fAngle = -10;
		}
	}

	if (GetKeyboardPress(DIK_E) && g_bLaunch == false)
	{
		g_fAngle += 0.4f;

		if (g_fAngle >= 10)
		{
			g_fAngle = 10;
		}
	}

	//飛行中の左右操作
	if (GetKeyboardPress(DIK_A) && g_bLaunch == true && g_fTransSpeed != 0)
	{
		g_fTestSpeedX += -0.1f;
		Rot += 2.0f;
		if (Rot >= 45)
		{
			Rot = 45;
		}
	}

	if (GetKeyboardPress(DIK_D) && g_bLaunch == true && g_fTransSpeed != 0)
	{
		g_fTestSpeedX += 0.1f;
		Rot -= 2.0f;
		if (Rot <= -45)
		{
			Rot = -45;
		}
	}

	//角度の戻り
	if (Rot >= 0)
	{
		Rot -= 1.0f;
	}
	if (Rot <= 0)
	{
		Rot += 1.0f;
	}

	//押している間力を溜める
	if (GetKeyboardPress(DIK_SPACE)  && g_bLaunch == false && g_fPressSP < JUMP_FORCE * JUMP_LIMIT && g_bCharge == false && g_bPressH == false)
	{
	

		g_fPressSP += JUMP_FORCE;
		g_nChargeCnt += 4;
	}

	if (g_fPressSP>JUMP_FORCE * JUMP_LIMIT)
	{
		g_bCharge = true;
	}

	if (g_bCharge == true)
	{
		g_fPressSP -= JUMP_FORCE;
		g_nChargeCnt -= 4;
	}

	if (g_fPressSP<0)
	{
		g_bCharge = false;
	}

	//放した瞬間に飛ぶ
	if (GetKeyboardRelease(DIK_SPACE) && g_fPressSP != 0 && g_fTransSpeed == 0  && g_bLaunch == false && g_bPressH == false)
	{
		//サウンド
		oto7++;
		if (oto7 > 0)
		{
			PlaySound(SOUND_LABEL_BGM007);//音楽をならす
		}
		oto7 = 0;
		//サウンド


		g_fTestSpeedY += g_fPressSP * JUMP_Y;
		g_fTransSpeed = g_fPressSP * JUMP_Z;
		g_fPressSP = 0.0f;
		g_bLaunch = true;
	}

	//重力追加
	if (g_Pos.y > 0 && g_bLaunch == true)
	{
		float fG = 0.0f;


			fG = g_fGravity;
			g_fTestSpeedY += fG;
		
	}

	//当たり判定///////////////////////////////////////////////////////////////////////////////////////////////
	//キューブあたり判定
	for (int nCnt = 0; nCnt < CUBE_NUM; nCnt++)
	{
		if (JudgementCheck(g_Pos.x, g_Pos.y, g_Pos.z, v3PosCube[nCnt].x, v3PosCube[nCnt].y, v3PosCube[nCnt].z, PLAYER_R, CUBE_JUDGE_R))
		{
			if (tadancube == true)
			{
				//サウンド
				oto0++;
				if (oto0 > 0)
				{
					PlaySound(SOUND_LABEL_BGM002);//音楽をならすok
				}
				//oto0 = 0;
				//サウンド

				g_fTransSpeed = 0;

				tadancube = false;
			}
		}
	}

	//コインあたり判定		
	for (Ui = 0; Ui < COIN_NUM; Ui++)//構造体の数だけ回す 
	{
		if (JudgementCheck(g_Pos.x, g_Pos.y, g_Pos.z, Coinatari[Ui].ataricoinx , Coinatari[Ui].ataricoiny , Coinatari[Ui].ataricoinz, PLAYER_R, COIN_HABA))
			{
				if (Coinatari[Ui].tadancoin == true)
				{
					//あたった内容
					//サウンド
					oto1++;
					if (oto1 > 0)
					{
						PlaySound(SOUND_LABEL_BGM003);//音楽をならすok
					}
					oto1 = 0;
					//サウンド


					coin = coin + 1;
					getcoin(coin);//getcoinへcoinの数字を送る
					getcoin1(coin);//getcoinへcoinの数字を送る
					Coinatari[Ui].tadancoin = false;//多段ヒット修正
				}
			}
	}

	//コイン10あたり判定		
	for (Ui = 0; Ui < COIN10_NUM; Ui++)//構造体の数だけ回す 
	{
		if (JudgementCheck(g_Pos.x, g_Pos.y, g_Pos.z, Coin10atari[Ui].ataricoin10x, Coin10atari[Ui].ataricoin10y , Coin10atari[Ui].ataricoin10z, PLAYER_R, COIN10_HABA))
		{
			if (Coin10atari[Ui].tadancoin10 == true)
			{
				//あたった内容

				//サウンド
				oto2++;
				if (oto2 > 0)
				{
					PlaySound(SOUND_LABEL_BGM003);//音楽をならすok
				}
				oto1 = 0;
				//サウンド

				coin = coin + 10;
				getcoin(coin);//getcoinへcoinの数字を送る
				getcoin1(coin);//getcoinへcoinの数字を送る
				Coin10atari[Ui].tadancoin10 = false;//多段ヒット修正
			}
		}
	}

	//青コインあたり判定		
	for (Ui = 0; Ui < KABE_NUM; Ui++)//構造体の数だけ回す 
	{
		if (JudgementCheck(g_Pos.x, g_Pos.y, g_Pos.z, Kabeatari[Ui].atarikabex, Kabeatari[Ui].atarikabey, Kabeatari[Ui].atarikabez, PLAYER_R, KABE_HABA))
		{
			if (Kabeatari[Ui].tadankabe == true)
			{
				//あたった内容

				//サウンド
				oto3++;
				if (oto3 > 0)
				{
					PlaySound(SOUND_LABEL_BGM003);//音楽をならすok
				}
				oto1 = 0;
				//サウンド


				//g_fTransSpeed = 0;
				coin = coin + 50;
				getcoin(coin);//getcoinへcoinの数字を送る
				getcoin1(coin);//getcoinへcoinの数字を送る
				Kabeatari[Ui].tadankabe = false;//多段ヒット修正
			}
		}
	}

	//アイテムのあたり判定
	for (Ui = 0; Ui < SPEEDUP_NUM; Ui++)//構造体の数だけ回す 
	{
		if (JudgementCheck(g_Pos.x, g_Pos.y, g_Pos.z, Itematari[Ui].atariitemx, Itematari[Ui].atariitemy , Itematari[Ui].atariitemz, PLAYER_R, ITEM_HABA) == true
			&& Itematari[Ui].g_bKaitenZON == false)
		{
			if (Itematari[Ui].tadanitem == true)
			{
				//
				//coin = coin + 100;
				//getcoin(coin);//getcoinへcoinの数字を送る
				//getcoin1(coin);//getcoinへcoinの数字を送る

				//サウンド
				oto4++;
				if (oto4 > 0)
				{
					PlaySound(SOUND_LABEL_BGM005);//音楽をならす
				}
				oto4 = 0;
				//サウンド

				//あたった場合
				g_fTransSpeed += 1.0f;//Zスピード上げる
				Itematari[Ui].g_bKaitenZON = true;//回転スイッチをtrueにするつまり回転を開始する
				Itematari[Ui].tadanitem = false;//多段ヒット修正
			}
		}

		//回転角度の処理
		if (Itematari[Ui].g_bKaitenZON)
		{
			if (g_fKaitenZ < KAITEN_KAISUU)
			{
				g_fKaitenZ += g_fKaitenSpeed;//Z回転スピードをこの数値にする
				g_fKaitenSpeed *= 0.988f;

				if (g_fKaitenSpeed < MITIGATE_SPEED && ((int)g_fKaitenZ) % 360>30 && ((int)g_fKaitenZ) % 360 < 330)
				{
					g_fKaitenSpeed = MITIGATE_SPEED;
				}

			}
			else
			{
				Itematari[Ui].g_bKaitenZON = false;
			}
		}


		if (g_fTransSpeed <= 0)
		{
			g_bKaitenZON = false;//回転スイッチをfalseにするつまり回転を止める
			g_fKaitenZ = 0;
			g_fKaitenSpeed = 32.0f;//回転のスピード
		}
	}

	for (kuri = 0; kuri < HUKUSEI_NUM; kuri++)//あたり判定を複数作成する
	{
		//コクーンタワー
		if (JudgementCheck(g_Pos.x, g_Pos.y, g_Pos.z, POS_X, POS_Y + Atarihukusei[kuri].atarisu, POS_Z + 10, PLAYER_R, KOKUN_HABA))
		{
			if (tadankokun == true)
			{
				//あたった内容

				//サウンド
				oto5++;
				if (oto5 > 0)
				{
					PlaySound(SOUND_LABEL_BGM006);//音楽をならす
				}
				//oto5 = 0;
				//サウンド

				coin = coin + 100;
				getcoin(coin);//getcoinへcoinの数字を送る
				getcoin1(coin);//getcoinへcoinの数字を送る
				tadankokun = false;//多段ヒット修正
			}
		}
	}
	

	for (kuri = 0; kuri < HUKUSEI_NUM; kuri++)//あたり判定を複数作成する
	{
		//建物あたり判定		
		for (Ui = 0; Ui < TATEMONO_NUM; Ui++)//構造体の数だけ回す 
		{
	
			if (JudgementCheck(g_Pos.x, g_Pos.y, g_Pos.z, Tatemonoatari[Ui].ataritatemonox + 5, Tatemonoatari[Ui].ataritatemonoy + Atarihukusei[kuri].atarisu, Tatemonoatari[Ui].ataritatemonoz - 10, PLAYER_R, TATEMONO_HABA))
			{
				if (Tatemonoatari[Ui].tadantatemono == true)
				{
					//あたった内容

					//サウンド
				oto6++;
				if (oto6 > 0)
				{
					PlaySound(SOUND_LABEL_BGM002);//音楽をならすok
				}
				//oto6 = 0;
				//サウンド

					g_fTransSpeed = 0;
					//coin = coin + 1;
					//getcoin(coin);//getcoinへcoinの数字を送る
					//getcoin1(coin);//getcoinへcoinの数字を送る
					Tatemonoatari[Ui].tadantatemono = false;//多段ヒット修正
				}
			}
		}
	}
	
	//当たり判定///////////////////////////////////////////////////////////////////////////////////////////////

	//リザルト画面へ移行
	if (g_Pos.y == 0 && g_fTransSpeed == 0) {
		ChangeScene(SCENE_RESULT);
	}


	//座標更新
	g_Pos.y += g_fTestSpeedY;

	g_Pos.z += g_fTransSpeed * cos(g_fAngle / 45);

	g_Pos.x += g_fTransSpeed * sin(g_fAngle / 45);
	g_Pos.x += g_fTestSpeedX;

	//地面に落ちた場合
	if (g_Pos.y < 0)
	{
		g_fTestSpeedY = -g_fTestSpeedY * REBOUND;
		g_fTestSpeedX = +g_fTestSpeedX * 0.89f;

		//摩擦力
		if (g_fTransSpeed > 0)
		{
			g_fTransSpeed -= FRICTION;
		}
		else
		{
			g_fTransSpeed = 0.0f;
		}
	}

	//地面へ突き刺さった場合地面へ戻る
	if (g_Pos.y < g_fieldMin.y)
	{
		g_Pos.y = g_fieldMin.y;
	}

	//行列計算
	D3DXMatrixRotationY(&g_mtxWorldRY, D3DXToRadian(90 + g_fAngle));		//rotation、Y軸回転
	D3DXMatrixRotationZ(&g_mtxWorldRZ, D3DXToRadian(NewRot + Rot));		//rotation、Z軸回転
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorldRY, &g_mtxWorldRZ);		//rotationY+rotationZ=world

	//回転計算
	D3DXMatrixRotationZ(&g_mtxWorldRZ, D3DXToRadian(g_fKaitenZ));//Z回転用関数
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &g_mtxWorldRZ);//合成

	D3DXMatrixScaling(&g_mtxWorldS, 0.3f, 0.3f, 0.3f);					//scale、放大縮小
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &g_mtxWorldS);			//rotation+scale=world

	D3DXMatrixTranslation(&g_mtxWorldT, g_Pos.x, g_Pos.y, g_Pos.z);		//move、平行移動
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &g_mtxWorldT);			//move+world = world

};

void DrawModel(void)
{
	if (g_bPressH == false)
	{
		LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

		DrawUI(TEX_8, SCREEN_WIDTH - 88, 400, 75, 400, 0, 0, 682, 768);
		DrawUI(TEX_9, SCREEN_WIDTH-80, 406, 60, 400, 0, 0, 300, 300);
		DrawUI(TEX_5, SCREEN_WIDTH - 80, 730 - g_nChargeCnt, 60, 380, 0, 0, 864, 768);

		//ワールド行列
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

		//Direct3Dにおける「メッシュ」の「レンダリング」は「マテリアル」を基準にしています。
		//Direct3Dにおける「オブジェクトデータ」の「描画」は「表面的質感情報」を基準にしています。
		for (DWORD i = 0; i < dwNumMaterials; i++) //マテリアルの数だけループする
		{
			pDevice->SetMaterial(&pMeshMaterials[i]);//マテリアルを設定する
			pDevice->SetTexture(0, pMeshTextures[i]);//テクスチャを設定する
			pMesh->DrawSubset(i);//レンダリングする
		}
	}
};

D3DXVECTOR3 GetPos()
{
	return g_Pos;
}

//コインのあたり判定に数値を送る
void ataricoin(int coinsu, float ix, float iy, float iz)
{
	Coinatari[coinsu].ataricoinx = ix;//ixの値をataribonbexに入れる
	Coinatari[coinsu].ataricoiny = iy;//iyの値をataribonbeyに入れる
	Coinatari[coinsu].ataricoinz = iz;//izの値をataribonbezに入れる
}

//コイン10のあたり判定に数値を送る
void ataricoin10(int coinsu, float ix, float iy, float iz)
{
	Coin10atari[coinsu].ataricoin10x = ix;//ixの値をataribonbexに入れる
	Coin10atari[coinsu].ataricoin10y = iy;//iyの値をataribonbeyに入れる
	Coin10atari[coinsu].ataricoin10z = iz;//izの値をataribonbezに入れる
}

//壁のあたり判定に数値を送る
void atarikabe(int kabesu, float ix, float iy, float iz)
{
	Kabeatari[kabesu].atarikabex = ix;//ixの値をataribonbexに入れる
	Kabeatari[kabesu].atarikabey = iy;//iyの値をataribonbeyに入れる
	Kabeatari[kabesu].atarikabez = iz;//izの値をataribonbezに入れる
}

//アイテムのあたり判定に数値を送る
void atariitem(int itemsu, float ix, float iy, float iz)
{
	Itematari[itemsu].atariitemx = ix;//ixの値をataribonbexに入れる
	Itematari[itemsu].atariitemy = iy;//iyの値をataribonbeyに入れる
	Itematari[itemsu].atariitemz = iz;//izの値をataribonbezに入れる
}

//壁のあたり判定に数値を送る
void ataritatemono(int tatemonosu, float ix, float iy, float iz)
{
	Tatemonoatari[tatemonosu].ataritatemonox = ix;//ixの値をataribonbexに入れる
	Tatemonoatari[tatemonosu].ataritatemonoy = iy;//iyの値をataribonbeyに入れる
	Tatemonoatari[tatemonosu].ataritatemonoz = iz;//izの値をataribonbezに入れる
}
