#include "taihou.h"
#include "main.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "polygon.h"
#include"camera.h"
#include"input.h"

//Xモデル表示用変数
static LPD3DXMESH pMesh = NULL;
static DWORD dwNumMaterials = 0;
static D3DMATERIAL9* pMeshMaterials = NULL;
static LPDIRECT3DTEXTURE9* pMeshTextures = NULL;


static D3DXMATRIX g_mtxWorld;
static D3DXMATRIX g_mtxWorldS;
static D3DXMATRIX g_mtxWorldT;
static D3DXMATRIX g_mtxWorldR;

static D3DXVECTOR3 g_Pos(0.0f, 0.0f, 0.0f);   //xmod pos

static float g_fAngle = 0.0f;	//発射時点の左右角度


//テクスチャ

void InitTaihou(void)
{

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	//Xファイルからメッシュをロードする
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX("./resource/xmodel/taihou.x", D3DXMESH_SYSTEMMEM,
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

void UninitTaihou(void)
{
	g_fAngle = 0.0f;	//発射時点の左右角度
	if (pMesh != NULL)
	{
		pMesh->Release();//これで開放される
		pMesh = NULL;//アドレスが残っているのでNULLで上書き
	}
};

void UpdateTaihou(void)
{

	//発射時点の左右操作
	if (GetKeyboardPress(DIK_Q) && g_Pos.y == 0.0f)
	{
		g_fAngle -= 0.4f;

		if (g_fAngle <= -10)
		{
			g_fAngle = -10;
		}
	}

	if (GetKeyboardPress(DIK_E) && g_Pos.y == 0.0f)
	{
		g_fAngle += 0.4f;

		if (g_fAngle >= 10)
		{
			g_fAngle = 10;
		}
	}

	D3DXMatrixRotationY(&g_mtxWorldR, D3DXToRadian(g_fAngle));		//rotation、Y軸回転

	D3DXMatrixScaling(&g_mtxWorldS, 0.35f, 0.35f, 0.35f);     //scale

	D3DXMatrixTranslation(&g_mtxWorldT, 0, 0, 0);   //move

	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorldS, &g_mtxWorldT);       //move+world = world
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &g_mtxWorldR);

};
void DrawTaihou(void)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

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
};
