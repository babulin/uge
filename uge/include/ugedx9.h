#pragma once
#include "uge.h"
#include "ugeapi.h"
#include <string>

namespace uge {

	//�����ʽ
	//const int D3DFVF_UGEVERTEX = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	const int D3DFVF_UGEVERTEX = (D3DFVF_XYZ | D3DFVF_TEX1);
	const int VERTEX_BUFFER_SIZE = 40;

	class UGEDX9 : public Graphics {
		public:
			virtual bool Initiate() override;						//��ʼ��
			virtual void Clear(const ugeColor32 color) override;	//����
			virtual void Render() override;							//��Ⱦ
			virtual std::string GetErrMsg() override;				//��ȡ������Ϣ
		public:
			UGEDX9(HWND hwnd, int width, int height);
			~UGEDX9();
		private:
			void SetErrMsg(const char* error);						//���ô�����Ϣ
			void SetProjectionMatrix();								//���þ���
			bool InitResource();									//��ʼ����Դ
			const char* format_str(const D3DFORMAT fmt);			//��ȡFMT��ʽ�İ�
		private:
			std::string err_msg;			//������Ϣ

			HWND _hwnd;						//���ھ��
			int _width;						//��Ļ��
			int _height;					//��Ļ��
			bool _z_buffer;					//�Ƿ���z����

			UgeIDirect3D *_d3d;				//d3d����
			UgeDevice * _d3d_device;		//d3d�豸

			D3DXMATRIX _proj_matrix;		//ͶӰ����
			D3DXMATRIX _view_matrix;		//�ӿھ���

			UgeVertexBuffer* _vertex_buffer;//���㻺��
			UgeIndexBuffer* _index_buffer;	//��������
			ugeVertex* _vert_array;			//���㻺������

			
			UgeTexture* _texture1;			//����
			UgeTexture* _texture2;			//����
			UgeTexture* _texture3;			//����
			UgeTexture* _texture4;			//����
	};
}