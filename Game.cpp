#include"Game.h"

Game::Game() {



}

Game::~Game() {



}

void Game::init(HINSTANCE* hInst, HWND* wHandle) {

	//Init Engine
	this->engine.init(wHandle);

	//init camera
	this->cam.initDI(hInst, wHandle);

}

void Game::update(float dt) {

	//Read keyboard/mouse input
	this->cam.getInput();

	//Update camera
	this->cam.update(dt);

	//Update engine
	this->engine.update(&this->cam);

}

Camera* Game::getCam(void){

	return &this->cam;

}