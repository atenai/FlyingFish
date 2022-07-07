#include "target.h"
#include "main.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "polygon.h"
#include"camera.h"
#include"input.h"



static LPD3DXMESH     g_pMesh = NULL;
static LPD3DXMESH     pTemMesh;
static DWORD          g_nMaterialNum = 0;
static LPD3DXBUFFER   g_pMaterialBuffer = NULL;

static D3DXMATRIX g_mtxWorld;
static D3DXMATRIX g_mtxWorldS;
static D3DXMATRIX g_mtxWorldT;
static D3DXMATRIX g_mtxWorldR;

static LPD3DXMATERIAL pMaterial;

static LPDIRECT3DTEXTURE9 g_pTextures[] = { NULL };


//テクスチャ

void InitTarget(void)
{
	//初期化
	LPD3DXBUFFER  pAdjacencyBuffer;

	HRESULT hr;

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	hr = D3DXLoadMeshFromX("resource\\xmodel\\hal.x",
		D3DXMESH_SYSTEMMEM, pDevice,
		&pAdjacencyBuffer, &g_pMaterialBuffer, NULL,
		&g_nMaterialNum, &g_pMesh);


	if (FAILED(hr))
	{
		MessageBox(NULL, "Xmodelファイルの読み込みに失敗しました", "エラー", MB_OK);
	}

	//マテリアルの取得
	pMaterial = (LPD3DXMATERIAL)g_pMaterialBuffer->GetBufferPointer();

	for (int nCnt = 0; nCnt < g_nMaterialNum; nCnt++)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			pMaterial[nCnt].pTextureFilename,
			&g_pTextures[nCnt]);
	}

	hr = g_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjacencyBuffer->GetBufferPointer(),
		NULL, NULL, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "メッシュの最適化に「失敗しました", "エラー", MB_OK);
	}

	//for (int i = 0; i < TEXTURE_MAX; i++)
	//{
	//	hr = D3DXCreateTextureFromFile(pDevice, g_aTextures[i].fileName, &g_pTextures[i]);
	//	if (FAILED(hr))
	//	{
	//		MessageBox(NULL, "テクスチャが読み込めませんでした", "注意", MB_OK);
	//	}
	//}

	if (pAdjacencyBuffer != NULL)
	{
		pAdjacencyBuffer->Release();
	}

	D3DVERTEXELEMENT9 Elements[32];

	g_pMesh->GetDeclaration(Elements);

	hr = g_pMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, Elements, pDevice, &pTemMesh);

	g_pMesh->Release();

	g_pMesh = pTemMesh;
};
void UninitTarget(void)
{
	g_pMaterialBuffer->Release();
};
void UpdateTarget(void)
{

	//--------------
	//  角度の計算
	//--------------

	//D3DXMatrixRotationY(&g_mtxWorld, D3DXToRadian(180));     //rotation
	//D3DXMatrixRotationZ(&g_mtxWorld, D3DXToRadian(NewRot + Rot));     //rotation
	//D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &g_mtxWorld);   //rotationY+rotationZ=world


	D3DXMatrixScaling(&g_mtxWorldS, 1.0f, 1.0f, 1.0f);     //scale
	//D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &g_mtxWorldS);   //rotation+scale=world

	D3DXMatrixTranslation(&g_mtxWorldT, POS_X, POS_Y, POS_Z);   //move

	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorldS, &g_mtxWorldT);       //move+world = world

};
void DrawTarget(void)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	for (unsigned int i = 0; i < g_nMaterialNum; i++)
	{
		pMaterial[i].MatD3D.Ambient.r = 0.3f;
		pMaterial[i].MatD3D.Ambient.g = 0.3f;
		pMaterial[i].MatD3D.Ambient.b = 0.3f;
		pMaterial[i].MatD3D.Ambient.a = 1.0f;

		pDevice->SetMaterial(&pMaterial[i].MatD3D);

		pDevice->SetTexture(0, g_pTextures[i]);

		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

		g_pMesh->DrawSubset(i);
	}
};
