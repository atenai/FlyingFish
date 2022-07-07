#include "meshfield.h"
#include "main.h"
#include <d3dx9.h>
#include <d3d9.h>

#define FVF_VERTEX_3D ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;  //法線
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;	//テクスチャ
}VERTEX3D;

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

static int g_nTexNum;
static LPDIRECT3DTEXTURE9 	g_pTextures[TEXTURE_MAX] = { NULL };

static int FieldNumX = 10.0f;
static int FieldNumZ = 10.0f;

static float FieldSizeX = 10.0f;
static float FieldSizeZ = 10.0f;

static VERTEX3D *g_v;
static WORD *g_index;

bool InitMeshField(int x, int z, float Sx, float Sz)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	int vSize = (FieldNumX + 1)*(FieldNumZ + 1);


	FieldNumX = x;
	FieldNumZ = z;

	FieldSizeX = Sx;
	FieldSizeZ = Sz;


	g_v = new VERTEX3D[vSize];
	int vindex = 0;
	if (FieldNumZ >= 2)
	{
		vindex = (FieldNumX * 2 + 3)*FieldNumZ + FieldNumZ - 2;
	}
	else
	{
		vindex = (FieldNumX + 1)*(FieldNumZ + 1);
	}
	g_index = new WORD[vindex];
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		HRESULT hr;

		hr = D3DXCreateTextureFromFile(pDevice, g_aTextures[i].fileName, &g_pTextures[i]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "テクスチャが読み込めませんでした", "注意", MB_OK);
			return false;
		}

		hr = pDevice->CreateVertexBuffer
		(
			sizeof(VERTEX3D) * vSize,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_pVertexBuffer,
			NULL
		);
		if (FAILED(hr))
		{
			MessageBox(NULL, "頂点バッファが作れなかった", "エラー", MB_OK);
			return false;
		}

		hr = pDevice->CreateIndexBuffer
		(
			sizeof(WORD) * vindex,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&g_pIndexBuffer,
			NULL
		);
		if (FAILED(hr))
		{
			MessageBox(NULL, "インデックスバッファが作れなかった", "エラー", MB_OK);
			return false;
		}
	}
	return true;
}

void UninitMeshField(void)
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
		if (g_pTextures[i] != NULL)
		{
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
	}
	delete g_v;
	delete g_index;
}

void UpdateMeshField(void)
{
	D3DXMatrixIdentity(&g_mtxfield);
	//D3DXMatrixRotationX(&g_mtxfield, -1.0f);

}

void DrawMeshField(void)
{
	VERTEX3D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
	LPWORD pIndex;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	if (pDevice == NULL)
	{
		return;
	}

	int fieldsizex = FieldSizeX / FieldNumX;
	int fieldsizez = FieldSizeZ / FieldNumZ;

	for (int z = 0; z <= FieldNumZ; z++)
	{
		for (int x = 0; x <FieldNumX + 1; x++)
		{
			g_v[z*(FieldNumX + 1) + x].pos.x = ((fieldsizex*x) - (FieldSizeX / 2));
			g_v[z*(FieldNumX + 1) + x].pos.y = 0;
			g_v[z*(FieldNumX + 1) + x].pos.z = ((FieldSizeZ / 2) - (fieldsizez*z));
			g_v[z*(FieldNumX + 1) + x].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_v[z*(FieldNumX + 1) + x].texcoord.x = float(x);
			g_v[z*(FieldNumX + 1) + x].texcoord.y = float(z);
			g_v[z*(FieldNumX + 1) + x].normal.x = 0.0f;
			g_v[z*(FieldNumX + 1) + x].normal.y = 1.0f;
			g_v[z*(FieldNumX + 1) + x].normal.z = 0.0f;
		}
	}

	memcpy(&pV[0], &g_v[0], sizeof(VERTEX3D) * ((FieldNumX + 1)*(FieldNumZ + 1)));

	g_pVertexBuffer->Unlock();

	int index_plus = 0;
	for (int z = 0; z< FieldNumZ; z++)
	{
		for (int x = 0; x <= FieldNumX; x++)
		{
			g_index[index_plus] = (FieldNumX + 1)*(z + 1) + x;
			g_index[index_plus + 1] = (FieldNumX + 1)*z + x;
			index_plus += 2;
		}
		if (z != FieldNumZ - 1)
		{
			g_index[index_plus] = g_index[index_plus - 1];
			g_index[index_plus + 1] = (FieldNumX + 1)*(z + 2);
			index_plus += 2;
		}
	}

	int vindex = 0;
	if (FieldNumZ >= 2)
	{
		vindex = (FieldNumX * 2 + 3)*FieldNumZ + FieldNumZ - 2;
	}
	else
	{
		vindex = (FieldNumX + 1)*(FieldNumZ + 1);
	}

	memcpy(&pIndex[0], &g_index[0], sizeof(WORD) * vindex);

	g_pIndexBuffer->Unlock();

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
	pDevice->SetIndices(g_pIndexBuffer);

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

	//ライトの設定
	D3DLIGHT9 light;

	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vecDir(1.0f, 0.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;

	light.Ambient.r = 1.0f;
	light.Ambient.g = 1.0f;
	light.Ambient.b = 1.0f;
	light.Ambient.a = 1.0f;

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
	pDevice->SetIndices(g_pIndexBuffer);

	pDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//FVFの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//行列の設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxfield);

	pDevice->SetTexture(0, g_pTextures[TEXTURE_FIELD]);


	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (FieldNumX + 1)*(FieldNumZ + 1), 0, vindex - 2);
}
