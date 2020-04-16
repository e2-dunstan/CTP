#include "ConsoleControls.h"
#include "Engine.h"

using namespace std;

void ConsoleControls::Init()
{
	consoleState = State::NONE;
	//MainMenu();
}

void ConsoleControls::OnKeyRelease(const unsigned char key, Engine* engine)
{
	if (key == 'm')
		engine->ThrowSphere();


	switch (consoleState)
	{
		case State::NONE:
		{
			if (key == '0' )
			{
				SpawnMenu();
				consoleState = State::SPAWN_OBJ;
			}
			break;
		}
		case State::SPAWN_OBJ:
		{
			if (key == cancel) Init();
			//else if (key == '0') engine->scenes.SpawnRandomBox();
			//else if (key == '1') engine->scenes.SpawnRandomSphere();
			//else if (key == '2')
			//else if (key == '3')
			break;
		}
		case State::GET_DATA:
		{
			break;
		}
		default:
			break;
	}
}

void ConsoleControls::MainMenu()
{
	string str = "";
	str += "-------------------------------------------------------\n";
	str += "Main Menu : please select from the following options...\n";
	str += "-------------------------------------------------------\n";
	str += "\t 0 - spawn a new object.\n";
	str += "\t 1 - output some data.\n";
	str += "\t 2 - toggle debug tools.\n";
	str += "-------------------------------------------------------\n";

	cout << str << endl;
}

void ConsoleControls::SpawnMenu()
{
	string str = "";
	str += "\nWhat object do you wish to spawn?\n";
	str += "\t 0 - box\n";
	str += "\t 1 - sphere\n";
	str += "\t 2 - capsule\n";
	str += "\t 3 - cylinder\n";
	str += "To cancel, press " + to_string(cancel) + ".\n";

	cout << str << endl;
}
