#include "main.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "polygon.h"
#include "camera.h"
#include "lighting.h"
#include "Shadow.h"
#include"Xmodel.h"
#include"input.h"

#define FVF_VERTEX_3D ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)

static D3DXMATRIX g_mtxWorld;
static D3DXMATRIX g_mtxWorldR;
static D3DXMATRIX g_mtxWorldT;
static D3DXMATRIX g_mtxWorldS;

static int g_nTexNum;
static LPDIRECT3DTEXTURE9 			g_pTextures[TEXTURE_MAX] = { NULL };

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

static D3DXVECTOR3 g_SPos(0.0f, 0.0f, 0.0f);   //Shadow pos
static D3DXVECTOR3 g_Scal(0.7f, 1.0f, 0.7f);

static float g_TransSpeed = 0.02f;
static D3DXVECTOR3 modpos;


typedef struct
{
	D3DXMATRIX MtxShadow;
	D3DXMATRIX MtxShadowT;
	D3DXMATRIX MtxShadowS;
	D3DXMATRIX MtxShadowR;
	D3DXVECTOR3 pos;
}SHADOWS;


void InitShadow(void)
{


	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	HRESULT hr;

	pDevice->CreateVertexBuffer(sizeof(VERTEX3D) * 24, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);

	pDevice->CreateIndexBuffer(sizeof(WORD) * 36, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);


	VERTEX3D Pv[] =
	{
		//上
		{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  D3DXVECTOR2(1.0f, 1.0f) }
	};

	WORD index[] =
	{
		0,1,2,3   //前面
	};



	VERTEX3D* pV;


	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	for (int i = 0; i < 4; i++)
	{
		pV[i].pos = Pv[i].pos;
		pV[i].normal = Pv[i].normal;
		pV[i].color = Pv[i].color;
		pV[i].texcoord = Pv[i].texcoord;
	}
	g_pVertexBuffer->Unlock();


	LPWORD pIndex;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);

	for (int i = 0; i < 4; i++)
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
void UninitShadow(void)
{

	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
	if (g_pIndexBuffer)
	{
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_pTextures[i])
		{
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
	}
}
void UpdateShadow(void)
{

	modpos = GetPos();
	if (modpos.y > 0)
	{
		g_Scal.x -= g_TransSpeed;
		g_Scal.z -= g_TransSpeed;
	}

	if (modpos.y <0.0f)
	{
		g_Scal.x += g_TransSpeed;
		g_Scal.z += g_TransSpeed;
	}


	if (g_Scal.x < 0.3f)
	{
		g_Scal.x = 0.3f;
	}
	if (g_Scal.z < 0.3f)
	{
		g_Scal.z = 0.3f;
	}
	if (g_Scal.x > 0.7f)
	{
		g_Scal.x = 0.7f;
	}
	if (g_Scal.z > 0.7f)
	{
		g_Scal.z = 0.7f;
	}





}

void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	if (pDevice == NULL)
	{
		return;
	}

	DrawLight();
	pDevice->SetTexture(0, g_pTextures[TEXTURE_SHADOW]);

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));

	pDevice->SetIndices(g_pIndexBuffer);

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);


	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);


	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);

	for (int i = 0; i < 1; i++)
	{
		g_SPos = GetPos();

		D3DXMatrixIdentity(&g_mtxWorld);
		D3DXMatrixScaling(&g_mtxWorldS, g_Scal.x, g_Scal.y, g_Scal.z);
		D3DXMatrixTranslation(&g_mtxWorldT, g_SPos.x, -0.5f, g_SPos.z);
		D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorldS, &g_mtxWorldT);

		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);

	}

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


}
