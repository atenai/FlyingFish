#include <d3dx9.h>
#include <d3d9.h>
#include"lighting.h"
#include"main.h"

void DrawLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	D3DLIGHT9 light;

	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vecDir(0.0f, 5.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;

	light.Ambient.r = 1.0f;
	light.Ambient.g = 1.0f;
	light.Ambient.b = 1.0f;
	light.Ambient.a = 1.0f;

	pDevice->SetLight(0, &light);
}