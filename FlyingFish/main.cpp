//������������������������������������������������
//messagebox��main�֐���
//������������������������������������������������
#include "main.h"
#include "polygon.h"
#include "input.h"
#include "polygon3DTest.h"
#include "camera.h"
#include "Xmodel.h"
#include "MeshField.h"
#include "Shadow.h"
#include "target.h"
#include "taihou.h"
#include "scene.h"
#include"lighting.h"
#include "UI.h"
//������������������������������������������������
//�}�N���錾
//������������������������������������������������
#define CLASS_NAME "sample"
#define WINDOW_NAME "�t���C���O�t�B�b�V��"

//������������������������������������������������
//�v���g�^�C�v�錾
//������������������������������������������������
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

bool Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//������������������������������������������������
//�O���[�o���ϐ�
//������������������������������������������������
/*Direct�RD�C���^�[�t�F�[�X*/
LPDIRECT3D9							g_pD3D = NULL;
/*Direct�RD�f�o�C�X*/
LPDIRECT3DDEVICE9				g_pD3DDevice = NULL;


//������������������������������������������������
//�{��
//������������������������������������������������
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE PrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/****** ������ ******/
	DWORD dWordStyle = WS_OVERLAPPEDWINDOW &~(WS_THICKFRAME | WS_MAXIMIZEBOX);		//�E�B���h�E�X�^�C��

	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),
		CS_VREDRAW | CS_HREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	RegisterClassEx(&wcex);

	RECT wr = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	AdjustWindowRect(&wr, dWordStyle, false);			//�E�B���h�E�̕��擾

	RECT dr;
	GetWindowRect(GetDesktopWindow(), &dr);

	int nScWidth = wr.right - wr.left;
	int nScHeight = wr.bottom - wr.top;

	int ScCenterX = (dr.right - wr.right) / 2;
	int ScCenterY = (dr.bottom - wr.bottom) / 2;

	HWND hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		dWordStyle,
		CW_USEDEFAULT,			//����
		CW_USEDEFAULT,			//����
		SCREEN_WIDTH,			//�E��
		SCREEN_HEIGHT,			//�E��
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!Init(hInstance, hWnd, true))
	{
		MessageBox(hWnd, "�������Ɏ��s���܂���", "�G���[", MB_OK);
		return -1;
	}

	ShowWindow(hWnd, nCmdShow);			//��ʕ\��
	UpdateWindow(hWnd);

	MSG msg;

	DWORD NowTime = 0;
	DWORD NowTimeOld = 0;



	for (;;)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				//�I������
				break;
			}
			else
			{
				//���b�Z�[�W����
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//�Q�[������
			timeBeginPeriod(1);					//���Ԓ���
			NowTime = timeGetTime();
			if ((NowTime - NowTimeOld) >= (1000 / 60))
			{
				//�X�V
				Update();

				//�`��
				Draw();

				NowTimeOld = NowTime;		//�O�̎��ԁ����̎���
			}
			timeEndPeriod(1);
		}

	}

	/****** �ϐ��錾 ******/
	return (int)msg.wParam;
}


//������������������������������������������������
//WndProc�֐�
//������������������������������������������������
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg,					//����̐����������ɓ���
	WPARAM wParam,		//�ǉ����P
	LPARAM lParam)			//�ǉ����Q
{
	switch (uMsg)
	{
	case WM_DESTROY:				//���̃��b�Z�[�W�𔭋�����B�I��点�邽�߂ɏI���̊m�F������]�T�����邭�炢�̋��肪����
		PostQuitMessage(0);			//���̃��b�Z�[�W�𔭋�����
		Uninit();
		break;

	case WM_PAINT:
		break;

	case WM_KEYDOWN:
		switch (wParam)				//wParam�ɓ��̓f�[�^�������Ă���
		{
		case VK_ESCAPE:				//[ESC]�L�[�������ꂽ��
			int nID = MessageBox(hWnd, "�Q�[�����I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_DEFBUTTON2);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);		//�͂���������Window�j���iWM_DESTROY���Ă΂��j
			}
			break;
		}
		break;

	default:
		break;
	}

	hWnd;
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//������������������������������������������������
//Init�֐�
//������������������������������������������������
bool Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//Direct�RD�C���^�[�t�F�[�X�̍쐬
	g_pD3D = Direct3DCreate9(D3DX_VERSION);
	if (g_pD3D == NULL)
	{
		return false;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	D3DDISPLAYMODE d3ddm;
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return false;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�[
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//����ʂ̕�
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//����ʂ̍���
	d3dpp.BackBufferFormat = d3ddm.Format;							//�`��̈�
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;										//�X�N���[���؂�ւ�
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//�f�o�C�X�쐬����
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		MessageBox(hWnd, "���Ȃ��̃p�\�R���ł͓����܂���", "�I�����b�Z�[�W", MB_OK | MB_DEFBUTTON2);
		return false;
	}

	//�f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X
	//�`��ƒ��_�������n�[�h�E�F�A�ōs��
	/*if(FAILED(g_CreateDevivce(D3DADAPTER_DEFAULT,
	D3DDEVTYPE_HAL,
	hWnd,
	D3DCREATE_SOFTWARE_VERTEXPROCESSINGm
	&d3dpp , */

	//�����_�[�X�e�[�g�̐ݒ�

	//���u�����h���s��
	//SRC		�����炩�����́B�܂�|���S��
	//DEST		���łɕ`�悳��Ă����ʂ̂���
	//SRC_RGB * SRC_�� 
	//								 + DEST_RGB * (1 - SRC_��)
	//g_pD3DDevice ->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

	//g_pD3DDevice ->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);

	//g_pD3DDevice ->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//�T���v���[�X�e�[�g�̐ݒ�//�e�N�X�`�����W�O�̎�舵�����ǂ����邩
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);//WRAP = �������܂�

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�t�B���^�����O
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	/****** �e�N�X�`�� ******/
	if (!InitPolygon())
	{
		return false;
	}
	InitUI();

	InitKeyboard(hInstance, hWnd);

	InitScenechenger();


	return true;
}

//������������������������������������������������
//Uninit�֐�
//������������������������������������������������
void Uninit(void)
{
	UninitPolygon();
	UninitUI();
	UninitKeyboard();

	UninitScenechenger();

	/****** �f�o�C�X�̊J�� ******/
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	/****** Direct3D�I�u�W�F�N�g�̊J�� ******/
	g_pD3D->Release();
	g_pD3D = NULL;
}

//������������������������������������������������
//Update�֐�
//������������������������������������������������
void Update(void)
{
	DrawLight();
	UpdateCamera();
	UpdatePolygon();
	UpdateKeyboard();

	UpdateScenechenger();
}

//������������������������������������������������
//Draw�֐�
//������������������������������������������������
void Draw(void)
{
	//Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(142, 209, 224, 255), 1.0f, 0);
		//(��`�̔z��i�����j, NULL , �����N���A����̂�(�F�AZ�o�b�t�@) , �N���A�J���[(�^�[�Q�b�g�������Ă����炻�̐F���L��) , Z�o�b�t�@�̃N���A�l , �X�e���V���̃N���A�l)
		DrawScenechenger();

		//DirectX3D�����`��̏I��
		g_pD3DDevice->EndScene();
	}

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

LPDIRECT3DDEVICE9 getD3DDevice(void)
{
	return g_pD3DDevice;
}
