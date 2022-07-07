//━━━━━━━━━━━━━━━━━━━━━━━━
//messageboxのmain関数★
//━━━━━━━━━━━━━━━━━━━━━━━━
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
//━━━━━━━━━━━━━━━━━━━━━━━━
//マクロ宣言
//━━━━━━━━━━━━━━━━━━━━━━━━
#define CLASS_NAME "sample"
#define WINDOW_NAME "フライングフィッシュ"

//━━━━━━━━━━━━━━━━━━━━━━━━
//プロトタイプ宣言
//━━━━━━━━━━━━━━━━━━━━━━━━
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

bool Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//━━━━━━━━━━━━━━━━━━━━━━━━
//グローバル変数
//━━━━━━━━━━━━━━━━━━━━━━━━
/*Direct３Dインターフェース*/
LPDIRECT3D9							g_pD3D = NULL;
/*Direct３Dデバイス*/
LPDIRECT3DDEVICE9				g_pD3DDevice = NULL;


//━━━━━━━━━━━━━━━━━━━━━━━━
//本文
//━━━━━━━━━━━━━━━━━━━━━━━━
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE PrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/****** 初期化 ******/
	DWORD dWordStyle = WS_OVERLAPPEDWINDOW &~(WS_THICKFRAME | WS_MAXIMIZEBOX);		//ウィンドウスタイル

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
	AdjustWindowRect(&wr, dWordStyle, false);			//ウィンドウの幅取得

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
		CW_USEDEFAULT,			//左上
		CW_USEDEFAULT,			//左上
		SCREEN_WIDTH,			//右下
		SCREEN_HEIGHT,			//右下
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!Init(hInstance, hWnd, true))
	{
		MessageBox(hWnd, "初期化に失敗しました", "エラー", MB_OK);
		return -1;
	}

	ShowWindow(hWnd, nCmdShow);			//画面表示
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
				//終了処理
				break;
			}
			else
			{
				//メッセージ処理
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//ゲーム処理
			timeBeginPeriod(1);					//時間調整
			NowTime = timeGetTime();
			if ((NowTime - NowTimeOld) >= (1000 / 60))
			{
				//更新
				Update();

				//描画
				Draw();

				NowTimeOld = NowTime;		//前の時間←今の時間
			}
			timeEndPeriod(1);
		}

	}

	/****** 変数宣言 ******/
	return (int)msg.wParam;
}


//━━━━━━━━━━━━━━━━━━━━━━━━
//WndProc関数
//━━━━━━━━━━━━━━━━━━━━━━━━
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg,					//動作の数字がここに入る
	WPARAM wParam,		//追加情報１
	LPARAM lParam)			//追加情報２
{
	switch (uMsg)
	{
	case WM_DESTROY:				//←のメッセージを発給する。終わらせるために終了の確認をする余裕があるくらいの空回りがある
		PostQuitMessage(0);			//←のメッセージを発給する
		Uninit();
		break;

	case WM_PAINT:
		break;

	case WM_KEYDOWN:
		switch (wParam)				//wParamに入力データが入っている
		{
		case VK_ESCAPE:				//[ESC]キーが押されたら
			int nID = MessageBox(hWnd, "ゲームを終了しますか？", "終了メッセージ", MB_YESNO | MB_DEFBUTTON2);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);		//はいだったらWindow破棄（WM_DESTROYが呼ばれる）
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


//━━━━━━━━━━━━━━━━━━━━━━━━
//Init関数
//━━━━━━━━━━━━━━━━━━━━━━━━
bool Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//Direct３Dインターフェースの作成
	g_pD3D = Direct3DCreate9(D3DX_VERSION);
	if (g_pD3D == NULL)
	{
		return false;
	}

	//現在のディスプレイモードを取得
	D3DDISPLAYMODE d3ddm;
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return false;
	}

	//デバイスのプレゼンテーションパラメーター
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//裏画面の幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//裏画面の高さ
	d3dpp.BackBufferFormat = d3ddm.Format;							//描画領域
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;										//スクリーン切り替え
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//デバイス作成制御
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		MessageBox(hWnd, "あなたのパソコンでは動きません", "終了メッセージ", MB_OK | MB_DEFBUTTON2);
		return false;
	}

	//ディスプレイアダプタを表すためのデバイス
	//描画と頂点処理をハードウェアで行う
	/*if(FAILED(g_CreateDevivce(D3DADAPTER_DEFAULT,
	D3DDEVTYPE_HAL,
	hWnd,
	D3DCREATE_SOFTWARE_VERTEXPROCESSINGm
	&d3dpp , */

	//レンダーステートの設定

	//αブレンドを行う
	//SRC		今からかくもの。つまりポリゴン
	//DEST		すでに描画されている画面のこと
	//SRC_RGB * SRC_α 
	//								 + DEST_RGB * (1 - SRC_α)
	//g_pD3DDevice ->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

	//g_pD3DDevice ->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);

	//g_pD3DDevice ->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//サンプラーステートの設定//テクスチャ座標外の取り扱いをどうするか
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);//WRAP = 反復します

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//フィルタリング
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	/****** テクスチャ ******/
	if (!InitPolygon())
	{
		return false;
	}
	InitUI();

	InitKeyboard(hInstance, hWnd);

	InitScenechenger();


	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━
//Uninit関数
//━━━━━━━━━━━━━━━━━━━━━━━━
void Uninit(void)
{
	UninitPolygon();
	UninitUI();
	UninitKeyboard();

	UninitScenechenger();

	/****** デバイスの開放 ******/
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	/****** Direct3Dオブジェクトの開放 ******/
	g_pD3D->Release();
	g_pD3D = NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━
//Update関数
//━━━━━━━━━━━━━━━━━━━━━━━━
void Update(void)
{
	DrawLight();
	UpdateCamera();
	UpdatePolygon();
	UpdateKeyboard();

	UpdateScenechenger();
}

//━━━━━━━━━━━━━━━━━━━━━━━━
//Draw関数
//━━━━━━━━━━━━━━━━━━━━━━━━
void Draw(void)
{
	//Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(142, 209, 224, 255), 1.0f, 0);
		//(矩形の配列（何個分）, NULL , 何をクリアするのか(色、Zバッファ) , クリアカラー(ターゲットが入っていたらその色が有効) , Zバッファのクリア値 , ステンシルのクリア値)
		DrawScenechenger();

		//DirectX3Dいよる描画の終了
		g_pD3DDevice->EndScene();
	}

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

LPDIRECT3DDEVICE9 getD3DDevice(void)
{
	return g_pD3DDevice;
}
