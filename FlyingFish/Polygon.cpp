//━━━━━━━━━━━━━━━━━━━━━━━━
//ポリゴン描画
//━━━━━━━━━━━━━━━━━━━━━━━━
#include "main.h"
#include "polygon.h"
#include "math.h"

//━━━━━━━━━━━━━━━━━━━━━━━━
//マクロ定義
//━━━━━━━━━━━━━━━━━━━━━━━━
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//RHWは座標変換済み
//━━━━━━━━━━━━━━━━━━━━━━━━
//構造体宣言
//━━━━━━━━━━━━━━━━━━━━━━━━

typedef struct
{
	D3DXVECTOR4 pos;			//図形の座標
	D3DCOLOR color;				//図形の色
	D3DXVECTOR2 texcoord;	//テクスチャ
}VERTEX_2D;

//━━━━━━━━━━━━━━━━━━━━━━━━
//グローバル変数
//━━━━━━━━━━━━━━━━━━━━━━━━
static float g_fScale = 1.0f;
static float g_fScaleAdd = 0.02f;

static float g_fAngle = 0.0f;
static float g_fRotationOffsetX = 0.0f;
static float g_fRotationOffsetY = 0.0f;

static int g_nTexNum;

static D3DCOLOR g_Color = 0xffffffff;

static float g_fDouble = 0.0f;

static LPDIRECT3DVERTEXBUFFER9 	g_pVertexBuffer = NULL;
static LPDIRECT3DTEXTURE9 				g_pTextures[TEXTURE_MAX] = { NULL };

static void CreateVertex(float dx, float dy,
	int tcx, int tcy,
	int nTurn);//頂点座標ｘ,頂点座標ｙ,幅,高さ,切り取る座標ｘ,切り取る座標ｙ,反転の向き0(正)1(逆)

static void CreateVertexAffine(float dx, float dy,
	int tcx, int tcy,
	int nTurn);//頂点座標ｘ,頂点座標ｙ,幅,高さ,切り取る座標ｘ,切り取る座標ｙ,反転の向き0(正)1(逆)

			   //━━━━━━━━━━━━━━━━━━━━━━━━
			   //関数
			   //━━━━━━━━━━━━━━━━━━━━━━━━
bool InitPolygon(void)
{
	HRESULT hr;

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	//変数初期化
	g_nTexNum = TEXTURE_UNKNOW;
	g_Color = 0xffffffff;
	g_fScale = 1.0f;
	g_fScaleAdd = 0.02f;

	/****** テクスチャを読み込む ******/
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		hr = D3DXCreateTextureFromFile(pDevice, g_aTextures[i].fileName, &g_pTextures[i]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "テクスチャが読み込めませんでした", "注意", MB_OK);
			return false;
		}
	}

	/****** VRAMを借りる ******/
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,				//4つ分
		D3DUSAGE_WRITEONLY,			//書き込みしかできない
		FVF_VERTEX_2D,						//０でも問題ない
		D3DPOOL_MANAGED,					//デバイスに管理を任せる
		&g_pVertexBuffer,						//管理者の居場所を書きこむため
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "テクスチャが読み込めませんでした", "注意", MB_OK);
	}

	return true;
}

void UninitPolygon(void)
{
	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_pTextures[i] != NULL)
		{
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
	}
}

void UpdatePolygon(void)
{
	g_fScale += g_fScaleAdd;
	if (g_fScale > 1.0f || g_fScale < 0.125f)
	{
		g_fScaleAdd = -g_fScaleAdd;
	}
}

void SetTexture(int texNum)
{
	g_nTexNum = texNum;
}

void SetScale(float fDouble)
{
	g_fDouble = fDouble;
}

void SetPolygonColor(D3DCOLOR color)
{
	g_Color = color;
}

void PrimitiveDrawLine(float x0, float y0, float x1, float y1)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	if (pDevice != NULL)
	{
		// FVF(今から使用する頂点情報)の設定
		VERTEX_2D* pV;
		/*g_pVertexBuffer->Lock(ここから, ここまで(0なら全部), 疑似アドレス, );*/
		g_pVertexBuffer->Lock(0, sizeof(VERTEX_2D) * 2, (void**)&pV, D3DLOCK_DISCARD);

		pV[0].pos = D3DXVECTOR4(x0 + -0.5f, y0 + -0.5f, 1.0f, 1.0f);
		pV[1].pos = D3DXVECTOR4(x1 + -0.5f, y1 + -0.5f, 1.0f, 1.0f);

		pV[0].color =
			pV[1].color = g_Color;

		pV[0].texcoord = D3DXVECTOR2(0.0f, 0.0f);
		pV[1].texcoord = D3DXVECTOR2(0.0f, 0.0f);

		g_pVertexBuffer->Unlock();

		pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX_2D));

		// FVF(今から使用する頂点情報)の設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		pDevice->SetTexture(0, NULL);

		pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, 1);
	}
}

void SetPolygonRotation(float cx, float cy, float angle)
{
	g_fAngle = angle;
	g_fRotationOffsetX = cx;
	g_fRotationOffsetY = cy;
}

void DrawPolygon(float dx, float dy, int tcx, int tcy, int nTurn, bool bAffine)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	if (pDevice == NULL)
	{
		return;
	}

	if (bAffine)
	{
		CreateVertexAffine(dx, dy, tcx, tcy, nTurn);
	}
	else
	{
		CreateVertex(dx, dy, tcx, tcy, nTurn);
	}

	//パイプライン（関数を使うよ）
	//GPUとバーテックスをパイプでつなぐ
	pDevice->SetStreamSource(0,									//0番目
		g_pVertexBuffer,			//つないでほしい居場所
		0,									//ここから流す
		sizeof(VERTEX_2D));		//隣の頂点までどのくらいの長さ

								//FVF（今から使用する頂点情報）の設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャセット
	pDevice->SetTexture(0, g_pTextures[g_nTexNum]);

	//ポリゴンを描く（バーテックスバッファにあるやつを描画する）
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,				//プリミティブタイプ（4角形）
		0,														//何番目から描画するか
		2);													//○角形を何個表示するか
}

//━━━━━━━━━━━━━━━━━━━━━━━━
//ロックのやつ
//━━━━━━━━━━━━━━━━━━━━━━━━
static void CreateVertex(float dx, float dy, int tcx, int tcy, int nTurn)
{
	HRESULT hr;

	/****** 変数宣言 ******/
	float u0;
	float v0;
	float u1;
	float v1;

	int dw = g_aTextures[g_nTexNum].texWidth * g_fDouble;
	int dh = g_aTextures[g_nTexNum].texHeight * g_fDouble;

	int tw = g_aTextures[g_nTexNum].Width;
	int th = g_aTextures[g_nTexNum].Height;

	//テクスチャの反転の分岐
	if (nTurn == TEXTURE_REAL)//そのまま
	{
		u0 = (float)tcx / tw;
		v0 = (float)tcy / th;
		u1 = (float)(tcx + g_aTextures[g_nTexNum].texWidth) / tw;
		v1 = (float)(tcy + g_aTextures[g_nTexNum].texHeight) / th;
	}
	else//反転
	{
		u0 = (float)(tcx + g_aTextures[g_nTexNum].texWidth) / tw;
		v0 = (float)tcy / th;
		u1 = (float)tcx / tw;
		v1 = (float)(tcy + g_aTextures[g_nTexNum].texHeight) / th;
	}

	/*ロックをかけると疑似アドレスをもらえる*/
	VERTEX_2D * pV;			//ここに入れたい

	hr = g_pVertexBuffer->Lock(0,				//こっから
		0,				//どこまで（０は全部）(正しくはsizeof(VERTEX_2D)*4)
		(void**)&pV,//(voidのポインタのポインタ型じゃないといけない）
		D3DLOCK_DISCARD);

	if (FAILED(hr))
	{
		MessageBox(NULL, "ロックできませんでした", "注意", MB_OK);
	}

	pV[0].pos = D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 1.0f, 1.0f);
	pV[1].pos = D3DXVECTOR4(dx + dw - 0.5f, dy - 0.5f, 1.0f, 1.0f);
	pV[2].pos = D3DXVECTOR4(dx - 0.5f, dy + dh - 0.5f, 1.0f, 1.0f);
	pV[3].pos = D3DXVECTOR4(dx + dw - 0.5f, dy + dh - 0.5f, 1.0f, 1.0f);

	pV[0].color =
		pV[1].color =
		pV[2].color =
		pV[3].color = g_Color;

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u0, v1);
	pV[3].texcoord = D3DXVECTOR2(u1, v1);

	g_pVertexBuffer->Unlock();
}

//━━━━━━━━━━━━━━━━━━━━━━━━
//ロックのやつ(回転バージョン）
//━━━━━━━━━━━━━━━━━━━━━━━━
void CreateVertexAffine(float dx, float dy, int tcx, int tcy, int nTurn)
{
	HRESULT hr;

	/****** 変数宣言 ******/
	float u0;
	float v0;
	float u1;
	float v1;

	int tw = g_aTextures[g_nTexNum].Width;
	int th = g_aTextures[g_nTexNum].Height;

	//テクスチャの反転の分岐
	if (nTurn == TEXTURE_REAL)//そのまま
	{
		u0 = (float)tcx / tw;
		v0 = (float)tcy / th;
		u1 = (float)(tcx + g_aTextures[g_nTexNum].texWidth) / tw;
		v1 = (float)(tcy + g_aTextures[g_nTexNum].texHeight) / th;
	}
	else//反転
	{
		u0 = (float)(tcx + g_aTextures[g_nTexNum].texWidth) / tw;
		v0 = (float)tcy / th;
		u1 = (float)tcx / tw;
		v1 = (float)(tcy + g_aTextures[g_nTexNum].texHeight) / th;
	}

	/*ロックをかけると疑似アドレスをもらえる*/
	VERTEX_2D * pV;			//ここに入れたい

	hr = g_pVertexBuffer->Lock(0,				//こっから
		0,				//どこまで（０は全部）(正しくはsizeof(VERTEX_2D)*4)
		(void**)&pV,//(voidのポインタのポインタ型じゃないといけない）
		D3DLOCK_DISCARD);

	//一時変数
	int dw = g_aTextures[g_nTexNum].texWidth * g_fDouble;
	int dh = g_aTextures[g_nTexNum].texHeight * g_fDouble;

	float x = -g_fRotationOffsetX;
	float x_w = dw - g_fRotationOffsetX;
	float y = -g_fRotationOffsetY;
	float y_h = dh - g_fRotationOffsetY;

	dx += g_fRotationOffsetX;
	dy += g_fRotationOffsetY;

	if (FAILED(hr))
	{
		MessageBox(NULL, "ロックできませんでした", "注意", MB_OK);
	}

	pV[0].pos = D3DXVECTOR4(dx + x		 * cosf(g_fAngle) - y		 * sinf(g_fAngle) - 0.5f, dy + x		* sinf(g_fAngle) + y		* cos(g_fAngle) - 0.5f, 1.0f, 1.0f);
	pV[1].pos = D3DXVECTOR4(dx + x_w	 * cosf(g_fAngle) - y		 * sinf(g_fAngle) - 0.5f, dy + x_w	* sinf(g_fAngle) + y		* cos(g_fAngle) - 0.5f, 1.0f, 1.0f);
	pV[2].pos = D3DXVECTOR4(dx + x		 * cosf(g_fAngle) - y_h	 * sinf(g_fAngle) - 0.5f, dy + x		* sinf(g_fAngle) + y_h	* cos(g_fAngle) - 0.5f, 1.0f, 1.0f);
	pV[3].pos = D3DXVECTOR4(dx + x_w	 * cosf(g_fAngle) - y_h	 * sinf(g_fAngle) - 0.5f, dy + x_w	* sinf(g_fAngle) + y_h	* cos(g_fAngle) - 0.5f, 1.0f, 1.0f);

	pV[0].color =
		pV[1].color =
		pV[2].color =
		pV[3].color = g_Color;

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u0, v1);
	pV[3].texcoord = D3DXVECTOR2(u1, v1);

	g_pVertexBuffer->Unlock();
}