#include "polygon3DTest.h"
#include "main.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "polygon.h"
#include "camera.h"
#include"lighting.h"

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;  //法線		
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;	//テクスチャ
}VERTEX3D;

#define FVF_VERTEX_3D ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)
//上から順にやる作業を設定している

static int g_nTexNum;
static LPDIRECT3DTEXTURE9 			g_pTextures[TEXTURE_MAX] = { NULL };


static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;


static D3DXMATRIX g_MtxOut[5];		//合成行列
static D3DXMATRIX g_MtxCube[5];	//cube行列
static D3DXMATRIX g_MtxTCube[5];	//cube平行移動行列
static D3DXMATRIX g_MtxSCube[5];	//cube拡大行列
static D3DXMATRIX g_MtxRCube[5];	//cube回転行列



void InitPolygon3D(void)
{
	HRESULT hr;

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();


	pDevice->CreateVertexBuffer(sizeof(VERTEX3D) * 24, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);

	pDevice->CreateIndexBuffer(sizeof(WORD) * 36, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);


	float u0[6];
	float v0[6];
	float u1[6];
	float v1[6];

	int div_x = 4;
	float tw = 1024.f;
	float th = 1024.f;
	float tcw = 256.f;
	float tch = 256.f;

	for (int i = 0; i < 6; i++)
	{
		int pattern = i;

		int patternH = pattern % div_x;
		int patternV = pattern / div_x;

		float tcx = patternH * tcw;
		float tcy = patternV * tch;

		u0[i] = (float)tcx / tw;
		v0[i] = (float)tcy / th;
		u1[i] = (float)(tcx + tcw) / tw;
		v1[i] = (float)(tcy + tch) / th;
	}


	VERTEX3D Pv[] = {
		//前面
		{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u0[0], v0[0]) },
		{ D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u1[0], v0[0]) },
		{ D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u0[0], v1[0]) },
		{ D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u1[0], v1[0]) },

		//右横面
		{ D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u0[1], v0[1]) },
		{ D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u1[1], v0[1]) },
		{ D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u0[1], v1[1]) },
		{ D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u1[1], v1[1]) },

		//後ろ面
		{ D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u0[2], v0[2]) },
		{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u1[2], v0[2]) },
		{ D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u0[2], v1[2]) },
		{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u1[2], v1[2]) },

		//左横面
		{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u0[3], v0[3]) },
		{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u1[3], v0[3]) },
		{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u0[3], v1[3]) },
		{ D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u1[3], v1[3]) },

		//上
		{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u0[4], v0[4]) },
		{ D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  D3DXVECTOR2(u1[4], v0[4]) },
		{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  D3DXVECTOR2(u0[4], v1[4]) },
		{ D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  D3DXVECTOR2(u1[4], v1[4]) },


		//下面
		{ D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  D3DXVECTOR2(u0[5], v0[5]) },
		{ D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u1[5], v0[5]) },
		{ D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u0[5], v1[5]) },
		{ D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(u1[5], v1[5]) },
	};


	WORD index[] = {
		0,1,2,    //前面
		1,3,2,
		4,5,6,	//右面
		5,7,6,
		8,9,10,	//左面
		9,11,10,
		12,13,14,
		13,15,14,
		16,17,18,
		17,19,18,
		20,21,22,
		21,23,22
	};



	VERTEX3D* pV;

	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	for (int i = 0; i < 24; i++)
	{
		pV[i].pos = Pv[i].pos;
		pV[i].normal = Pv[i].normal;
		pV[i].color = Pv[i].color;
		pV[i].texcoord = Pv[i].texcoord;
	}


	g_pVertexBuffer->Unlock();




	LPWORD pIndex;

	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);

	for (int i = 0; i < 36; i++)
	{
		pIndex[i] = index[i];
	}


	g_pIndexBuffer->Unlock();





	/****** テクスチャを読み込む ******/
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		hr = D3DXCreateTextureFromFile(pDevice, g_aTextures[i].fileName, &g_pTextures[i]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "テクスチャが読み込めませんでした", "注意", MB_OK);
			return;
		}
	}
}

void UninitPolygon3D(void)
{}

void UpdatePolygon3D(void)
{
	D3DXMatrixScaling(&g_MtxSCube[1], CUBE_HABA, CUBE_HABA, CUBE_HABA);

	D3DXMatrixTranslation(&g_MtxTCube[1], CUBE_1_X, CUBE_1_Y, CUBE_1_Z);

	D3DXMatrixMultiply(&g_MtxCube[1], &g_MtxSCube[1], &g_MtxTCube[1]);



	D3DXMatrixScaling(&g_MtxSCube[2], CUBE_HABA, CUBE_HABA, CUBE_HABA);

	D3DXMatrixTranslation(&g_MtxTCube[2], CUBE_2_X, CUBE_2_Y, CUBE_2_Z);

	D3DXMatrixMultiply(&g_MtxCube[2], &g_MtxSCube[2], &g_MtxTCube[2]);

	D3DXMatrixScaling(&g_MtxSCube[3], CUBE_HABA, CUBE_HABA, CUBE_HABA);

	D3DXMatrixTranslation(&g_MtxTCube[3], CUBE_3_X, CUBE_3_Y, CUBE_3_Z);

	D3DXMatrixMultiply(&g_MtxCube[3], &g_MtxSCube[3], &g_MtxTCube[3]);


	D3DXMatrixScaling(&g_MtxSCube[4], CUBE_HABA, CUBE_HABA, CUBE_HABA);

	D3DXMatrixTranslation(&g_MtxTCube[4], CUBE_4_X, CUBE_4_Y, CUBE_4_Z);

	D3DXMatrixMultiply(&g_MtxCube[4], &g_MtxSCube[4], &g_MtxTCube[4]);

	/*D3DXMatrixScaling(&g_MtxSCube[5], CUBE_HABA, CUBE_HABA, CUBE_HABA);

	D3DXMatrixTranslation(&g_MtxTCube[5], CUBE_5_X, CUBE_5_Y, CUBE_5_Z);

	D3DXMatrixMultiply(&g_MtxCube[5], &g_MtxSCube[5], &g_MtxTCube[5]);*/

}

void DrawPolygon3D()
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	DrawLight();

	//マテリアルの設定
	D3DMATERIAL9 mat;

	ZeroMemory(&mat, sizeof(mat));

	mat.Diffuse.r = 1.0f;
	mat.Diffuse.g = 1.0f;
	mat.Diffuse.b = 1.0f;
	mat.Diffuse.a = 1.0f;

	mat.Ambient.r = 1.0f;
	mat.Ambient.g = 1.0f;
	mat.Ambient.b = 1.0f;
	mat.Ambient.a = 1.0f;

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
	pDevice->SetIndices(g_pIndexBuffer);

	pDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));


	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetMaterial(&mat);

	pDevice->LightEnable(0, TRUE);

	for (int i = 0; i < CUBE_NUM; i++)
	{
		//テクスチャ
		pDevice->SetTexture(0, g_pTextures[g_nTexNum]);

		//行列変換

		pDevice->SetTransform(D3DTS_WORLD, &g_MtxCube[i]);
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
	}



}

void Set3DTexture(int texNum)
{
	g_nTexNum = texNum;
}
