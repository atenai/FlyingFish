#include "coin.h"
#include "main.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "polygon.h"
#include "camera.h"
#include "input.h"
#include "judge.h"
#include "xmodel.h"
#include "kabe.h"
#include <time.h>

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
	bool hihixyouzikabe;//ボンベの表示非表示
}KABE_DATA;

static KABE_DATA Kabe[KABE_NUM];//構造体でアイテムをいくつ作るかを宣言

							//構造体を回す（ループ）させるための変数
static int Ui = 0;//構造体をループさせるための変数

void Initkabe(void)
{

	for (Ui = 0; Ui < KABE_NUM; Ui++)//構造体の数だけ回す 
	{
		Kabe[Ui].pos.x = -500 + rand() % 1000;
		Kabe[Ui].pos.y = 50 + rand() % 500;
		Kabe[Ui].pos.z = 50 + rand() % 2800;
	}

	

	//初期化
	LPD3DXBUFFER  pAdjacencyBuffer;

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	//モデリングデータの場所
	HRESULT hr;

	hr = D3DXLoadMeshFromX("resource\\xmodel\\aokane.x",
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
void Uninitkabe(void)
{
	g_pCoinMaterialBuffer->Release();
};
void Updatekabe(void)
{
	for (Ui = 0; Ui < KABE_NUM; Ui++)//構造体の数だけ回す 
	{
		atarikabe(Ui, Kabe[Ui].pos.x, Kabe[Ui].pos.y, Kabe[Ui].pos.z);//更新したボンベの数と座標をあたり判定に送る

																		//SRT
		D3DXMatrixIdentity(&Kabe[Ui].mtxworld);

		//D3DXMatrixScaling(&Kabe[Ui].mtxworldS, 10.0f, 10.0f, 1.0f);     //拡大関数
		//D3DXMatrixMultiply(&Kabe[Ui].mtxworld, &Kabe[Ui].mtxworld, &Kabe[Ui].mtxworldS);//合成  

		D3DXMatrixTranslation(&Kabe[Ui].mtxworldT, Kabe[Ui].pos.x, Kabe[Ui].pos.y, Kabe[Ui].pos.z);//平行移動関数
		D3DXMatrixMultiply(&Kabe[Ui].mtxworld, &Kabe[Ui].mtxworld, &Kabe[Ui].mtxworldT);//合成  
	}
};
void Drawkabe(void)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	for (Ui = 0; Ui < KABE_NUM; Ui++)//構造体の数だけ回す 
	{



		for (unsigned int i = 0; i < g_nCoinMaterialNum; i++)
		{
			pCoinMaterial[i].MatD3D.Ambient.r = 0.3f;
			pCoinMaterial[i].MatD3D.Ambient.g = 0.3f;
			pCoinMaterial[i].MatD3D.Ambient.b = 0.3f;
			pCoinMaterial[i].MatD3D.Ambient.a = 1.0f;

			pDevice->SetMaterial(&pCoinMaterial[i].MatD3D);

			pDevice->SetTransform(D3DTS_WORLD, &Kabe[Ui].mtxworld);

			g_pCoinMesh->DrawSubset(i);
		}
	}
};
