#include "login.h"
#include "character.h"

namespace game {

	ugeImage bg;
	bool Login::Initiate()
	{
		std::cout << "Login::Initiate()" << std::endl;

		//���ص�¼����
		//pUge->LoadWzl("data\\ui", 1813, &bg);

		return true;
	}

	bool Login::Update()
	{
		//std::cout << "Login::Update()" << std::endl;

		bg.x = 0.0f;
		bg.y = 0.0f;
		return true;
	}

	bool Login::Show()
	{
		//std::cout << "Login::Show()" << std::endl;
		//��Ⱦ����
		pUge->DxRenderQuad(&bg);

		return true;
	}
}