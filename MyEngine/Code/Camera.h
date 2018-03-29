#ifndef CAMERA_H
#define CAMERA_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "PreprocessorMacros.h"

#pragma comment (lib, "d3d11.lib")

using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	void Initialize(float width, float height, float x, float y, float z, float pitch, float yaw, float roll);

	XMVECTOR position;
	XMFLOAT3 rotation;
	XMMATRIX rotationMatrix;

	XMMATRIX viewMatirx;
	XMMATRIX projectionMatrix;
};

static const XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
static const XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

#endif