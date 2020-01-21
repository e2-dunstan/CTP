#pragma once
#include <iostream>
#include "Utilities.h"

class Engine;

class ConsoleControls
{
public:
	ConsoleControls() = default;
	//ConsoleControls(const Engine& enginePtr) engine(std::make_unique<Engine>(enginePtr)) {}
	~ConsoleControls() = default;

	void Init();
	void OnKeyRelease(const unsigned char key, Engine* engine);

private:
	//std::unique_ptr<Engine> engine = std::make_unique<Engine>();

	void MainMenu();
	void SpawnMenu();

	enum class State { NONE, SPAWN_OBJ, GET_DATA };
	State consoleState = State::NONE;

	const char cancel = 'c';

};