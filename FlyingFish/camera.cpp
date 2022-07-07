#include "Camera.h"
#include "main.h"
#include "Xmodel.h"

//***************************************************************************************
//	グローバル変数宣言
//***************************************************************************************

static D3DXMATRIX g_MtxView;		//ビュー行列
static D3DXMATRIX g_MtxProjection;	//プロジェクション行列

static D3DXVECTOR3 g_Up(0, 1.f, 0);//上ベクトル
static D3DXVECTOR3 g_Eye(0.0f, 3.0f, -3.0f);//カメラ座標
static D3DXVECTOR3 g_At(0.0f, 0.0f, 0.0f);//見てる場所(注視点)

static D3DXVECTOR3 g_Eye1(0.0f, 1.0f, -6.0f);//カメラ座標



void UpdateCamera(void)
{

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();
	g_At = GetPos();
	g_Eye = GetPos() + g_Eye1;


	D3DXMatrixLookAtLH(&g_MtxView, &g_Eye, &g_At, &g_Up);
	//プロジェクション行列の作成
	D3DXMatrixPerspectiveFovLH(&g_MtxProjection, D3DX_PI / 3 , (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 3000.0f);

	//各種類行列の設定
	pDevice->SetTransform(D3DTS_VIEW, &g_MtxView);
	pDevice->SetTransform(D3DTS_PROJECTION, &g_MtxProjection);
}
