#include"Camera.h"

Camera::Camera() {

	//Camera
	this->camForward = { 0.0f, 0.0f, 1.0f, 0.0f };
	this->camRight = { 1.0f, 0.0f, 0.0f, 0.0f };
	this->camUp = { 0.0f, 1.0f, 0.0f, 0.0f };
	this->camY = { 0.0f, 1.0f, 0.0f, 0.0f };

	this->camTarget = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->position = { 0.0f, 0.0f, 0.0f, 0.0f };

	this->proj = DirectX::XMMatrixPerspectiveFovLH((DirectX::XM_PI * 0.45f), (WIN_WIDTH / WIN_HEIGHT), 0.1f, 10000.0f);;

	this->camRotation = DirectX::XMMatrixIdentity();
	this->camView = DirectX::XMMatrixIdentity();

	this->speed = 0.0f;
	this->moveX = 0.0f;
	this->moveZ = 0.0f;
	this->yaw = 0.0f;
	this->pitch = 0.0f;

	//Keyboard and mouse
	this->keyboard = nullptr;
	this->mouse = nullptr;
	this->dI = nullptr;

}

Camera::~Camera() {

	//Unaquire mouse/keyboard from this application
	if (this->keyboard) {

		this->keyboard->Unacquire();
		this->keyboard->Release();

	}

	if (this->mouse) {

		this->mouse->Unacquire();
		this->mouse->Release();

	}

	if (this->dI) {

		this->dI->Release();

	}

}

void Camera::getInput() {

	//Stores mouse/keyboard info
	BYTE keyboardState[256];

	//Acuires the mouse and keyboard for this application(incase it is used elsewhere)
	this->keyboard->Acquire();
	this->mouse->Acquire();

	//Get current state of mouse/keyboard
	this->keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	this->mouse->GetDeviceState(sizeof(DIMOUSESTATE), &this->currentMouseState);

	//Store directions
	bool x = false, z = false;

	//Update speed value
	if (keyboardState[DIK_LSHIFT] & 0x80) {

		this->speed = 150.0f;

	}
	else {

		this->speed = 50.0f;

	}

	//Keyboard input
	if (keyboardState[DIK_A] & 0x80) {

		this->moveX -= this->speed;
		x = true;

	}
	else if (keyboardState[DIK_D] & 0x80) {

		this->moveX += this->speed;
		x = true;

	}

	if (keyboardState[DIK_W] & 0x80) {

		this->moveZ += this->speed;
		z = true;

	}
	else if (keyboardState[DIK_S] & 0x80) {

		this->moveZ -= this->speed;
		z = true;

	}

	//Keeps same speed when moving in one or two directions
	if (x && z) {

		this->moveX /= 2;
		this->moveZ /= 2;

	}

	//Check mouse input
	if (this->currentMouseState.lX != this->previousMouseState.lX) {

		this->yaw += this->currentMouseState.lX * 0.005f;

		//Save previous mouse state
		this->previousMouseState = this->currentMouseState;

	}

	if (this->currentMouseState.lY != this->previousMouseState.lY) {

		this->pitch += this->currentMouseState.lY * 0.005f;

		//Save previous mouse state
		this->previousMouseState = this->currentMouseState;

	}

}

void Camera::update(float dt) {

	//Rotation matrix, rotates X, Y & Z
	this->camRotation = DirectX::XMMatrixRotationRollPitchYaw(this->pitch, this->yaw, 0.0f);

	//Transforms defaultForward vector with rotation matrix
	this->camTarget = DirectX::XMVector3TransformCoord({ 0.0f, 0.0f, 1.0f, 0.0f }, this->camRotation);

	//Normalize camTarget vector
	this->camTarget = DirectX::XMVector3Normalize(this->camTarget);

	//Rotation matrix around Y
	DirectX::XMMATRIX matRotY = DirectX::XMMatrixRotationY(this->yaw);

	//Transform camera vectors with rotation matrix
	this->camRight = DirectX::XMVector3TransformCoord({ 1.0f, 0.0f, 0.0f, 0.0f }, matRotY);
	this->camForward = DirectX::XMVector3TransformCoord({ 0.0f, 0.0f, 1.0f, 0.0f }, matRotY);
	this->camUp = DirectX::XMVector3TransformCoord(this->camUp, matRotY);

	//Apply movement to the camera
	this->position = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->camRight, (this->moveX * dt)), this->position);
	this->position = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->camForward, (this->moveZ * dt)), this->position);

	//Reset movement values
	this->moveX = 0.0f;
	this->moveZ = 0.0f;

	//Add the position to the camera target matrix
	this->camTarget = DirectX::XMVectorAdd(this->position, this->camTarget);

	//Set values for the camView matrix which is the final view
	this->camView = DirectX::XMMatrixLookAtLH(this->position, this->camTarget, this->camUp);

}

bool Camera::initDI(HINSTANCE* hInst, HWND* wHandle) {

	bool result = true;

	//Create direct input object
	HRESULT hr = DirectInput8Create(
		*hInst,				//Application instance
		DIRECTINPUT_VERSION,//Version of DirectInput to use
		IID_IDirectInput8,	//Interface of DirectInput to use
		(void**)&this->dI,	//Pointer to DirectInput object
		NULL);				//COM

	if (FAILED(hr)) {

		result = false;

	}
	else {

		//Create Keyboard
		hr = this->dI->CreateDevice(
			GUID_SysKeyboard,	//GUID Flag
			&this->keyboard,	//Device to use
			NULL				//COM
		);

		if (FAILED(hr)) {

			result = false;

		}
		else {

			//Create Mouse
			hr = this->dI->CreateDevice(
				GUID_SysMouse,	//GUID Flag
				&this->mouse,	//Device to use
				NULL			//COM
			);

			if (FAILED(hr)) {

				result = false;

			}
			else {

				//Set data format
				this->keyboard->SetDataFormat(&c_dfDIKeyboard);	//Set type of device
				this->keyboard->SetCooperativeLevel(*wHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);	//Flags

				this->mouse->SetDataFormat(&c_dfDIMouse);		//Set type of device
				this->mouse->SetCooperativeLevel(*wHandle, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);	//Flags

			}

		}

	}

	return result;

}

DirectX::XMMATRIX Camera::getView(void) const {

	return this->camView;

}

DirectX::XMMATRIX Camera::getProj(void) const{

	return this->proj;

}

DirectX::XMVECTOR Camera::getPosition(void) {

	return this->position;

}
