#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"

//�֐��|�C���^�̏�����
//typedef {void(�߂�l) (*SceneFunc(�ϐ���))(�����̃|�C���^���������Ƃ��\)}(���J�b�R���^,�����̂�typedef�ō���Ă���);
typedef void(*SceneFunc)(void);
static const SceneFunc pInit[] = {
	InitTitle,
	InitGame,
	InitResult };
static const SceneFunc pUninit[] = {
	UninitTitle,
	UninitGame,
	UninitResult };
static const SceneFunc pUpdate[] = {
	UpdateTitle,
	UpdateGame,
	UpdateResult };
static const SceneFunc pDraw[] = {
	DrawTitle,
	DrawGame,
	DrawResult };

#define START_SCENE (SCENE_TITLE)

static SCENE_ID g_nID = START_SCENE;
static SCENE_ID g_nNextID = START_SCENE;

void InitScenechenger(void)
{
	pInit[g_nID]();
}

void UninitScenechenger(void)
{
	pUninit[g_nID]();
}

void UpdateScenechenger(void)
{
	pUpdate[g_nID]();
}

void DrawScenechenger(void)
{
	pDraw[g_nID]();

	if (g_nNextID != g_nID)
	{
		UninitScenechenger();
		g_nID = g_nNextID;
		InitScenechenger();
	}
}

SCENE_ID ChangeScene(SCENE_ID id)
{
	g_nNextID = id;
	return g_nID;
}
