#ifndef GAME_H
#define GAME_H

#include"Engine.h"
#include"Level.h"
#include"ObjectImporter.h"


class Game {

private:

	Camera cam;
	Engine engine;
	Level level;
	ObjectImporter objImporter;

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
