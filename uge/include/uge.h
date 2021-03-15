#pragma once

#include <Windows.h>

#define	UGE_VERSION 0x100
#define DX9_VERSION 0x09;

#include "ugedefs.h"
#include "ugehelp.h"
#include "ugecolor.h"

namespace uge {

	// ����ṹ��
	struct ugeVertex {
		float x, y; // screen position
		float z; // Z-buffer depth 0..1
		//float rhw;
		//uint32_t col; // color
		float tx, ty; // texture coordinates
	};

	//wzxͷ���ṹ
	struct WzxHeader {
		char description[20];	//www.shandagames.com
		int x1[6];				//ռλ
		int imageCount;			//ͼƬ����
	};

	//Wzlͷ���ṹ
	struct WzlHeader {
		char description[44];	//www.shandagames.com
		int imageCount;			//ͼƬ����
		int x3[4];				//ռλ
	};

	//WzlͼƬ��Ϣ�ṹ
	struct WzlBmpInfo {
		BYTE pixelFormat;	//ͼƬλ��
		BYTE compressed;	//��ʾͼƬ�����Ƿ񾭹�gzipѹ��
		BYTE reserve;		//ռλ
		BYTE compressLevel; //���ͼƬ������ѹ����������ͱ�ʾѹ���ĵȼ�
		short width;		//ͼƬ���
		short height;		//ͼƬ�߶�
		short x;			//ƫ��x
		short y;			//ƫ��y
		int size;			//ͼƬ���ݳ���[ѹ�����]
	};

	// �ص���������
	typedef bool (*ugeCallback)();

	// Ӧ�ó���
	class Game {
	public:
		Game() {}
		virtual bool Config() = 0;
		virtual bool Initiate() = 0;
		virtual bool Update() = 0;
		virtual bool Show() = 0;
	};

	Game* gameCreate();

	// ����ӿ�
	class UGE {
	public:
		virtual ~UGE() = default;
		virtual void UGE_CALL SetFrameCallback(const ugeCallback value) = 0;
		virtual void UGE_CALL SetUpdateCallback(const ugeCallback value) = 0;
		virtual void UGE_CALL SetGame(Game* game) = 0;
		virtual void UGE_CALL SetScreen(int width,int height) = 0;
		virtual void UGE_CALL SetTitle(const char* title) = 0;
		virtual void UGE_CALL SetWindowed(bool value) = 0;
		virtual bool UGE_CALL Initiate() = 0;
		virtual bool UGE_CALL Start() = 0;
		virtual void UGE_CALL Release() = 0;
		virtual std::string GetErrMsg() = 0;
	};

	// ͼ��ӿ�
	class Graphics {
	public:
		virtual ~Graphics() = default;
		virtual bool UGE_CALL Initiate() = 0;
		virtual void UGE_CALL Clear(const ugeColor32 color) = 0;
		virtual void UGE_CALL Render() = 0;
		virtual std::string GetErrMsg() = 0;

	};

	extern "C" {
		UGE_EXPORT UGE* UGE_CALL ugeCreate(const int ver);
	}
}

