#include "ugei.h"
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

    //+-----------------------------------
    //| ͨ��wzl��������
    //+-----------------------------------
    //UTEXTURE UGEI::CreateWzlTexture(wzl::WzlBmpInfo wzlBmp, byte* dstBuffer, bool has16To32)
    //{
    //    //��ȡ��������
    //    UgeTexture* p_tex;

    //    //����
    //    D3DSURFACE_DESC _desc = {};//����
    //    HRESULT hr = S_OK;

    //    ///������ͼ
    //    D3DFORMAT fmt = D3DFMT_A8R8G8B8;
    //    if (wzlBmp.pixelFormat == 3)
    //    {
    //        fmt = D3DFMT_A8R8G8B8;
    //    }
    //    else if (wzlBmp.pixelFormat == 5)
    //    {
    //        fmt = D3DFMT_R5G6B5;
    //        if (has16To32)
    //        {
    //            fmt = D3DFMT_A8R8G8B8;
    //        }
    //    }

    //    _d3d_device->CreateTexture(wzlBmp.width, wzlBmp.height, 0, D3DUSAGE_DYNAMIC, fmt, D3DPOOL_DEFAULT, &p_tex, NULL);

    //    //��ȡ������Ϣ
    //    p_tex->GetLevelDesc(0, &_desc);

    //    ///��������
    //    D3DLOCKED_RECT lockRect;
    //    p_tex->LockRect(0, &lockRect, 0, 0);

    //    int sort = 0;
    //    int index = 0;
    //    UINT width = _desc.Width;
    //    UINT height = _desc.Height;

    //    DWORD* imageData3 = nullptr;
    //    SHORT* imageData5 = nullptr;
    //    if (wzlBmp.pixelFormat == 3)
    //    {
    //        imageData3 = (DWORD*)lockRect.pBits;
    //    }
    //    else if (wzlBmp.pixelFormat == 5)
    //    {
    //        imageData5 = (SHORT*)lockRect.pBits;
    //        if (has16To32)
    //        {
    //            imageData3 = (DWORD*)lockRect.pBits;
    //            if (width > height)
    //            {
    //                //width = width + 1;
    //                //height = height - 1;
    //            }
    //        }

    //        if (width > height)
    //        {
    //            //width = width + 1;
    //            //height = height - 1;
    //        }
    //    }

    //    //��ȡ��ɫ��
    //    //OpenRGB();

    //    for (UINT h = 0; h < height; h++)
    //    {
    //        for (UINT w = 0; w < width; w++)
    //        {
    //            if (wzlBmp.pixelFormat == 3) {
    //                //���� ��һ�� ��ͼƬ�����һ�� ���ݴ������¶�ȡ
    //                //D3DFMT_A8R8G8B8
    //                sort = h * width + w;
    //                BYTE  r = wzl::palette[dstBuffer[sort]].rgbRed;
    //                BYTE  g = wzl::palette[dstBuffer[sort]].rgbGreen;
    //                BYTE  b = wzl::palette[dstBuffer[sort]].rgbBlue;
    //                DWORD color = D3DCOLOR_ARGB(0xff, r, g, b);
    //                //index = (height - h) * width + w;

    //                //ͼƬ���� ���ƴ�������
    //                UINT index = (height - 1 - h) * lockRect.Pitch / 4 + w;
    //                if (color != 0xff000000)
    //                {
    //                    imageData3[index] = D3DCOLOR_ARGB(0xff, r, g, b);
    //                }
    //                else {
    //                    imageData3[index] = D3DCOLOR_ARGB(0, 0, 0, 0);
    //                }
    //            }
    //            else if (wzlBmp.pixelFormat == 5)
    //            {
    //                if (has16To32)
    //                {
    //                    //D3DFMT_R5G6B5
    //                    sort = ((h * width) + w) * 2;
    //                    BYTE sh1 = dstBuffer[sort];
    //                    BYTE sh2 = dstBuffer[sort + 1];
    //                    USHORT sVal = (sh2 << 8) | sh1;

    //                    //D3DFMT_A8R8G8B
    //                    BYTE r = (sVal & 0xF800) >> 8;
    //                    BYTE g = (sVal & 0x07E0) >> 3;
    //                    BYTE b = (sVal & 0x001F) << 3;
    //                    DWORD color = D3DCOLOR_ARGB(0xFF, r, g, b);

    //                    //ͼƬ���� ���ƴ�������
    //                    UINT index = (height - h) * lockRect.Pitch / 4  + w;
    //                    if (color != 0xFF000000)
    //                    {
    //                        imageData3[index] = D3DCOLOR_ARGB(0xFF, r, g, b);
    //                    }
    //                    else {
    //                        imageData3[index] = D3DCOLOR_ARGB(0x88, 0, 0, 0);
    //                    }
    //                }
    //                else {
    //                    //D3DFMT_R5G6B5
    //                    sort = (h * width + w) * 2;
    //                    BYTE sh1 = dstBuffer[sort];
    //                    BYTE sh2 = dstBuffer[sort + 1];
    //                    USHORT sVal = (sh2 << 8) | sh1;

    //                    index = (height - 1 - h) * lockRect.Pitch / 2 + w;
    //                    imageData5[index] = sVal;
    //                }

    //            }
    //        }
    //    }

    //    //����
    //    p_tex->UnlockRect(0);


    //    if (wzlBmp.pixelFormat == 15 && !has16To32) {

    //        //�½�һ��D3DFMT_A8R8G8B8
    //        IDirect3DSurface9* srcSurface;
    //        p_tex->GetSurfaceLevel(0, &srcSurface);

    //        UgeTexture* dstTexture = nullptr;
    //        IDirect3DSurface9* dstSurface;
    //        hr = _d3d_device->CreateTexture(wzlBmp.width, wzlBmp.height, 0, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &dstTexture, NULL);
    //        dstTexture->GetSurfaceLevel(0, &dstSurface);

    //        D3DXLoadSurfaceFromSurface(
    //            dstSurface,
    //            NULL, NULL,
    //            srcSurface,
    //            NULL, NULL,
    //            D3DX_FILTER_LINEAR,
    //            D3DCOLOR_ARGB(0xFF, 0, 0, 0)
    //        );
    //        D3DSURFACE_DESC dstDesc;
    //        dstSurface->GetDesc(&dstDesc);

    //        //�ͷ�֮ǰ��
    //        p_tex->Release();
    //        p_tex = dstTexture;
    //    }

    //    return reinterpret_cast<UTEXTURE>(p_tex);
    //}

	bool UGEI::LoadTexture(const char* filename,bool bMipmap)
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

		return reinterpret_cast<UTEXTURE>(p_tex);
	}

 //   bool UGEI::LoadWzl(const char* path,int sort,ugeImage* image)
	//{
	//	// ����Դ��ȡ����
	//	wzl::Wzl* bmp = _wzl_cache->GetWzlCache(path);

	//	// �ӻ����ȡ����
	//	wzl::WzlTexture* wzlTex = bmp->GetTextureCache(sort);
	//	if (wzlTex == nullptr)
	//	{
	//		//��������
 //           wzl::WzlBmpInfo wzlBmpInfo = {};
	//		int dstSize = 0;

	//		//��ȡ������Դ
	//		byte* dstBuffer = bmp->GetBmp(sort, &wzlBmpInfo, &dstSize);
	//		if (dstBuffer == nullptr)
	//		{
	//			delete[] dstBuffer;
	//			return false;
	//		}

	//		//��������
 //           wzlTex = new wzl::WzlTexture();
 //           wzlTex->sort = sort;
 //           wzlTex->wzlBmpInfo = wzlBmpInfo;
 //           wzlTex->quote = 1;
 //           wzlTex->tex = CreateWzlTexture(wzlBmpInfo, dstBuffer);

	//		delete[] dstBuffer;

	//		//���滺��
 //           bmp->SetTextureCache(wzlTex);
	//	}

	//	// ��������
 //       strcpy_s(image->path,path);
 //       image->sort = sort;
 //       image->px = wzlTex->wzlBmpInfo.x;
 //       image->py = wzlTex->wzlBmpInfo.y;
 //       image->width = wzlTex->wzlBmpInfo.width;
 //       image->height = wzlTex->wzlBmpInfo.height;
 //       image->tex = wzlTex->tex;

 //       return true;
	//}

    //bool UGEI::LoadWzl(const char* path, int sort,int total, ugeAnimation* animation)
    //{
    //    // ����Դ��ȡ����
    //    wzl::Wzl* bmp = _wzl_cache->GetWzlCache(path);

    //    // �ӻ����ȡ����
    //    for (int i = 0; i < total; i++)
    //    {
    //        int t_sort = sort + i;
    //        wzl::WzlTexture* wzlTex = bmp->GetTextureCache(t_sort);
    //        if (wzlTex == nullptr)
    //        {
    //            //��������
    //            wzl::WzlBmpInfo wzlBmpInfo = {};
    //            int dstSize = 0;

    //            //��ȡ������Դ
    //            byte* dstBuffer = bmp->GetBmp(t_sort, &wzlBmpInfo, &dstSize);
    //            if (dstBuffer == nullptr)
    //            {
    //                delete[] dstBuffer;
    //                return false;
    //            }

    //            //��������
    //            wzlTex = new wzl::WzlTexture();
    //            wzlTex->sort = t_sort;
    //            wzlTex->wzlBmpInfo = wzlBmpInfo;
    //            wzlTex->quote = 1;
    //            wzlTex->tex = CreateWzlTexture(wzlBmpInfo, dstBuffer);

    //            delete[] dstBuffer;

    //            //���滺��
    //            bmp->SetTextureCache(wzlTex);
    //        }

    //        // ��������
    //        ugeImage* image = &(animation->image[i]);
    //        strcpy_s(image->path, path);
    //        image->sort = t_sort;
    //        image->px = wzlTex->wzlBmpInfo.x;
    //        image->py = wzlTex->wzlBmpInfo.y;
    //        image->width = wzlTex->wzlBmpInfo.width;
    //        image->height = wzlTex->wzlBmpInfo.height;
    //        image->tex = wzlTex->tex;
    //    }

    //    animation->total = total;
    //    animation->curFrame = 0;
    //    animation->time = 0;
    //    animation->rate = 8.0f / 60.0f;

    //    return true;
    //}

    //bool UGEI::ReleaseWzl(ugeImage* image) {
    //    if (_wzl_cache->ReleaseWzlCache(image->path, image->sort)) {

    //    }
    //    return true;
    //}

    //bool UGEI::ReleaseWzl(ugeAnimation* animation) {
    //    for (int i = 0; i < animation->total; i++)
    //    {
    //        if (_wzl_cache->ReleaseWzlCache(animation->image[i].path, animation->image[i].sort)) {

    //        }
    //    }
    //    return true;
    //}
}