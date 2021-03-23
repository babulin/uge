#include "wzlbmp.h"

namespace uge {

    WzlBmp::WzlBmp(const char* path)
	{
        //this->d3d_device = d3dDevice;
        this->path = path;
        wzxData = nullptr;
        wzxSize = 0;
        wzxHead = {};

        wzlData = nullptr;
        wzxSize = 0;
        wzlHead = {};

        _LoadWzx();
        _LoadWzl();
	}

    WzlBmp::~WzlBmp()
    {
        if (wzxData)
        {
            free(wzxData);
        }

        if (wzlData)
        {
            free(wzlData);
        }

        if (!_wzlTexMap.empty())
        {
            for (UWzlTexMap::iterator it = _wzlTexMap.begin(); it != _wzlTexMap.end(); it++)
            {
                delete it->second;
            }
        }

        Log("wzl %s:free", path);
    }

    int WzlBmp::_GetOffset(int sort)
    {
        //��ȡ��һ��
        int offset = 0;
        memcpy_s(&offset, wzxSize, reinterpret_cast<byte*>(wzxData) + sizeof(wzxHead) + (sort * sizeof(int)), sizeof(offset));
        if (offset == 48)
        {
            return 0;
        }
        return offset;
    }

    byte* WzlBmp::GetBmp(int sort,WzlBmpInfo* wzlBmp, int* dstSize)
    {
        //��ȡͼƬ���sort��ƫ��ֵoffset
        int offset = _GetOffset(sort);
        if (!offset)
        {
            return nullptr;
        }
        //��ȡ��һ��
        WzlBmpInfo* _wzlBmp = new WzlBmpInfo();
        memcpy_s(_wzlBmp, wzlSize, reinterpret_cast<byte*>(wzlData) + offset, sizeof(WzlBmpInfo));

        //��ȡ����
        byte* srcBuffer = new byte[_wzlBmp->size];
        memcpy_s(srcBuffer, wzlSize, reinterpret_cast<byte*>(wzlData) + offset + sizeof(WzlBmpInfo), _wzlBmp->size);

        //ͼƬʵ�ʴ�С (5 = 16λͼ D3DFMT_R5G6B5[2�����ݴ�С����])(3 = 32λͼ D3DFMT_A8R8G8B8)
        ULONG _dstSize = _wzlBmp->width * _wzlBmp->height * (_wzlBmp->pixelFormat == 5 ? 2 : 1);

        //��ѹ����
        byte* dstBuffer = new byte[_dstSize];

        //���λͼ����[���ɫ���Ӧ��λ������] //����ǣ���909 ���������ݽ��н�ѹ���õ���λͼ����
        uncompress(dstBuffer, &_dstSize, srcBuffer, _wzlBmp->size);

        //���Դ���ݻ�����
        delete[] srcBuffer;

        if (wzlBmp)
        {
            *dstSize = _dstSize;
            *wzlBmp = *_wzlBmp;
        }

        return dstBuffer;
    }

    WzlTexture* WzlBmp::GetTextureCache(int sort)
    {
        //��������
        UWzlTexMap::const_iterator curWzlTexMap = _wzlTexMap.find(sort);
        if (curWzlTexMap == _wzlTexMap.end())
        {
            return nullptr;
        }

        //���ô�����1
        _wzlTexMap.at(sort)->quote++;

        return curWzlTexMap->second;
    }

    bool WzlBmp::SetTextureCache(WzlTexture* tex)
    {
        _wzlTexMap.insert(std::pair<int, WzlTexture*>(tex->sort, tex));
        return true;
    }

    bool WzlBmp::ReleaseTexture(int sort, bool* hasErase)
    {
        UWzlTexMap::const_iterator t_CurTex = _wzlTexMap.find(sort);
        if (t_CurTex == _wzlTexMap.end())
        {
            return false;
        }

        //�ҵ�
        t_CurTex->second->quote--;
        if (t_CurTex->second->quote <= 0)
        {
            reinterpret_cast<UgeTexture*>(t_CurTex->second->tex)->Release();
            delete t_CurTex->second;
            _wzlTexMap.erase(sort);
        }

        //����Ƿ�������
        if (_wzlTexMap.empty())
        {
            delete this;
            *hasErase = true;
        }
        
        return true;
    }

    //+-----------------------------------
    //| ����wzx
    //+-----------------------------------
    bool WzlBmp::_LoadWzx()
    {
        // wzx·��
        static char wzx_path[MAX_PATH];
        strcpy_s(wzx_path, path);
        strcat_s(wzx_path, ".wzx");

        //��ȡwzx
        wzxData = open_file(wzx_path, &wzxSize);
        if (wzxData == nullptr) {
            return false;
        }

        //��ȡͷ��
        memcpy_s(&wzxHead, wzxSize, wzxData, sizeof(wzxHead));

        return false;
    }


    //+-----------------------------------
    //| ����wzl
    //+-----------------------------------
    bool WzlBmp::_LoadWzl()
    {
        // wzl·��
        static char wzl_path[MAX_PATH];
        strcpy_s(wzl_path, path);
        strcat_s(wzl_path, ".wzl");

        //��ȡwzl
        wzlData = open_file(wzl_path, &wzlSize);
        if (wzlData == nullptr) {
            return false;
        }

        //��ȡͷ��
        memcpy_s(&wzlHead, wzlSize, wzlData, sizeof(wzlHead));

        return true;
    }

}

