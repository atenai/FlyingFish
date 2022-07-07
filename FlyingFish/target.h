#ifndef _TARGET_H_
#define _TARGET_H_

#include <d3dx9.h>
#include <d3d9.h>
#include"Xmodel.h"

#define POS_X	(0)
#define POS_Y	(0)
#define POS_Z	(2800)
#define KOKUN_HABA (85.0f)
#define KOKUN_NUM (1)

void InitTarget(void);
void UninitTarget(void);
void UpdateTarget(void);
void DrawTarget(void);

#endif _TARGET_H_