#pragma once
#include <uge.h>
using namespace uge;
namespace game {
	class World : public ugeGame {
	public:
		World(UGE* pUge) :ugeGame(pUge) {}
		// ͨ�� Game �̳�
		virtual bool Initiate() override;
		virtual bool Update() override;
		virtual bool Show() override;
	};
}