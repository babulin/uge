#include "ugei.h"
#include "wzlbmp.h"

namespace uge {

	extern UGEI* pUGE;

	//+------------------------
	//| ���ô�����Ϣ
	//+------------------------
	void UGEI::_SetErrMsg(const char* error)
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

	ugeTexture* UGEI::LoadTexture(const char* filename,bool bMipmap)
	{
		UgeTexture* p_tex;
		D3DXIMAGE_INFO info;

		int size;
		void* data = open_file(filename, &size);
		if (! data) {
			return NULL;
		}

		if (FAILED(D3DXCreateTextureFromFileInMemoryEx(_d3d_device, data, size,
			D3DX_DEFAULT, D3DX_DEFAULT,
			bMipmap ? 0 : 1, // Mip levels
			0, // Usage
			D3DFMT_A8R8G8B8, // Format
			D3DPOOL_MANAGED, // Memory pool
			D3DX_FILTER_NONE, // Filter
			D3DX_DEFAULT, // Mip filter
			0, // Color key
			&info, nullptr,
			&p_tex)))
		{
			free(data);
			_SetErrMsg("��������ʧ��");
			return NULL;
		}

		Log("����:%d x %d fmt��%s", info.Width, info.Height, _FormatStr(info.Format));

		//��������
		const auto tex_item = new ugeTexture();
		tex_item->tex = reinterpret_cast<UTEXTURE>(p_tex);
		tex_item->width = info.Width;
		tex_item->height = info.Height;

		return tex_item;
	}

	ugeTexture* UGEI::LoadWzl(const char* filename,int sort)
	{
		//����hum
		WzlBmp bmp(filename,_d3d_device);
		WzlBmpInfo wzlBmp;
		int dstSize = 0;
		byte* dstBuffer = bmp._GetBmp(sort, &wzlBmp, &dstSize);
		if (dstBuffer == nullptr)
		{
			return NULL;
		}

		//��������
		UTEXTURE u_tex = bmp.LoadTexture(wzlBmp, dstBuffer);

		delete[] dstBuffer;

		//��������
		const auto tex_item = new ugeTexture();
		tex_item->tex = u_tex;
		tex_item->width = wzlBmp.width;
		tex_item->height = wzlBmp.height;

		return tex_item;
	}
}