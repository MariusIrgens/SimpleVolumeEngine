// Constructs the system manager
// Sets off the main loop

#include <iostream>
#include "SystemManager.h"

int main()
{
	std::unique_ptr<SystemManager> systemManager = std::make_unique<SystemManager>();
	systemManager->frameloop();
}

// Code by Marius Irgens