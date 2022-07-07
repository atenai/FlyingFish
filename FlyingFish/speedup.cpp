#include "speedup.h"
#include "main.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "polygon.h"
#include"camera.h"
#include"input.h"
#include "xmodel.h"


//Xモデル表示よう関数の初期化
static LPD3DXMESH     g_pMesh = NULL;
static LPD3DXMESH     pTemMesh;
static DWORD          g_nMaterialNum = 0;
static LPD3DXBUFFER   g_pMaterialBuffer = NULL;
static LPD3DXMATERIAL pMaterial;
static LPDIRECT3DTEXTURE9 g_pTextures[] = { NULL };


typedef struct {
	D3DXVECTOR3 pos;//座標
	D3DXMATRIX mtxworld;//
	D3DXMATRIX mtxworldS;//拡縮用変数
	D3DXMATRIX mtxworldR;//拡縮用変数
	D3DXMATRIX mtxworldT;//移動用関数
	bool hihixyouzibonbe;//ボンベの表示非表示
}ITEM_DATA;

static ITEM_DATA Item[SPEEDUP_NUM];//構造体でアイテムをいくつ作るかを宣言

//構造体を回す（ループ）させるための変数
static int Ui = 0;//構造体をループさせるための変数

void InitSpeedUp(void)
{
	for (Ui = 0; Ui < SPEEDUP_NUM; Ui++)//構造体の数だけ回す 
	{
		Item[Ui].pos.x = -500 + rand() % 1000;
		Item[Ui].pos.y = 100 + rand() % 500;
		Item[Ui].pos.z = 50 + rand() % 1900;
	}



	//初期化
	LPD3DXBUFFER  pAdjacencyBuffer;

	HRESULT hr;

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();
	//モデリングデータの場所
	hr = D3DXLoadMeshFromX("resource\\xmodel\\taru.x",
		D3DXMESH_SYSTEMMEM, pDevice,
		&pAdjacencyBuffer, &g_pMaterialBuffer, NULL,
		&g_nMaterialNum, &g_pMesh);

	if (FAILED(hr))
	{
		MessageBox(NULL, "Xmodelファイルの読み込みに失敗しました", "エラー", MB_OK);
	}

	//マテリアルの取得
	pMaterial = (LPD3DXMATERIAL)g_pMaterialBuffer->GetBufferPointer();



	hr = g_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjacencyBuffer->GetBufferPointer(),
		NULL, NULL, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "メッシュの最適化に「失敗しました", "エラー", MB_OK);
	}

	

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
void UninitSpeedUp(void)
{
	g_pMaterialBuffer->Release();
};
void UpdateSpeedUp(void)
{
	for (Ui = 0; Ui < SPEEDUP_NUM; Ui++)//構造体の数だけ回す 
	{
		atariitem(Ui, Item[Ui].pos.x, Item[Ui].pos.y, Item[Ui].pos.z);//更新したボンベの数と座標をあたり判定に送る

																	  //SRT
		D3DXMatrixIdentity(&Item[Ui].mtxworld);

		//D3DXMatrixScaling(&Item[Ui].mtxworldS, 1.0f, 1.0f, 1.0f);     //拡大関数
		//D3DXMatrixMultiply(&Item[Ui].mtxworld, &Item[Ui].mtxworld, &Item[Ui].mtxworldS);//合成  

		D3DXMatrixTranslation(&Item[Ui].mtxworldT, Item[Ui].pos.x, Item[Ui].pos.y, Item[Ui].pos.z);//平行移動関数
		D3DXMatrixMultiply(&Item[Ui].mtxworld, &Item[Ui].mtxworld, &Item[Ui].mtxworldT);//合成  
	}
};
void DrawSpeedUp(void)
{

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	for (Ui = 0; Ui < SPEEDUP_NUM; Ui++)//構造体の数だけ回す 
	{

		pDevice->SetTransform(D3DTS_WORLD, &Item[Ui].mtxworld);

		for (unsigned int i = 0; i < g_nMaterialNum; i++)
		{
			pMaterial[i].MatD3D.Ambient.r = 0.3f;
			pMaterial[i].MatD3D.Ambient.g = 0.3f;
			pMaterial[i].MatD3D.Ambient.b = 0.3f;
			pMaterial[i].MatD3D.Ambient.a = 1.0f;

			pDevice->SetMaterial(&pMaterial[i].MatD3D);


			g_pMesh->DrawSubset(i);
		}
	}
};
