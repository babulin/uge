#pragma once
#include <uge.h>
using namespace uge;
namespace game {
	class Login : public ugeGame {
		bool kDownSpace = false;
	public:
		Login(UGE* pUge) :ugeGame(pUge) {}
		// ͨ�� Game �̳�
		virtual bool Initiate() override;
		virtual bool Update() override;
		virtual bool Show() override;
	};
}