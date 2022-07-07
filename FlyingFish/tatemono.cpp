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
#include "tatemono.h"

//Xモデル表示よう関数の初期化
static LPD3DXBUFFER   g_pTatemonoMaterialBuffer = NULL;
static DWORD          g_nTatemonoMaterialNum = 0;
static LPD3DXMESH     g_pTatemonoMesh = NULL;
static LPD3DXMATERIAL pTatemonoMaterial;
static LPD3DXMESH     pTatemonoTemMesh;


typedef struct {
	D3DXVECTOR3 pos;//座標
	D3DXMATRIX mtxworld;//
	D3DXMATRIX mtxworldS;//拡縮用変数
	D3DXMATRIX mtxworldR;//拡縮用変数
	D3DXMATRIX mtxworldT;//移動用関数
	bool hihixyouzitatemono;//表示非表示
}TATEMONO_DATA;

static TATEMONO_DATA Tatemono[TATEMONO_NUM];//構造体でアイテムをいくつ作るかを宣言

								//構造体を回す（ループ）させるための変数
static int Ui = 0;//構造体をループさせるための変数

void Inittatemono(void)
{

	for (Ui = 0; Ui < TATEMONO_NUM; Ui++)//構造体の数だけ回す 
	{
		Tatemono[Ui].pos.x = -500 + rand() % 1000;
		Tatemono[Ui].pos.y = 0;
		Tatemono[Ui].pos.z = 500 + rand() % 1500;
	}



	//初期化
	LPD3DXBUFFER  pAdjacencyBuffer;

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	//モデリングデータの場所
	HRESULT hr;

	hr = D3DXLoadMeshFromX("resource\\xmodel\\biru2.x",
		D3DXMESH_SYSTEMMEM, pDevice,
		&pAdjacencyBuffer, &g_pTatemonoMaterialBuffer, NULL,
		&g_nTatemonoMaterialNum, &g_pTatemonoMesh);


	if (FAILED(hr))
	{
		MessageBox(NULL, "kaneファイルの読み込みに失敗しました", "エラー", MB_OK);
	}

	//マテリアルの取得
	pTatemonoMaterial = (LPD3DXMATERIAL)g_pTatemonoMaterialBuffer->GetBufferPointer();



	hr = g_pTatemonoMesh->OptimizeInplace(D3DXMESHOPT_COMPACT |
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

	g_pTatemonoMesh->GetDeclaration(Elements);

	hr = g_pTatemonoMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, Elements, pDevice, &pTatemonoTemMesh);

	g_pTatemonoMesh->Release();

	g_pTatemonoMesh = pTatemonoTemMesh;
};
void Uninittatemono(void)
{
	g_pTatemonoMaterialBuffer->Release();
};
void Updatetatemono(void)
{
	for (Ui = 0; Ui < TATEMONO_NUM; Ui++)//構造体の数だけ回す 
	{
		ataritatemono(Ui, Tatemono[Ui].pos.x, Tatemono[Ui].pos.y, Tatemono[Ui].pos.z);//更新したボンベの数と座標をあたり判定に送る

																	  //SRT
		D3DXMatrixIdentity(&Tatemono[Ui].mtxworld);

		D3DXMatrixScaling(&Tatemono[Ui].mtxworldS, 3.0f, 3.0f, 3.0f);     //拡大関数
		D3DXMatrixMultiply(&Tatemono[Ui].mtxworld, &Tatemono[Ui].mtxworld, &Tatemono[Ui].mtxworldS);//合成  

		D3DXMatrixTranslation(&Tatemono[Ui].mtxworldT, Tatemono[Ui].pos.x, Tatemono[Ui].pos.y, Tatemono[Ui].pos.z);//平行移動関数
		D3DXMatrixMultiply(&Tatemono[Ui].mtxworld, &Tatemono[Ui].mtxworld, &Tatemono[Ui].mtxworldT);//合成  
	}
};
void Drawtatemono(void)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	for (Ui = 0; Ui < TATEMONO_NUM; Ui++)//構造体の数だけ回す 
	{



		for (unsigned int i = 0; i < g_nTatemonoMaterialNum; i++)
		{
			pTatemonoMaterial[i].MatD3D.Ambient.r = 0.3f;
			pTatemonoMaterial[i].MatD3D.Ambient.g = 0.3f;
			pTatemonoMaterial[i].MatD3D.Ambient.b = 0.3f;
			pTatemonoMaterial[i].MatD3D.Ambient.a = 1.0f;

			pDevice->SetMaterial(&pTatemonoMaterial[i].MatD3D);

			pDevice->SetTransform(D3DTS_WORLD, &Tatemono[Ui].mtxworld);

			g_pTatemonoMesh->DrawSubset(i);
		}
	}
};
