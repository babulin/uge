#include <ugex.h>
#include "login.h"
#include "character.h"
#include "world.h"

uge::ugeGame* uge::gameCreate(UGE *uge)
{		
	//���ò���
	uge->SetTitle("UGE Initiate - tut01 v1.0.0");
	uge->SetWindowed(true);
	uge->SetScreen(1920, 1080);
	std::cout << "gameCreate()" << std::endl;
	return new game::World(uge);
}