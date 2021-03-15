#pragma once
#include "uge.h"
#include "ugeapi.h"
#include <string>

namespace uge {



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

	//d3d����
	const int D3DFVF_UGEVERTEX = (D3DFVF_XYZ | D3DFVF_TEX1);
	const int VERTEX_BUFFER_SIZE = 40;

	// ����ʵ�ֽӿ���
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

		virtual TextureList* LoadTexture(const char* filename, bool bMipmap = false) override;
		virtual TextureList* LoadWzl(const char* filename,int sort) override;
		virtual void DxRenderQuad(ugeQuad* quad) override;

		// ��Ա����
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static UGEI* Create();

	private:
		UGEI();
		~UGEI();
		void _SetErrMsg(const char* error);

		//directX9
		bool _DxInit();
		bool _InitResource();									//��ʼ����Դ
		void _Render();											//��Ⱦ
		void _BeginScene();
		void _EndScene();
		void _Clear(const ugeColor32 color);					//����
		void _SetProjectionMatrix();							//���þ���
		const char* _FormatStr(const D3DFORMAT fmt);			//��ȡFMT��ʽ�İ�

	private:
		//����
		HWND hwnd;						//��ǰ���ھ��
		bool windowed;					//����ģʽ
		std::string win_title;			//���ڱ���
		int screen_width;				//��Ļ��
		int screen_height;				//��Ļ��
		bool (*frame_func)();			//֡�ص�����ָ��
		bool (*update_func)();			//���»ص�����ָ��
		Game* game;						//Ӧ�ó���ָ��

		//�ڲ�
		HINSTANCE h_instance;			//��ǰʵ��
		HWND hwnd_parent;				//��-���ھ��
		bool active;					//�����Ƿ񼤻�
		RECT rect_windowed;				//���ڿͻ�������
		LONG style_windowed;			//���ڷ��
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
		TextureList* _texture_list;		//��������
		TextureList* _cur_tex;			//��ǰ����
		bool _z_buffer;					//�Ƿ���z����

		UgeIDirect3D* _d3d;				//d3d����
		UgeDevice* _d3d_device;			//d3d�豸

		D3DXMATRIX _proj_matrix;		//ͶӰ����
		D3DXMATRIX _view_matrix;		//�ӿھ���

		UgeVertexBuffer* _vertex_buffer;//���㻺��
		UgeIndexBuffer* _index_buffer;	//��������
		ugeVertex* _vert_array;			//���㻺������

		int _n_prim;					//��ǰͼԴ��
		UgeTexture* _texture1;			//����
		UgeTexture* _texture2;			//����
		UgeTexture* _texture3;			//����
		UgeTexture* _texture4;			//����
	};

	extern UGEI* pUge;
}