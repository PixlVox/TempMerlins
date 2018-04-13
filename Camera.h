#ifndef CAMERA_H
#define CAMERA_H

//Used for vector & matrix calculations
#include<d3d11.h>
#include<SimpleMath.h>
#include"Settings.h"

//Used for keyboard/mouse input
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

const Vector4 defRight = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
const Vector4 defForward = Vector4(0.0f, 0.0f, 1.0f, 0.0f);

class Camera {

private:

	//Vectors
	DirectX::XMVECTOR camForward;
	DirectX::XMVECTOR camRight;
	DirectX::XMVECTOR camY;
	DirectX::XMVECTOR camTarget;
	DirectX::XMVECTOR camUp;
	DirectX::XMVECTOR position;

	DirectX::XMMATRIX camRotation;
	DirectX::XMMATRIX camView;
	DirectX::XMMATRIX proj;

	//Camera movement and rotation values
	float speed;
	float moveX;
	float moveZ;
	float yaw;
	float pitch;

	//Keyboard/Mouse controlls
	IDirectInputDevice8* keyboard;
	IDirectInputDevice8* mouse;

	//Previous mouse state and directInput object(used to get input from keyboard)
	DIMOUSESTATE previousMouseState;
	DIMOUSESTATE currentMouseState;
	LPDIRECTINPUT8 dI;

public:

	Camera();
	~Camera();

	void getInput();
	void update(float dt);
	bool initDI(HINSTANCE* hInst, HWND* wHandle);

	//Get
	DirectX::XMMATRIX getView(void) const;
	DirectX::XMMATRIX getProj(void) const;
	DirectX::XMVECTOR getPosition(void);

};

#endif // !CAMERA_H
