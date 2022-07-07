#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <d3dx9.h>
#include <d3d9.h>

void UpdateCamera(void);

D3DXMATRIX GetViewMatrix(void);
D3DXVECTOR3 GetUp(void);
D3DXVECTOR3 GetAt(void);
D3DXVECTOR3 GetEye(void);

#endif //_CAMERA_H_