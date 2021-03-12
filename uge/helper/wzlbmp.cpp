#include "wzlbmp.h"

namespace uge {

    WzlBmp::WzlBmp()
	{

	}

    //+-----------------------------------
    //| ����wzl
    //+-----------------------------------
    bool WzlBmp::loadWzl(const char* filename, int sort, byte** _dstBuffer, int* _dstSize, WzlBmpInfo* _wzlBmp)
    {
        // wzx·��
        static char wzx_path[MAX_PATH];
        strcpy_s(wzx_path, filename);
        strcat_s(wzx_path, ".wzx");

        // wzl·��
        static char wzl_path[MAX_PATH];
        strcpy_s(wzl_path, filename);
        strcat_s(wzl_path, ".wzl");

        //��ȡwzx
        void* ptr3 = nullptr;
        DWORD size = 0;

        if (!open_file(wzx_path, &ptr3, &size)) {
            return false;
        }
        byte* _btWzx = (byte*)ptr3;

        //��ȡͷ��
        WzxHeader wzx = {};
        memcpy_s(&wzx, size, _btWzx, sizeof(wzx));

        //��ȡ��һ��
        int offset = 0;
        memcpy_s(&offset, size, _btWzx + sizeof(wzx) + (sort * sizeof(int)), sizeof(offset));



        //��ȡwzl
        void* ptr4;
        //const char* path = "Data\\hum3.wzl";
        if (!open_file(wzl_path, &ptr4, &size)) {
            return false;
        }
        byte* _btWzl = (byte*)ptr4;

        //��ȡͷ��
        WzlHeader wzl = {};
        memcpy_s(&wzl, size, _btWzl, sizeof(wzl));

        //��ȡ��һ��
        WzlBmpInfo wzlBmp = {};
        memcpy_s(&wzlBmp, size, _btWzl + offset, sizeof(wzlBmp));

        //��ȡ����
        byte* srcBuffer = new byte[wzlBmp.size];
        memcpy_s(srcBuffer, size, _btWzl + offset + sizeof(wzlBmp), wzlBmp.size);



        //ͼƬʵ�ʴ�С (5 = 16λͼ D3DFMT_R5G6B5[2�����ݴ�С����])(3 = 32λͼ D3DFMT_A8R8G8B8)
        ULONG dstSize = wzlBmp.width * wzlBmp.height * (wzlBmp.pixelFormat == 5 ? 2 : 1);

        //��ѹ����
        byte* dstBuffer = new byte[dstSize];

        //���λͼ����[���ɫ���Ӧ��λ������] //����ǣ���909 ���������ݽ��н�ѹ���õ���λͼ����
        uncompress(dstBuffer, &dstSize, srcBuffer, wzlBmp.size);

        //���Դ���ݻ�����
        delete[] srcBuffer;
        free(ptr3);

        *_dstBuffer = dstBuffer;
        *_dstSize = dstSize;
        *_wzlBmp = wzlBmp;

        //��Ϣ
        char buf[255] = { 0 };
        sprintf_s(buf, "[bmp] %s %d [%d x %d] ���سɹ���\n", wzl_path, sort, wzlBmp.width, wzlBmp.height);
        Log(buf);

        return true;
    }

    //+-----------------------------------
    //| ͨ��wzl��������
    //+-----------------------------------
    bool WzlBmp::textureWzl(WzlBmpInfo wzlBmp, byte* dstBuffer, UgeTexture** _texture, UgeDevice* d3dDevice, bool has16To32)
    {
        //����
        D3DSURFACE_DESC _desc = {};//����
        HRESULT hr = S_OK;

        ///������ͼ
        D3DFORMAT fmt = D3DFMT_A8R8G8B8;
        if (wzlBmp.pixelFormat == 3)
        {
            fmt = D3DFMT_A8R8G8B8;
        }
        else if (wzlBmp.pixelFormat == 5)
        {
            fmt = D3DFMT_R5G6B5;
            if (has16To32)
            {
                fmt = D3DFMT_A8R8G8B8;
            }
        }

        d3dDevice->CreateTexture(wzlBmp.width, wzlBmp.height, 0, D3DUSAGE_DYNAMIC, fmt, D3DPOOL_DEFAULT, _texture, NULL);

        //��ȡ������Ϣ
        (*_texture)->GetLevelDesc(0, &_desc);

        ///��������
        D3DLOCKED_RECT lockRect;
        (*_texture)->LockRect(0, &lockRect, 0, 0);

        int sort = 0;
        int index = 0;
        UINT width = _desc.Width;
        UINT height = _desc.Height;

        DWORD* imageData3 = nullptr;
        SHORT* imageData5 = nullptr;
        if (wzlBmp.pixelFormat == 3)
        {
            imageData3 = (DWORD*)lockRect.pBits;
        }
        else if (wzlBmp.pixelFormat == 5)
        {
            imageData5 = (SHORT*)lockRect.pBits;
            if (has16To32)
            {
                imageData3 = (DWORD*)lockRect.pBits;
                if (width > height)
                {
                    width = width + 1;
                    height = height - 1;
                }
            }
        }

        //��ȡ��ɫ��
        //OpenRGB();

        for (UINT h = 0; h < height; h++)
        {
            for (UINT w = 0; w < width; w++)
            {
                if (wzlBmp.pixelFormat == 3) {
                    //���� ��һ�� ��ͼƬ�����һ�� ���ݴ������¶�ȡ
                    //D3DFMT_A8R8G8B8
                    sort = h * width + w;
                    BYTE  r = palette[dstBuffer[sort]].rgbRed;
                    BYTE  g = palette[dstBuffer[sort]].rgbGreen;
                    BYTE  b = palette[dstBuffer[sort]].rgbBlue;
                    DWORD color = D3DCOLOR_ARGB(0xff, r, g, b);
                    //index = (height - h) * width + w;

                    //ͼƬ���� ���ƴ�������
                    UINT index = (height - 1 - h) * lockRect.Pitch / 4 + w;
                    if (color != 0xff000000)
                    {
                        imageData3[index] = D3DCOLOR_ARGB(0xff, r, g, b);
                    }
                    else {
                        imageData3[index] = D3DCOLOR_ARGB(0, 0, 0, 0);
                    }
                }
                else if (wzlBmp.pixelFormat == 5)
                {
                    if (has16To32)
                    {
                        //D3DFMT_R5G6B5
                        sort = ((h * width) + w) * 2;
                        BYTE sh1 = dstBuffer[sort];
                        BYTE sh2 = dstBuffer[sort + 1];
                        USHORT sVal = (sh2 << 8) | sh1;

                        //D3DFMT_A8R8G8B
                        BYTE r = (sVal & 0xF800) >> 8;
                        BYTE g = (sVal & 0x07E0) >> 3;
                        BYTE b = (sVal & 0x001F) << 3;
                        DWORD color = D3DCOLOR_ARGB(0xFF, r, g, b);

                        //ͼƬ���� ���ƴ�������
                        UINT index = (height - h) * lockRect.Pitch / 4 + w;
                        if (color != 0xFF000000)
                        {
                            imageData3[index] = D3DCOLOR_ARGB(0xFF, r, g, b);
                        }
                        else {
                            imageData3[index] = D3DCOLOR_ARGB(0x88, 0, 0, 0);
                        }
                    }
                    else {
                        //D3DFMT_R5G6B5
                        sort = (h * width + w) * 2;
                        BYTE sh1 = dstBuffer[sort];
                        BYTE sh2 = dstBuffer[sort + 1];
                        USHORT sVal = (sh2 << 8) | sh1;

                        index = (height - 1 - h) * lockRect.Pitch / 2 + w;
                        imageData5[index] = sVal;
                    }

                }
            }
        }

        //����
        (*_texture)->UnlockRect(0);


        if (wzlBmp.pixelFormat == 5 && !has16To32) {

            //�½�һ��D3DFMT_A8R8G8B8
            IDirect3DSurface9* srcSurface;
            (*_texture)->GetSurfaceLevel(0, &srcSurface);

            LPDIRECT3DTEXTURE9 dstTexture = NULL;
            IDirect3DSurface9* dstSurface;
            hr = d3dDevice->CreateTexture(wzlBmp.width, wzlBmp.height, 0, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &dstTexture, NULL);
            dstTexture->GetSurfaceLevel(0, &dstSurface);

            D3DXLoadSurfaceFromSurface(
                dstSurface,
                NULL, NULL,
                srcSurface,
                NULL, NULL,
                D3DX_FILTER_LINEAR,
                D3DCOLOR_ARGB(0xFF, 0, 0, 0)
            );
            D3DSURFACE_DESC dstDesc;
            dstSurface->GetDesc(&dstDesc);

            //�ͷ�֮ǰ��
            (*_texture)->Release();
            (*_texture) = dstTexture;
        }

        return true;
    }
}

