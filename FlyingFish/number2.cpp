#include <d3dx9.h>
#include "Number2.h"
#include "Polygon.h"
#include "UI.h"
#include "scene.h"
//void DrawNumber(int �\��������, float �\������X���W, float �\������Y���W)
void DrawNumber2(int n, float x, float y)
{
	int nX = 0, nY = 0;
	if (n < 0 || n>9)
	{
		return;
	}
	if (n > 4)
	{
		nX = n - 5;
		nY = 1;
	}
	else
	{
		nX = n;
		nY = 0;
	}
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));//�|���S���J���[�̐ݒ�
	DrawUI(TEX_10, x, y, NUMBER_WIDTH2, NUMBER_HEIGHT2, 0 + 200 * nX, 0 + 250 * nY, 200, 250);//�e�N�X�`���̕`��
}

//29�͉摜�̐����P���̉��̒���29�~5��145
//32�͉摜�̐����P���̏c�̒���32�~2��64