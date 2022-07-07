//������������������������������������������������
//�|���S���`��
//������������������������������������������������
#include "main.h"
#include "polygon.h"
#include "math.h"

//������������������������������������������������
//�}�N����`
//������������������������������������������������
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//RHW�͍��W�ϊ��ς�
//������������������������������������������������
//�\���̐錾
//������������������������������������������������

typedef struct
{
	D3DXVECTOR4 pos;			//�}�`�̍��W
	D3DCOLOR color;				//�}�`�̐F
	D3DXVECTOR2 texcoord;	//�e�N�X�`��
}VERTEX_2D;

//������������������������������������������������
//�O���[�o���ϐ�
//������������������������������������������������
static float g_fScale = 1.0f;
static float g_fScaleAdd = 0.02f;

static float g_fAngle = 0.0f;
static float g_fRotationOffsetX = 0.0f;
static float g_fRotationOffsetY = 0.0f;

static int g_nTexNum;

static D3DCOLOR g_Color = 0xffffffff;

static float g_fDouble = 0.0f;

static LPDIRECT3DVERTEXBUFFER9 	g_pVertexBuffer = NULL;
static LPDIRECT3DTEXTURE9 				g_pTextures[TEXTURE_MAX] = { NULL };

static void CreateVertex(float dx, float dy,
	int tcx, int tcy,
	int nTurn);//���_���W��,���_���W��,��,����,�؂�����W��,�؂�����W��,���]�̌���0(��)1(�t)

static void CreateVertexAffine(float dx, float dy,
	int tcx, int tcy,
	int nTurn);//���_���W��,���_���W��,��,����,�؂�����W��,�؂�����W��,���]�̌���0(��)1(�t)

			   //������������������������������������������������
			   //�֐�
			   //������������������������������������������������
bool InitPolygon(void)
{
	HRESULT hr;

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	//�ϐ�������
	g_nTexNum = TEXTURE_UNKNOW;
	g_Color = 0xffffffff;
	g_fScale = 1.0f;
	g_fScaleAdd = 0.02f;

	/****** �e�N�X�`����ǂݍ��� ******/
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		hr = D3DXCreateTextureFromFile(pDevice, g_aTextures[i].fileName, &g_pTextures[i]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "�e�N�X�`�����ǂݍ��߂܂���ł���", "����", MB_OK);
			return false;
		}
	}

	/****** VRAM���؂�� ******/
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,				//4��
		D3DUSAGE_WRITEONLY,			//�������݂����ł��Ȃ�
		FVF_VERTEX_2D,						//�O�ł����Ȃ�
		D3DPOOL_MANAGED,					//�f�o�C�X�ɊǗ���C����
		&g_pVertexBuffer,						//�Ǘ��҂̋��ꏊ���������ނ���
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "�e�N�X�`�����ǂݍ��߂܂���ł���", "����", MB_OK);
	}

	return true;
}

void UninitPolygon(void)
{
	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_pTextures[i] != NULL)
		{
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
	}
}

void UpdatePolygon(void)
{
	g_fScale += g_fScaleAdd;
	if (g_fScale > 1.0f || g_fScale < 0.125f)
	{
		g_fScaleAdd = -g_fScaleAdd;
	}
}

void SetTexture(int texNum)
{
	g_nTexNum = texNum;
}

void SetScale(float fDouble)
{
	g_fDouble = fDouble;
}

void SetPolygonColor(D3DCOLOR color)
{
	g_Color = color;
}

void PrimitiveDrawLine(float x0, float y0, float x1, float y1)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	if (pDevice != NULL)
	{
		// FVF(������g�p���钸�_���)�̐ݒ�
		VERTEX_2D* pV;
		/*g_pVertexBuffer->Lock(��������, �����܂�(0�Ȃ�S��), �^���A�h���X, );*/
		g_pVertexBuffer->Lock(0, sizeof(VERTEX_2D) * 2, (void**)&pV, D3DLOCK_DISCARD);

		pV[0].pos = D3DXVECTOR4(x0 + -0.5f, y0 + -0.5f, 1.0f, 1.0f);
		pV[1].pos = D3DXVECTOR4(x1 + -0.5f, y1 + -0.5f, 1.0f, 1.0f);

		pV[0].color =
			pV[1].color = g_Color;

		pV[0].texcoord = D3DXVECTOR2(0.0f, 0.0f);
		pV[1].texcoord = D3DXVECTOR2(0.0f, 0.0f);

		g_pVertexBuffer->Unlock();

		pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX_2D));

		// FVF(������g�p���钸�_���)�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		pDevice->SetTexture(0, NULL);

		pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, 1);
	}
}

void SetPolygonRotation(float cx, float cy, float angle)
{
	g_fAngle = angle;
	g_fRotationOffsetX = cx;
	g_fRotationOffsetY = cy;
}

void DrawPolygon(float dx, float dy, int tcx, int tcy, int nTurn, bool bAffine)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	if (pDevice == NULL)
	{
		return;
	}

	if (bAffine)
	{
		CreateVertexAffine(dx, dy, tcx, tcy, nTurn);
	}
	else
	{
		CreateVertex(dx, dy, tcx, tcy, nTurn);
	}

	//�p�C�v���C���i�֐����g����j
	//GPU�ƃo�[�e�b�N�X���p�C�v�łȂ�
	pDevice->SetStreamSource(0,									//0�Ԗ�
		g_pVertexBuffer,			//�Ȃ��łق������ꏊ
		0,									//�������痬��
		sizeof(VERTEX_2D));		//�ׂ̒��_�܂łǂ̂��炢�̒���

								//FVF�i������g�p���钸�_���j�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���Z�b�g
	pDevice->SetTexture(0, g_pTextures[g_nTexNum]);

	//�|���S����`���i�o�[�e�b�N�X�o�b�t�@�ɂ�����`�悷��j
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�^�C�v�i4�p�`�j
		0,														//���Ԗڂ���`�悷�邩
		2);													//���p�`�����\�����邩
}

//������������������������������������������������
//���b�N�̂��
//������������������������������������������������
static void CreateVertex(float dx, float dy, int tcx, int tcy, int nTurn)
{
	HRESULT hr;

	/****** �ϐ��錾 ******/
	float u0;
	float v0;
	float u1;
	float v1;

	int dw = g_aTextures[g_nTexNum].texWidth * g_fDouble;
	int dh = g_aTextures[g_nTexNum].texHeight * g_fDouble;

	int tw = g_aTextures[g_nTexNum].Width;
	int th = g_aTextures[g_nTexNum].Height;

	//�e�N�X�`���̔��]�̕���
	if (nTurn == TEXTURE_REAL)//���̂܂�
	{
		u0 = (float)tcx / tw;
		v0 = (float)tcy / th;
		u1 = (float)(tcx + g_aTextures[g_nTexNum].texWidth) / tw;
		v1 = (float)(tcy + g_aTextures[g_nTexNum].texHeight) / th;
	}
	else//���]
	{
		u0 = (float)(tcx + g_aTextures[g_nTexNum].texWidth) / tw;
		v0 = (float)tcy / th;
		u1 = (float)tcx / tw;
		v1 = (float)(tcy + g_aTextures[g_nTexNum].texHeight) / th;
	}

	/*���b�N��������Ƌ^���A�h���X�����炦��*/
	VERTEX_2D * pV;			//�����ɓ��ꂽ��

	hr = g_pVertexBuffer->Lock(0,				//��������
		0,				//�ǂ��܂Łi�O�͑S���j(��������sizeof(VERTEX_2D)*4)
		(void**)&pV,//(void�̃|�C���^�̃|�C���^�^����Ȃ��Ƃ����Ȃ��j
		D3DLOCK_DISCARD);

	if (FAILED(hr))
	{
		MessageBox(NULL, "���b�N�ł��܂���ł���", "����", MB_OK);
	}

	pV[0].pos = D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 1.0f, 1.0f);
	pV[1].pos = D3DXVECTOR4(dx + dw - 0.5f, dy - 0.5f, 1.0f, 1.0f);
	pV[2].pos = D3DXVECTOR4(dx - 0.5f, dy + dh - 0.5f, 1.0f, 1.0f);
	pV[3].pos = D3DXVECTOR4(dx + dw - 0.5f, dy + dh - 0.5f, 1.0f, 1.0f);

	pV[0].color =
		pV[1].color =
		pV[2].color =
		pV[3].color = g_Color;

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u0, v1);
	pV[3].texcoord = D3DXVECTOR2(u1, v1);

	g_pVertexBuffer->Unlock();
}

//������������������������������������������������
//���b�N�̂��(��]�o�[�W�����j
//������������������������������������������������
void CreateVertexAffine(float dx, float dy, int tcx, int tcy, int nTurn)
{
	HRESULT hr;

	/****** �ϐ��錾 ******/
	float u0;
	float v0;
	float u1;
	float v1;

	int tw = g_aTextures[g_nTexNum].Width;
	int th = g_aTextures[g_nTexNum].Height;

	//�e�N�X�`���̔��]�̕���
	if (nTurn == TEXTURE_REAL)//���̂܂�
	{
		u0 = (float)tcx / tw;
		v0 = (float)tcy / th;
		u1 = (float)(tcx + g_aTextures[g_nTexNum].texWidth) / tw;
		v1 = (float)(tcy + g_aTextures[g_nTexNum].texHeight) / th;
	}
	else//���]
	{
		u0 = (float)(tcx + g_aTextures[g_nTexNum].texWidth) / tw;
		v0 = (float)tcy / th;
		u1 = (float)tcx / tw;
		v1 = (float)(tcy + g_aTextures[g_nTexNum].texHeight) / th;
	}

	/*���b�N��������Ƌ^���A�h���X�����炦��*/
	VERTEX_2D * pV;			//�����ɓ��ꂽ��

	hr = g_pVertexBuffer->Lock(0,				//��������
		0,				//�ǂ��܂Łi�O�͑S���j(��������sizeof(VERTEX_2D)*4)
		(void**)&pV,//(void�̃|�C���^�̃|�C���^�^����Ȃ��Ƃ����Ȃ��j
		D3DLOCK_DISCARD);

	//�ꎞ�ϐ�
	int dw = g_aTextures[g_nTexNum].texWidth * g_fDouble;
	int dh = g_aTextures[g_nTexNum].texHeight * g_fDouble;

	float x = -g_fRotationOffsetX;
	float x_w = dw - g_fRotationOffsetX;
	float y = -g_fRotationOffsetY;
	float y_h = dh - g_fRotationOffsetY;

	dx += g_fRotationOffsetX;
	dy += g_fRotationOffsetY;

	if (FAILED(hr))
	{
		MessageBox(NULL, "���b�N�ł��܂���ł���", "����", MB_OK);
	}

	pV[0].pos = D3DXVECTOR4(dx + x		 * cosf(g_fAngle) - y		 * sinf(g_fAngle) - 0.5f, dy + x		* sinf(g_fAngle) + y		* cos(g_fAngle) - 0.5f, 1.0f, 1.0f);
	pV[1].pos = D3DXVECTOR4(dx + x_w	 * cosf(g_fAngle) - y		 * sinf(g_fAngle) - 0.5f, dy + x_w	* sinf(g_fAngle) + y		* cos(g_fAngle) - 0.5f, 1.0f, 1.0f);
	pV[2].pos = D3DXVECTOR4(dx + x		 * cosf(g_fAngle) - y_h	 * sinf(g_fAngle) - 0.5f, dy + x		* sinf(g_fAngle) + y_h	* cos(g_fAngle) - 0.5f, 1.0f, 1.0f);
	pV[3].pos = D3DXVECTOR4(dx + x_w	 * cosf(g_fAngle) - y_h	 * sinf(g_fAngle) - 0.5f, dy + x_w	* sinf(g_fAngle) + y_h	* cos(g_fAngle) - 0.5f, 1.0f, 1.0f);

	pV[0].color =
		pV[1].color =
		pV[2].color =
		pV[3].color = g_Color;

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u0, v1);
	pV[3].texcoord = D3DXVECTOR2(u1, v1);

	g_pVertexBuffer->Unlock();
}