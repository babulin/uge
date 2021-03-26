#pragma once

#include <Windows.h>

#define	UGE_VERSION 0x100
#define DX9_VERSION 0x09;

#include "ugedefs.h"
#include "ugehelp.h"
#include "ugecolor.h"
#include "ugegame.h"

namespace uge {

	// ����
	using UTEXTURE = uintptr_t;

	// ����
	struct ugeLine {
		float x;		//����x
		float y;		//����y
		float x1;		//ƫ��x
		float y1;		//ƫ��y
		uint32_t col;	//��ɫ
	};

	// ������Դ
	struct ugeImage{
		float x;		//����x
		float y;		//����y
		float px;		//ƫ��x
		float py;		//ƫ��y
		int width;		//ͼƬ��
		int height;		//ͼƬ��
		UTEXTURE tex;	//�����ַ
		char path[_MAX_DIR];
		int sort;
	};

	// ������Դ
	struct ugeAnimation {
		float x;			//����x
		float y;			//����y
		int total;			//��֡��
		int curFrame;		//��ǰ֡
		float rate;			//����֡��
		float time;			//ǰ֡ʱ��
		ugeImage image[8];	//ͼƬ����
	};

	// �ص���������
	typedef bool (*ugeCallback)();

	// ����ӿ�
	class UGE {
	public:
		virtual ~UGE() = default;
		virtual void UGE_CALL SetFrameCallback(const ugeCallback value) = 0;
		virtual void UGE_CALL SetUpdateCallback(const ugeCallback value) = 0;
		virtual void UGE_CALL SetGame(ugeGame* game) = 0;
		virtual void UGE_CALL SetScreen(int width,int height) = 0;
		virtual void UGE_CALL SetTitle(const char* title) = 0;
		virtual void UGE_CALL SetWindowed(bool value) = 0;
		virtual bool UGE_CALL Initiate() = 0;
		virtual bool UGE_CALL Start() = 0;
		virtual void UGE_CALL Release() = 0;
		virtual std::string GetErrMsg() = 0;

		virtual bool LoadTexture(const char* filename, bool bMipmap = false) = 0;
		virtual bool LoadWzl(const char* path, int sort, ugeImage* image) = 0;
		virtual bool LoadWzl(const char* path, int sort,int total, ugeAnimation* animation) = 0;
		virtual bool ReleaseWzl(ugeImage* image) = 0;
		virtual bool ReleaseWzl(ugeAnimation* animation) = 0;
		virtual void DxRenderQuad(ugeImage* image, bool fillMode = false) = 0;
		virtual void DxRenderQuad(ugeAnimation* animation, bool fillMode = false) = 0;
		virtual void DxRenderQuad(ugeLine* line) = 0;
	};

	ugeGame* gameCreate(UGE* _uge);

	extern "C" {
		UGE_EXPORT UGE* UGE_CALL ugeCreate(const int ver);
	}

}


