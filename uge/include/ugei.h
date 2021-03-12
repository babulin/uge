#pragma once
#include "uge.h"
#include "ugedx9.h"
#include <string>

namespace uge {

	// �ص���������
	typedef bool (*ugeCallback)();

	// ����ӿ���
	class UGEI : public UGE {
	public:
		
		virtual void UGE_CALL SetFrameCallback(const ugeCallback value) override;	//֡�ص�
		virtual void UGE_CALL SetUpdateCallback(const ugeCallback value) override;	//���ݸ���
		virtual void UGE_CALL SetGame(Game* game) override;
		virtual void UGE_CALL SetScreen(int width, int height) override;			//��Ļ���
		virtual void UGE_CALL SetWindowed(bool value) override;						//�Ƿ񴰿�ģʽ
		virtual void UGE_CALL SetTitle(const char* title) override;					//���ڱ���
		virtual bool UGE_CALL Initiate() override;									//��ʼ������
		virtual bool UGE_CALL Start() override;										//����
		virtual void UGE_CALL Release() override;									//�����ͷ�
		virtual std::string GetErrMsg() override;

		// ��Ա����
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static UGEI* Create();

	private:
		UGEI();
		~UGEI();
		void SetErrMsg(const char* error);
	private:
		HINSTANCE h_instance;			//��ǰʵ��
		HWND hwnd;						//��ǰ���ھ��
		HWND hwnd_parent;				//��-���ھ��
		bool active;					//�����Ƿ񼤻�
		std::string win_title;			//���ڱ���
		bool windowed;					//����ģʽ
		int screen_width;				//��Ļ��
		int screen_height;				//��Ļ��
		RECT rect_windowed;				//���ڿͻ�������
		LONG style_windowed;			//���ڷ��

		bool (*frame_func)();			//֡�ص�����ָ��
		bool (*update_func)();			//���»ص�����ָ��
		Game* game;						//Ӧ�ó���ָ��

		bool splash_screen_enabled;		//�Ƿ����ÿ�������

		std::string err_msg;			//������Ϣ
	
		// Timer
		float	 _game_time_s;			//��Ϸ����ʱ��(s)
		uint32_t _pre_time_ms;			//ǰһ֡ʱ��(ms)
		uint32_t _fps_time_ms;			//����1s��֡ʱ��(ms)
		uint32_t _delay_time_ms;		//��֡���ʱ��(ms)
		float	 _delay_time_s;			//��֡���ʱ��(s)
		uint32_t _fixed_delay_ms;		//�̶���ʱ(ms)
		uint32_t _run_fps;				//1���ڼ����е�fps
		uint32_t _fps;					//ǰ1���֡��

		// Graphics
		Graphics *pGraphics;
	};

	extern UGEI* pUge;
}