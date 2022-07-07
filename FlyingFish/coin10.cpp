#include "coin10.h"
#include "main.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "polygon.h"
#include "judge.h"
#include "xmodel.h"

//Xモデル表示よう関数の初期化
static LPD3DXBUFFER   g_pCoinMaterialBuffer = NULL;
static DWORD          g_nCoinMaterialNum = 0;
static LPD3DXMESH     g_pCoinMesh = NULL;
static LPD3DXMATERIAL pCoinMaterial;
static LPD3DXMESH     pCoinTemMesh;


typedef struct {
	D3DXVECTOR3 pos;//座標
	D3DXMATRIX mtxworld;//
	D3DXMATRIX mtxworldS;//拡縮用変数
	D3DXMATRIX mtxworldR;//拡縮用変数
	D3DXMATRIX mtxworldT;//移動用関数
	bool hihixyouzicoin10;//ボンベの表示非表示
}COIN10_DATA;

static COIN10_DATA Coin[COIN10_NUM];//構造体でアイテムをいくつ作るかを宣言

						 //構造体を回す（ループ）させるための変数
static int Ui = 0;//構造体をループさせるための変数

void Initcoin10(void)
{

	for (Ui = 0; Ui < COIN10_NUM; Ui++)//構造体の数だけ回す 
	{
		Coin[Ui].pos.x = -500 + rand() % 1000;//-500～500
		Coin[Ui].pos.y = 50 + rand() % 500;//50～550
		Coin[Ui].pos.z = 50 + rand() % 2800;
	}
	
	//初期化
	LPD3DXBUFFER  pAdjacencyBuffer;

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	//モデリングデータの場所
	HRESULT hr;

	hr = D3DXLoadMeshFromX("resource\\xmodel\\akakane.x",
		D3DXMESH_SYSTEMMEM, pDevice,
		&pAdjacencyBuffer, &g_pCoinMaterialBuffer, NULL,
		&g_nCoinMaterialNum, &g_pCoinMesh);


	if (FAILED(hr))
	{
		MessageBox(NULL, "kaneファイルの読み込みに失敗しました", "エラー", MB_OK);
	}

	//マテリアルの取得
	pCoinMaterial = (LPD3DXMATERIAL)g_pCoinMaterialBuffer->GetBufferPointer();



	hr = g_pCoinMesh->OptimizeInplace(D3DXMESHOPT_COMPACT |
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

	g_pCoinMesh->GetDeclaration(Elements);

	hr = g_pCoinMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, Elements, pDevice, &pCoinTemMesh);

	g_pCoinMesh->Release();

	g_pCoinMesh = pCoinTemMesh;
};

void Uninitcoin10(void)
{
	g_pCoinMaterialBuffer->Release();
};

void Updatecoin10(void)
{
	for (Ui = 0; Ui < COIN10_NUM; Ui++)//構造体の数だけ回す 
	{
		ataricoin10(Ui, Coin[Ui].pos.x, Coin[Ui].pos.y, Coin[Ui].pos.z);//更新したボンベの数と座標をあたり判定に送る

																	  //SRT
		D3DXMatrixIdentity(&Coin[Ui].mtxworld);

		//D3DXMatrixScaling(&Coin[Ui].mtxworldS, 1.0f, 1.0f, 1.0f);     //拡大関数
		//D3DXMatrixMultiply(&Coin[Ui].mtxworld, &Coin[Ui].mtxworld, &Coin[Ui].mtxworldS);//合成  

		D3DXMatrixTranslation(&Coin[Ui].mtxworldT, Coin[Ui].pos.x, Coin[Ui].pos.y, Coin[Ui].pos.z);//平行移動関数
		D3DXMatrixMultiply(&Coin[Ui].mtxworld, &Coin[Ui].mtxworld, &Coin[Ui].mtxworldT);//合成  
	}
};

void Drawcoin10(void)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	for (Ui = 0; Ui < COIN10_NUM; Ui++)//構造体の数だけ回す 
	{



		for (unsigned int i = 0; i < g_nCoinMaterialNum; i++)
		{
			pCoinMaterial[i].MatD3D.Ambient.r = 0.3f;
			pCoinMaterial[i].MatD3D.Ambient.g = 0.3f;
			pCoinMaterial[i].MatD3D.Ambient.b = 0.3f;
			pCoinMaterial[i].MatD3D.Ambient.a = 1.0f;

			pDevice->SetMaterial(&pCoinMaterial[i].MatD3D);

			pDevice->SetTransform(D3DTS_WORLD, &Coin[Ui].mtxworld);

			g_pCoinMesh->DrawSubset(i);
		}
	}
};
