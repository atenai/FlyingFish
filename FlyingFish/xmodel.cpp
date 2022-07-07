#include "Xmodel.h"
#include "main.h"
#include <d3dx9.h>
#include <d3d9.h>
#include <math.h>
#include "polygon.h"
#include "camera.h"
#include "input.h"
#include "polygon.h"
#include "judge.h"
#include "polygon3DTest.h"
#include "speedup.h"
#include "coin.h"
#include "coin10.h"
#include "kabe.h"
#include "target.h"
#include "Scene.h"
#include "title.h"
#include "tatemono.h"
#include "UI.h"
#include "sound.h"

#define INIT_POSY	(0.8f)		//�ŏ���Y���W
#define INIT_POSZ	(1.0f)		//�ŏ���Z���W
#define GRAVITY		(-0.015f)	//�d��
#define FRICTION	(0.1f)		//���C�W��
#define REBOUND		(0.15f)		//�n�ʂŒ��˂�W��	
#define JUMP_FORCE	(0.016f)	//���߂�͂̑���
#define JUMP_LIMIT	(80)
#define JUMP_Y		(3.4f)		//Y�����W��
#define JUMP_Z		(2.7f)		//Z�����W��
#define PLAYER_R	(5.0f)		//Player�̋��̔��a
#define CUBE_JUDGE_R (CUBE_HABA*0.7f)	//CUBE�̊O�ډ~���a
#define KAITEN_KAISUU	(7*360)
#define MITIGATE_SPEED	(2.0f)
#define HUKUSEI_NUM (390)//�r�������蔻��̕�����

//X���f���\���p�ϐ�
static LPD3DXMESH pMesh = NULL;
static DWORD dwNumMaterials = 0;
static D3DMATERIAL9* pMeshMaterials = NULL;
static LPDIRECT3DTEXTURE9* pMeshTextures = NULL;


static D3DXMATRIX g_mtxWorld;
static D3DXMATRIX g_mtxWorldS;
static D3DXMATRIX g_mtxWorldT;
static D3DXMATRIX g_mtxWorldR;
static D3DXMATRIX g_mtxWorldRY;
static D3DXMATRIX g_mtxWorldRZ;

static D3DXVECTOR3 g_fieldMin(-5.0f, 0, -5.0f);

static D3DXVECTOR3 g_Pos(0.0f, INIT_POSY, INIT_POSZ);   //xmod pos

static float g_fKaitenZ = 0;//Z��]�p���l�����锠
static float g_fKaitenSpeed = 32.0f;//��]�̃X�s�[�h
static bool g_bKaitenZON = false;//��]�X�C�b�`
static int g_nCheckZ = 0;	//��]�I�����Ɍ��ɖ߂邽�߂ɕK�v�ȕϐ��B�X�s�[�h�A�b�v�I�����_�̊p�x�����B

static float Rot = 0.0f;
static float NewRot = 0.0f;
static float OldRot = 0.0f;


static float g_fTransSpeed = 0.0f;
static float g_fTestSpeedX = 0.0f;
static float g_fTestSpeedY = 0.0f;


static float g_fPressSP = 0.0f;
static float g_fAngle = 0.0f;	//���ˎ��_�̍��E�p�x

static float g_fGravity = GRAVITY;
static bool g_bLaunch = false;
static int g_nChargeCnt = 0;
static bool g_bCharge = false;

static bool g_bPressH = false;

static int oto0;//���y�p�ϐ�
static int oto1;//���y�p�ϐ�
static int oto2;//���y�p�ϐ�
static int oto3;//���y�p�ϐ�
static int oto4;//���y�p�ϐ�
static int oto5;//���y�p�ϐ�
static int oto6;//���y�p�ϐ�
static int oto7;//���y�p�ϐ�
static int oto8;//���y�p�ϐ�

//�\���̂��񂷁i���[�v�j�����邽�߂̕ϐ�
static int Ui = 0;//�\���̂����[�v�����邽�߂̕ϐ�

//�R�C���p�ϐ�
static float coin = 0;//�R�C���p�ϐ�

//�R�C��///////////////////////////////////////
typedef struct
{
	float ataricoinx;//�{���x�̍��Wx
	float ataricoiny;//�{���x�̍��Wy
	float ataricoinz;//�{���x�̍��Wz
	bool tadancoin;//���i�q�b�g�C��
}COIN_ATARI;

static COIN_ATARI Coinatari[COIN_NUM];//�\���̂ŃA�C�e����������邩��錾

//�R�C��///////////////////////////////////////

//�R�C��10///////////////////////////////////////
typedef struct
{
	float ataricoin10x;//�{���x�̍��Wx
	float ataricoin10y;//�{���x�̍��Wy
	float ataricoin10z;//�{���x�̍��Wz
	bool tadancoin10;//���i�q�b�g�C��
}COIN10_ATARI;

static COIN10_ATARI Coin10atari[COIN10_NUM];//�\���̂ŃA�C�e����������邩��錾

//�R�C��10///////////////////////////////////////

//��///////////////////////////////////////
typedef struct
{
	float atarikabex;//�{���x�̍��Wx
	float atarikabey;//�{���x�̍��Wy
	float atarikabez;//�{���x�̍��Wz
	bool tadankabe;//���i�q�b�g�C��
}KABE_ATARI;

static KABE_ATARI Kabeatari[KABE_NUM];//�\���̂ŃA�C�e����������邩��錾
//��///////////////////////////////////////

//�A�C�e��///////////////////////////////////////
typedef struct
{
	float atariitemx;//�{���x�̍��Wx
	float atariitemy;//�{���x�̍��Wy
	float atariitemz;//�{���x�̍��Wz
	bool tadanitem;//���i�q�b�g�C��
	bool g_bKaitenZON;//��]�X�C�b�`
}ITEM_ATARI;

static ITEM_ATARI Itematari[SPEEDUP_NUM];//�\���̂ŃA�C�e����������邩��錾
//�A�C�e��///////////////////////////////////////

//�R�N�[���^���[/////////////////////////////////
static bool tadankokun = true;
//�R�N�[���^���[

//�L���[�u
static bool tadancube = true;
//�L���[�u

//����///////////////////////////////////////
typedef struct
{
	float ataritatemonox;//�{���x�̍��Wx
	float ataritatemonoy;//�{���x�̍��Wy
	float ataritatemonoz;//�{���x�̍��Wz
	bool tadantatemono;//���i�q�b�g�C��
}TATEMONO_ATARI;

static TATEMONO_ATARI Tatemonoatari[TATEMONO_NUM];//�\���̂ŃA�C�e����������邩��錾
//����///////////////////////////////////////

//�����蔻�蕡����//////////////////////////
typedef struct {
	float atarisu;
}HUKUSEI_ATARI;

static HUKUSEI_ATARI Atarihukusei[HUKUSEI_NUM];//�\���̂ŃA�C�e����������邩��錾

int kuri = 0;
//�����蔻�蕡����//////////////////////////

//��Q���̍��W
static D3DXVECTOR3 v3PosCube[CUBE_NUM] = 
{ 
	{ CUBE_1_X, CUBE_1_Y ,CUBE_1_Z },
	{ CUBE_2_X ,CUBE_2_Y ,CUBE_2_Z },
	{ CUBE_3_X, CUBE_3_Y ,CUBE_3_Z },
	{ CUBE_4_X ,CUBE_4_Y ,CUBE_4_Z },
	{ CUBE_5_X, CUBE_5_Y ,CUBE_5_Z } 
};

//�e�N�X�`��
static LPDIRECT3DTEXTURE9 g_pTextures[] = { NULL };

void InitModel(void)
{
	oto0 = 0;//���y�p�ϐ�������
	oto1 = 0;//���y�p�ϐ�������
	oto2 = 0;//���y�p�ϐ�������
	oto3 = 0;//���y�p�ϐ�������
	oto4 = 0;//���y�p�ϐ�������
	oto5 = 0;//���y�p�ϐ�������
	oto6 = 0;//���y�p�ϐ�������
	oto7 = 0;//���y�p�ϐ�������
	oto8 = 0;//���y�p�ϐ�������

	coin = 0;//�R�C��������

	for (Ui = 0; Ui < COIN_NUM; Ui++)//�R�C���\���̂̐������� 
	{
		//	Coinatari[Ui].ataricoinx = 0;
		//	Coinatari[Ui].ataricoiny = 0;
		//	Coinatari[Ui].ataricoinz = 0;
		Coinatari[Ui].tadancoin = true;//���i�q�b�g�C��
	}

	for (Ui = 0; Ui < COIN10_NUM; Ui++)//�R�C��10�\���̂̐������� 
	{
		//	Coinatari[Ui].ataricoinx = 0;
		//	Coinatari[Ui].ataricoiny = 0;
		//	Coinatari[Ui].ataricoinz = 0;
		Coin10atari[Ui].tadancoin10 = true;//���i�q�b�g�C��
	}

	for (Ui = 0; Ui < KABE_NUM; Ui++)//�Ǎ\���̂̐������� 
	{
		//	Itematari[Ui].atariitemx = 0;
		//	Itematari[Ui].atariitemy = 0;
		//	Itematari[Ui].atariitemz = 0;
		Kabeatari[Ui].tadankabe = true;//���i�q�b�g�C��
	}

	for (Ui = 0; Ui < SPEEDUP_NUM; Ui++)//�A�C�e���\���̂̐������� 
	{
	//	Itematari[Ui].atariitemx = 0;
	//	Itematari[Ui].atariitemy = 0;
	//	Itematari[Ui].atariitemz = 0;
		Itematari[Ui].tadanitem = true;//���i�q�b�g�C��
		Itematari[Ui].g_bKaitenZON = false;
	}
	//�R�N�[���^���[
	tadankokun = true;//���i�q�b�g�C��
	//����
	for (Ui = 0; Ui < TATEMONO_NUM; Ui++)//�Ǎ\���̂̐������� 
	{
		//	Itematari[Ui].atariitemx = 0;
		//	Itematari[Ui].atariitemy = 0;
		//	Itematari[Ui].atariitemz = 0;
		Tatemonoatari[Ui].tadantatemono = true;//���i�q�b�g�C��
	}

	//�����蔻�蕡��
	for (Ui = 0; Ui < HUKUSEI_NUM; Ui++)//�����蔻�蕡���\���̂̐������� 
	{
		//	Itematari[Ui].atariitemx = 0;
		//	Itematari[Ui].atariitemy = 0;
		//	Itematari[Ui].atariitemz = 0;
		Atarihukusei[Ui].atarisu = Atarihukusei[Ui - 1].atarisu + 1;//�O�̂����蔻��̈ʒu����+1����
	}

	//�L���[�u
	tadancube = true;//���i�q�b�g�C��

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();


	//X�t�@�C�����烁�b�V�������[�h����
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX("./resource/xmodel/fish_plus.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&dwNumMaterials, &pMesh)))
	{
		MessageBox(NULL, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
		return;
	}

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];//�}�e���A���̃������̈���}�e���A���̐������m�ۂ�pMeshMaterials�ɓ����
	pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];//�e�N�X�`���ɕK�v�ȃ������̈���m�ۂ�pMeshTextures�ɓ����

														   //��Ŋm�ۂ����������̈�ɒ��g�����Ă���
	for (DWORD i = 0; i < dwNumMaterials; i++) //�}�e���A���̐��������[�v����
	{
		pMeshMaterials[i] = d3dxMaterials[i].MatD3D;//�}�e���A���f�[�^������
		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;//Ambient��Diffuse�Ɠ����l�ɂ���

															  //pTextureFilename�����ƂɃe�N�X�`���I�u�W�F�N�g�̐���
		pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL && lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				d3dxMaterials[i].pTextureFilename,
				&pMeshTextures[i])))
			{
				MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
			}
		}
	}
	pD3DXMtrlBuffer->Release();//�������̊J��
};

void UninitModel(void)
{
	if (pMesh != NULL)
	{
		pMesh->Release();//����ŊJ�������
		pMesh = NULL;//�A�h���X���c���Ă���̂�NULL�ŏ㏑��
	}

	g_Pos = { 0.0f, INIT_POSY, INIT_POSZ };
	g_fTransSpeed = 0.0f;
	g_fTestSpeedX = 0.0f;
	g_fTestSpeedY = 0.0f;
	g_fPressSP = 0.0f;
	g_fAngle = 0.0f;
	g_bLaunch = false;
	g_nChargeCnt = 0;
	g_bCharge = false;
	
	g_fKaitenZ = 0;//Z��]�p���l�����锠
	g_fKaitenSpeed = 32.0f;//��]�̃X�s�[�h
};

void UpdateModel(void)
{
	//�J����������������]��
	if (g_bLaunch == false)
	{
		if (GetKeyboardPress(DIK_H))
		{
			if (g_bPressH == false)
			{
				g_bPressH = true;
				g_Pos.y = 840.0f;
			}

		}
		else
		{
			g_bPressH = false;
			g_Pos.y = INIT_POSY;
		}
	}

	//���ˎ��_�̍��E����
	if (GetKeyboardPress(DIK_Q) && g_bLaunch == false)
	{
		g_fAngle -= 0.4f;

		if (g_fAngle <= -10)
		{
			g_fAngle = -10;
		}
	}

	if (GetKeyboardPress(DIK_E) && g_bLaunch == false)
	{
		g_fAngle += 0.4f;

		if (g_fAngle >= 10)
		{
			g_fAngle = 10;
		}
	}

	//��s���̍��E����
	if (GetKeyboardPress(DIK_A) && g_bLaunch == true && g_fTransSpeed != 0)
	{
		g_fTestSpeedX += -0.1f;
		Rot += 2.0f;
		if (Rot >= 45)
		{
			Rot = 45;
		}
	}

	if (GetKeyboardPress(DIK_D) && g_bLaunch == true && g_fTransSpeed != 0)
	{
		g_fTestSpeedX += 0.1f;
		Rot -= 2.0f;
		if (Rot <= -45)
		{
			Rot = -45;
		}
	}

	//�p�x�̖߂�
	if (Rot >= 0)
	{
		Rot -= 1.0f;
	}
	if (Rot <= 0)
	{
		Rot += 1.0f;
	}

	//�����Ă���ԗ͂𗭂߂�
	if (GetKeyboardPress(DIK_SPACE)  && g_bLaunch == false && g_fPressSP < JUMP_FORCE * JUMP_LIMIT && g_bCharge == false && g_bPressH == false)
	{
	

		g_fPressSP += JUMP_FORCE;
		g_nChargeCnt += 4;
	}

	if (g_fPressSP>JUMP_FORCE * JUMP_LIMIT)
	{
		g_bCharge = true;
	}

	if (g_bCharge == true)
	{
		g_fPressSP -= JUMP_FORCE;
		g_nChargeCnt -= 4;
	}

	if (g_fPressSP<0)
	{
		g_bCharge = false;
	}

	//�������u�Ԃɔ��
	if (GetKeyboardRelease(DIK_SPACE) && g_fPressSP != 0 && g_fTransSpeed == 0  && g_bLaunch == false && g_bPressH == false)
	{
		//�T�E���h
		oto7++;
		if (oto7 > 0)
		{
			PlaySound(SOUND_LABEL_BGM007);//���y���Ȃ炷
		}
		oto7 = 0;
		//�T�E���h


		g_fTestSpeedY += g_fPressSP * JUMP_Y;
		g_fTransSpeed = g_fPressSP * JUMP_Z;
		g_fPressSP = 0.0f;
		g_bLaunch = true;
	}

	//�d�͒ǉ�
	if (g_Pos.y > 0 && g_bLaunch == true)
	{
		float fG = 0.0f;


			fG = g_fGravity;
			g_fTestSpeedY += fG;
		
	}

	//�����蔻��///////////////////////////////////////////////////////////////////////////////////////////////
	//�L���[�u�����蔻��
	for (int nCnt = 0; nCnt < CUBE_NUM; nCnt++)
	{
		if (JudgementCheck(g_Pos.x, g_Pos.y, g_Pos.z, v3PosCube[nCnt].x, v3PosCube[nCnt].y, v3PosCube[nCnt].z, PLAYER_R, CUBE_JUDGE_R))
		{
			if (tadancube == true)
			{
				//�T�E���h
				oto0++;
				if (oto0 > 0)
				{
					PlaySound(SOUND_LABEL_BGM002);//���y���Ȃ炷ok
				}
				//oto0 = 0;
				//�T�E���h

				g_fTransSpeed = 0;

				tadancube = false;
			}
		}
	}

	//�R�C�������蔻��		
	for (Ui = 0; Ui < COIN_NUM; Ui++)//�\���̂̐������� 
	{
		if (JudgementCheck(g_Pos.x, g_Pos.y, g_Pos.z, Coinatari[Ui].ataricoinx , Coinatari[Ui].ataricoiny , Coinatari[Ui].ataricoinz, PLAYER_R, COIN_HABA))
			{
				if (Coinatari[Ui].tadancoin == true)
				{
					//�����������e
					//�T�E���h
					oto1++;
					if (oto1 > 0)
					{
						PlaySound(SOUND_LABEL_BGM003);//���y���Ȃ炷ok
					}
					oto1 = 0;
					//�T�E���h


					coin = coin + 1;
					getcoin(coin);//getcoin��coin�̐����𑗂�
					getcoin1(coin);//getcoin��coin�̐����𑗂�
					Coinatari[Ui].tadancoin = false;//���i�q�b�g�C��
				}
			}
	}

	//�R�C��10�����蔻��		
	for (Ui = 0; Ui < COIN10_NUM; Ui++)//�\���̂̐������� 
	{
		if (JudgementCheck(g_Pos.x, g_Pos.y, g_Pos.z, Coin10atari[Ui].ataricoin10x, Coin10atari[Ui].ataricoin10y , Coin10atari[Ui].ataricoin10z, PLAYER_R, COIN10_HABA))
		{
			if (Coin10atari[Ui].tadancoin10 == true)
			{
				//�����������e

				//�T�E���h
				oto2++;
				if (oto2 > 0)
				{
					PlaySound(SOUND_LABEL_BGM003);//���y���Ȃ炷ok
				}
				oto1 = 0;
				//�T�E���h

				coin = coin + 10;
				getcoin(coin);//getcoin��coin�̐����𑗂�
				getcoin1(coin);//getcoin��coin�̐����𑗂�
				Coin10atari[Ui].tadancoin10 = false;//���i�q�b�g�C��
			}
		}
	}

	//�R�C�������蔻��		
	for (Ui = 0; Ui < KABE_NUM; Ui++)//�\���̂̐������� 
	{
		if (JudgementCheck(g_Pos.x, g_Pos.y, g_Pos.z, Kabeatari[Ui].atarikabex, Kabeatari[Ui].atarikabey, Kabeatari[Ui].atarikabez, PLAYER_R, KABE_HABA))
		{
			if (Kabeatari[Ui].tadankabe == true)
			{
				//�����������e

				//�T�E���h
				oto3++;
				if (oto3 > 0)
				{
					PlaySound(SOUND_LABEL_BGM003);//���y���Ȃ炷ok
				}
				oto1 = 0;
				//�T�E���h


				//g_fTransSpeed = 0;
				coin = coin + 50;
				getcoin(coin);//getcoin��coin�̐����𑗂�
				getcoin1(coin);//getcoin��coin�̐����𑗂�
				Kabeatari[Ui].tadankabe = false;//���i�q�b�g�C��
			}
		}
	}

	//�A�C�e���̂����蔻��
	for (Ui = 0; Ui < SPEEDUP_NUM; Ui++)//�\���̂̐������� 
	{
		if (JudgementCheck(g_Pos.x, g_Pos.y, g_Pos.z, Itematari[Ui].atariitemx, Itematari[Ui].atariitemy , Itematari[Ui].atariitemz, PLAYER_R, ITEM_HABA) == true
			&& Itematari[Ui].g_bKaitenZON == false)
		{
			if (Itematari[Ui].tadanitem == true)
			{
				//
				//coin = coin + 100;
				//getcoin(coin);//getcoin��coin�̐����𑗂�
				//getcoin1(coin);//getcoin��coin�̐����𑗂�

				//�T�E���h
				oto4++;
				if (oto4 > 0)
				{
					PlaySound(SOUND_LABEL_BGM005);//���y���Ȃ炷
				}
				oto4 = 0;
				//�T�E���h

				//���������ꍇ
				g_fTransSpeed += 1.0f;//Z�X�s�[�h�グ��
				Itematari[Ui].g_bKaitenZON = true;//��]�X�C�b�`��true�ɂ���܂��]���J�n����
				Itematari[Ui].tadanitem = false;//���i�q�b�g�C��
			}
		}

		//��]�p�x�̏���
		if (Itematari[Ui].g_bKaitenZON)
		{
			if (g_fKaitenZ < KAITEN_KAISUU)
			{
				g_fKaitenZ += g_fKaitenSpeed;//Z��]�X�s�[�h�����̐��l�ɂ���
				g_fKaitenSpeed *= 0.988f;

				if (g_fKaitenSpeed < MITIGATE_SPEED && ((int)g_fKaitenZ) % 360>30 && ((int)g_fKaitenZ) % 360 < 330)
				{
					g_fKaitenSpeed = MITIGATE_SPEED;
				}

			}
			else
			{
				Itematari[Ui].g_bKaitenZON = false;
			}
		}


		if (g_fTransSpeed <= 0)
		{
			g_bKaitenZON = false;//��]�X�C�b�`��false�ɂ���܂��]���~�߂�
			g_fKaitenZ = 0;
			g_fKaitenSpeed = 32.0f;//��]�̃X�s�[�h
		}
	}

	for (kuri = 0; kuri < HUKUSEI_NUM; kuri++)//�����蔻��𕡐��쐬����
	{
		//�R�N�[���^���[
		if (JudgementCheck(g_Pos.x, g_Pos.y, g_Pos.z, POS_X, POS_Y + Atarihukusei[kuri].atarisu, POS_Z + 10, PLAYER_R, KOKUN_HABA))
		{
			if (tadankokun == true)
			{
				//�����������e

				//�T�E���h
				oto5++;
				if (oto5 > 0)
				{
					PlaySound(SOUND_LABEL_BGM006);//���y���Ȃ炷
				}
				//oto5 = 0;
				//�T�E���h

				coin = coin + 100;
				getcoin(coin);//getcoin��coin�̐����𑗂�
				getcoin1(coin);//getcoin��coin�̐����𑗂�
				tadankokun = false;//���i�q�b�g�C��
			}
		}
	}
	

	for (kuri = 0; kuri < HUKUSEI_NUM; kuri++)//�����蔻��𕡐��쐬����
	{
		//���������蔻��		
		for (Ui = 0; Ui < TATEMONO_NUM; Ui++)//�\���̂̐������� 
		{
	
			if (JudgementCheck(g_Pos.x, g_Pos.y, g_Pos.z, Tatemonoatari[Ui].ataritatemonox + 5, Tatemonoatari[Ui].ataritatemonoy + Atarihukusei[kuri].atarisu, Tatemonoatari[Ui].ataritatemonoz - 10, PLAYER_R, TATEMONO_HABA))
			{
				if (Tatemonoatari[Ui].tadantatemono == true)
				{
					//�����������e

					//�T�E���h
				oto6++;
				if (oto6 > 0)
				{
					PlaySound(SOUND_LABEL_BGM002);//���y���Ȃ炷ok
				}
				//oto6 = 0;
				//�T�E���h

					g_fTransSpeed = 0;
					//coin = coin + 1;
					//getcoin(coin);//getcoin��coin�̐����𑗂�
					//getcoin1(coin);//getcoin��coin�̐����𑗂�
					Tatemonoatari[Ui].tadantatemono = false;//���i�q�b�g�C��
				}
			}
		}
	}
	
	//�����蔻��///////////////////////////////////////////////////////////////////////////////////////////////

	//���U���g��ʂֈڍs
	if (g_Pos.y == 0 && g_fTransSpeed == 0) {
		ChangeScene(SCENE_RESULT);
	}


	//���W�X�V
	g_Pos.y += g_fTestSpeedY;

	g_Pos.z += g_fTransSpeed * cos(g_fAngle / 45);

	g_Pos.x += g_fTransSpeed * sin(g_fAngle / 45);
	g_Pos.x += g_fTestSpeedX;

	//�n�ʂɗ������ꍇ
	if (g_Pos.y < 0)
	{
		g_fTestSpeedY = -g_fTestSpeedY * REBOUND;
		g_fTestSpeedX = +g_fTestSpeedX * 0.89f;

		//���C��
		if (g_fTransSpeed > 0)
		{
			g_fTransSpeed -= FRICTION;
		}
		else
		{
			g_fTransSpeed = 0.0f;
		}
	}

	//�n�ʂ֓˂��h�������ꍇ�n�ʂ֖߂�
	if (g_Pos.y < g_fieldMin.y)
	{
		g_Pos.y = g_fieldMin.y;
	}

	//�s��v�Z
	D3DXMatrixRotationY(&g_mtxWorldRY, D3DXToRadian(90 + g_fAngle));		//rotation�AY����]
	D3DXMatrixRotationZ(&g_mtxWorldRZ, D3DXToRadian(NewRot + Rot));		//rotation�AZ����]
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorldRY, &g_mtxWorldRZ);		//rotationY+rotationZ=world

	//��]�v�Z
	D3DXMatrixRotationZ(&g_mtxWorldRZ, D3DXToRadian(g_fKaitenZ));//Z��]�p�֐�
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &g_mtxWorldRZ);//����

	D3DXMatrixScaling(&g_mtxWorldS, 0.3f, 0.3f, 0.3f);					//scale�A����k��
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &g_mtxWorldS);			//rotation+scale=world

	D3DXMatrixTranslation(&g_mtxWorldT, g_Pos.x, g_Pos.y, g_Pos.z);		//move�A���s�ړ�
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &g_mtxWorldT);			//move+world = world

};

void DrawModel(void)
{
	if (g_bPressH == false)
	{
		LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

		DrawUI(TEX_8, SCREEN_WIDTH - 88, 400, 75, 400, 0, 0, 682, 768);
		DrawUI(TEX_9, SCREEN_WIDTH-80, 406, 60, 400, 0, 0, 300, 300);
		DrawUI(TEX_5, SCREEN_WIDTH - 80, 730 - g_nChargeCnt, 60, 380, 0, 0, 864, 768);

		//���[���h�s��
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

		//Direct3D�ɂ�����u���b�V���v�́u�����_�����O�v�́u�}�e���A���v����ɂ��Ă��܂��B
		//Direct3D�ɂ�����u�I�u�W�F�N�g�f�[�^�v�́u�`��v�́u�\�ʓI�������v����ɂ��Ă��܂��B
		for (DWORD i = 0; i < dwNumMaterials; i++) //�}�e���A���̐��������[�v����
		{
			pDevice->SetMaterial(&pMeshMaterials[i]);//�}�e���A����ݒ肷��
			pDevice->SetTexture(0, pMeshTextures[i]);//�e�N�X�`����ݒ肷��
			pMesh->DrawSubset(i);//�����_�����O����
		}
	}
};

D3DXVECTOR3 GetPos()
{
	return g_Pos;
}

//�R�C���̂����蔻��ɐ��l�𑗂�
void ataricoin(int coinsu, float ix, float iy, float iz)
{
	Coinatari[coinsu].ataricoinx = ix;//ix�̒l��ataribonbex�ɓ����
	Coinatari[coinsu].ataricoiny = iy;//iy�̒l��ataribonbey�ɓ����
	Coinatari[coinsu].ataricoinz = iz;//iz�̒l��ataribonbez�ɓ����
}

//�R�C��10�̂����蔻��ɐ��l�𑗂�
void ataricoin10(int coinsu, float ix, float iy, float iz)
{
	Coin10atari[coinsu].ataricoin10x = ix;//ix�̒l��ataribonbex�ɓ����
	Coin10atari[coinsu].ataricoin10y = iy;//iy�̒l��ataribonbey�ɓ����
	Coin10atari[coinsu].ataricoin10z = iz;//iz�̒l��ataribonbez�ɓ����
}

//�ǂ̂����蔻��ɐ��l�𑗂�
void atarikabe(int kabesu, float ix, float iy, float iz)
{
	Kabeatari[kabesu].atarikabex = ix;//ix�̒l��ataribonbex�ɓ����
	Kabeatari[kabesu].atarikabey = iy;//iy�̒l��ataribonbey�ɓ����
	Kabeatari[kabesu].atarikabez = iz;//iz�̒l��ataribonbez�ɓ����
}

//�A�C�e���̂����蔻��ɐ��l�𑗂�
void atariitem(int itemsu, float ix, float iy, float iz)
{
	Itematari[itemsu].atariitemx = ix;//ix�̒l��ataribonbex�ɓ����
	Itematari[itemsu].atariitemy = iy;//iy�̒l��ataribonbey�ɓ����
	Itematari[itemsu].atariitemz = iz;//iz�̒l��ataribonbez�ɓ����
}

//�ǂ̂����蔻��ɐ��l�𑗂�
void ataritatemono(int tatemonosu, float ix, float iy, float iz)
{
	Tatemonoatari[tatemonosu].ataritatemonox = ix;//ix�̒l��ataribonbex�ɓ����
	Tatemonoatari[tatemonosu].ataritatemonoy = iy;//iy�̒l��ataribonbey�ɓ����
	Tatemonoatari[tatemonosu].ataritatemonoz = iz;//iz�̒l��ataribonbez�ɓ����
}
