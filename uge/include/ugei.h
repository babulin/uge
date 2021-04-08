#pragma once
#include "uge.h"
#include "ugeapi.h"
#include <string>
#include <queue>

namespace uge {

	// ����ṹ��
	struct ugeVertex {
		float x, y; // screen position
		float z; // Z-buffer depth 0..1
		//float rhw;
		uint32_t col; // color
		float tx, ty; // texture coordinates
	};

	//����ṹ
	struct ugeQuad {
		UTEXTURE tex;
		ugeVertex v[4];
		ugeBlendMode blend;
	};

	// UGE ͼԴ���ͳ���(��������)
	enum {
		UGEPRIM_LINES = 2,
		UGEPRIM_TRIPLES = 3,
		UGEPRIM_QUADS = 4,
	};

	//d3d����
	const int D3DFVF_UGEVERTEX = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	const int VERTEX_BUFFER_SIZE = 40000;

	// ����ʵ�ֽӿ���
	class UGEI : public UGE {
	public:
		virtual void UGE_CALL SetFrameCallback(const ugeCallback value) override;	//֡�ص�
		virtual void UGE_CALL SetUpdateCallback(const ugeCallback value) override;	//���ݸ���
		virtual void UGE_CALL SetGame(ugeGame* game) override;
		virtual void UGE_CALL SetScreen(int width, int height) override;			//��Ļ���
		virtual void UGE_CALL GetScreen(int* width, int* height) override;
		virtual void UGE_CALL SetWindowed(bool value) override;						//�Ƿ񴰿�ģʽ
		virtual void UGE_CALL SetTitle(const char* title) override;					//���ڱ���
		virtual bool UGE_CALL Initiate() override;									//��ʼ������
		virtual bool UGE_CALL Start() override;										//����
		virtual void UGE_CALL Release() override;									//�����ͷ�
		virtual std::string GetErrMsg() override;

		//�ṩ��wzl.dll
		virtual UTEXTURE UGE_CALL CreateWzlTexture(int width, int height, int pixelFormat, const RGBQUAD palette[256], byte* dstBuffer) override;
		virtual void UGE_CALL ReleaseWzlTexture(UTEXTURE tex) override;

		//��Ⱦ
		virtual bool LoadTexture(const char* filename, bool bMipmap = false) override;
		virtual void DxRenderQuad(ugeImage* image, bool fillMode = false) override;
		virtual void DxRenderQuad(ugeAnimation* animation, bool fillMode = false) override;
		virtual void DxRenderQuad(ugeLine* line) override;

		//����
		virtual void GetMousePos(float* x, float* y) override;
		virtual bool KeyDown(const int key) override;
		virtual bool KeyUp(const int key) override;
		virtual bool KeyState(const int key) override;

		// ��Ա����
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static UGEI* Create();

	private:
		UGEI();
		~UGEI();
		void _SetErrMsg(const char* error);

		//�����¼�
		void _InputInit();
		void _SetMousePos(const float x, const float y);
		void _UpdateMouse();
		void _KeyboardEvents(const int type,const int key, const int scan, int flags, const int x,const int y);
		void _ClearQueue();

		//directX9
		bool _DxInit();
		bool _InitResource();									//��ʼ����Դ
		void _BeginScene();
		void _EndScene();
		void _Clear(const ugeColor32 color);					//����
		void _SetProjectionMatrix();							//���þ���
		const char* _FormatStr(const D3DFORMAT fmt);			//��ȡFMT��ʽ�İ�

	private:
		// ����
		std::string err_msg;			//������Ϣ

		// ������Ϣ
		HWND hwnd;						//��ǰ���ھ��
		HWND hwnd_parent;				//��-���ھ��
		HINSTANCE h_instance;			//��ǰʵ��
		bool windowed;					//����ģʽ
		bool active;					//�����Ƿ񼤻�
		std::string win_title;			//���ڱ���
		int screen_width;				//��Ļ��
		int screen_height;				//��Ļ��
		RECT rect_windowed;				//���ڿͻ�������
		LONG style_windowed;			//���ڷ��

		//����
		int _v_key;						//��ǰ���µļ�
		int _input_char;				//�����ַ�
		int _zpos;						//����λ��
		float _xpos;					//���x
		float _ypos;					//���y
		bool _mouse_over;				//����Ƿ񾭹�
		bool _is_captured;				//�Ƿ�ѹסһ����

		struct KeyStates {
			bool pressed : 1;
			bool released : 1;
		};
		KeyStates _key_table[256];				//�������Ӧ״̬
		std::queue<ugeInputEvent> _kbev_queue;	//�����¼�����

		// Resoure

		// ��Ϸ����
		bool (*frame_func)();			//֡�ص�����ָ��
		bool (*update_func)();			//���»ص�����ָ��
		bool splash_screen_enabled;		//�Ƿ����ÿ�������
		ugeGame* game;						//Ӧ�ó���ָ��
	
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
		bool _z_buffer;					//�Ƿ���z����
		UgeIDirect3D* _d3d;				//d3d����
		UgeDevice* _d3d_device;			//d3d�豸
		D3DXMATRIX _proj_matrix;		//ͶӰ����
		D3DXMATRIX _view_matrix;		//�ӿھ���
		UgeVertexBuffer* _vertex_buffer;//���㻺��
		UgeIndexBuffer* _index_buffer;	//��������
		ugeVertex* _vert_array;			//���㻺������

		int _n_prim;					//��ǰͼԴ��
	};

	extern UGEI* pUgei;
}