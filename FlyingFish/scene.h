#pragma once
#ifndef _SCENE_H_
#define _SCENE_H_

typedef enum
{
	SCENE_NONE = -1,
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_MAX,
}SCENE_ID;

void InitScenechenger(void);
void UninitScenechenger(void);
void UpdateScenechenger(void);
void DrawScenechenger(void);

SCENE_ID ChangeScene(SCENE_ID id);
#endif
