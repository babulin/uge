#include "ugei.h"

namespace uge {

	extern UGEI* pUGE;

	//+------------------------
	//| ���ô�����Ϣ
	//+------------------------
	void UGEI::SetErrMsg(const char* error)
	{
		Log("%s\n", error);
		err_msg = error;
	}

	//+------------------------
	//| ���ô�����Ϣ
	//+------------------------
	std::string UGEI::GetErrMsg()
	{
		return err_msg;
	}
}