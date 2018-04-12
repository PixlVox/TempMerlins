#ifndef GAME_H
#define GAME_H

#include"Engine.h"

class Game {

private:

	Camera cam;
	Engine engine;

public:

	Game();
	~Game();

	void init(HINSTANCE* hInst, HWND* wHandle);
	void update(float dt);

	//Get
	Camera* getCam(void);

	//Set


};

#endif // !GAME_H
