#pragma once
#include <uge.h>
using namespace uge;

bool Start();

int main(int argc, char** argv)
{
	Start();
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nstr, int nCmd) 
{
	Start();
	return 0;
}

bool Start() {
	// ��ȡ�������
	UGE* pUge = ugeCreate(UGE_VERSION);

	// �ص�
	auto app = gameCreate(pUge);

	// ��������������
	pUge->SetGame(app);

	// �����ʼ��
	if (pUge->Initiate() && app->Initiate())
	{
		// ��������
		pUge->Start();
	}
	else
	{
		MessageBox(nullptr, pUge->GetErrMsg().c_str(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
	}
	pUge->Release();
	return false;
}

