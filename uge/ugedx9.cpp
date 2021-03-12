#include "ugedx9.h"
#include "wzlbmp.h"

namespace uge {

    //+---------------------------
    //| ���캯��
    //+---------------------------
    UGEDX9::UGEDX9(HWND hwnd, int width, int height)
        :_hwnd(hwnd),
        _width(width),
        _height(height)
    {
        _z_buffer = false;                  //z_����

        _d3d = nullptr;                     //d3d����
        _d3d_device = nullptr;              //d3d�豸
        _vertex_buffer = nullptr;           //���㻺��
        _index_buffer = nullptr;            //��������
        _vert_array = nullptr;              //����ṹ����
        _texture1 = nullptr;                //����
        _texture2 = nullptr;                //����
        _texture3 = nullptr;                //����
        _texture4 = nullptr;                //����
    }

    //+-----------------------------------
    //| ��������
    //+-----------------------------------
    UGEDX9::~UGEDX9()
    {
        _d3d->Release();
        _d3d_device->Release();
        _vertex_buffer->Release();
        _index_buffer->Release();
        _texture1->Release();
        _texture2->Release();
        _texture3->Release();
        _texture4->Release();

        Log("DX9 ����\n");
    }

    //+-----------------------------------
    //| ��ȡ�����ʽ
    //+-----------------------------------
    const char* UGEDX9::format_str(const D3DFORMAT fmt)
    {
        int type = 0;
        switch (fmt) {
        case D3DFMT_R5G6B5:
            type = 1;
            break;
        case D3DFMT_X1R5G5B5:
            type = 2;
            break;
        case D3DFMT_A1R5G5B5:
            type = 3;
            break;
        case D3DFMT_X8R8G8B8:
            type = 4;
            break;
        case D3DFMT_A8R8G8B8:
            type = 5;
            break;
        default:
            type = 0;
            break;
        }

        static const char* szFormats[] = {
        "FMT_UNKNOWN", "FMT_R5G6B5", "FMT_X1R5G5B5", "FMT_A1R5G5B5", "FMT_X8R8G8B8", "FMT_A8R8G8B8"
        };

        return szFormats[type];
    }

    //+-----------------------------------
    //| ����ͶӰ����
    //+-----------------------------------
    void UGEDX9::SetProjectionMatrix() {
        D3DXMATRIX tmp;
        D3DXMatrixScaling(&_proj_matrix, 1.0f, -1.0f, 1.0f);
        D3DXMatrixTranslation(&tmp, -0.5f, (float)_height + 0.5f, 0.0f);
        D3DXMatrixMultiply(&_proj_matrix, &_proj_matrix, &tmp);
        D3DXMatrixOrthoOffCenterLH(&tmp, 0, static_cast<float>(_width), 0, static_cast<float>(_height), 0.0f, 1.0f);
        D3DXMatrixMultiply(&_proj_matrix, &_proj_matrix, &tmp);
    }

    //+---------------------------
    //| ��ʼ��D3D
    //+---------------------------
    bool UGEDX9::Initiate()
    {
        D3DADAPTER_IDENTIFIER9 ad_id;
        D3DDISPLAYMODE display_mode;
        D3DPRESENT_PARAMETERS d3dpp;

        //���� D3D
        _d3d = Direct3DCreate9(D3D_SDK_VERSION);
        if (_d3d == nullptr)
        {
            SetErrMsg("��ʼ��D3Dʧ��!");
            return false;
        }

        // ��ȡ��������Ϣ
        _d3d->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &ad_id);
        Log("D3D ����:%s",ad_id.Driver);
        Log("�Կ�����: %s",ad_id.Description);
        Log("�汾��: %d.%d.%d.%d",
            HIWORD(ad_id.DriverVersion.HighPart),
            LOWORD(ad_id.DriverVersion.HighPart),
            HIWORD(ad_id.DriverVersion.LowPart),
            LOWORD(ad_id.DriverVersion.LowPart));

        // ������ʾģʽ
        if (FAILED(_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode)) || display_mode.Format == D3DFMT_UNKNOWN)
        {
            SetErrMsg("�޷�ȷ��������Ƶģʽ");
            return false;
        }

        // ��ʾ����[����]
        ZeroMemory(&d3dpp, sizeof(d3dpp));
        d3dpp.BackBufferWidth = _width;
        d3dpp.BackBufferHeight = _height;
        d3dpp.BackBufferFormat = display_mode.Format;   //D3DFMT_X8R8G8B8 (32λ)
        d3dpp.BackBufferCount = 1;                      //�󱸻�����
        d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
        d3dpp.hDeviceWindow = _hwnd;
        d3dpp.Windowed = true;
        d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

        if (_z_buffer) {
            d3dpp.EnableAutoDepthStencil = TRUE;
            d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        }

        // ����D3D Device
        D3DCAPS9 caps;
        _d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
        uint32_t vp;
        if ((caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
            || !(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)) {
            Log("��ѡ��������㴦���豸");
            vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        }
        else {
            Log("��ѡ��Ӳ�����㴦���豸");
            vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
        }

        if (FAILED(_d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,_hwnd,vp,&d3dpp,&_d3d_device)))
        {
            SetErrMsg("�޷�����D3D device!");
            return false;
        }

        Log("ģʽ: %d * %d - %s", _width, _height, format_str(display_mode.Format));

        // ��ʼ���任����
        SetProjectionMatrix();
        D3DXMatrixIdentity(&_view_matrix);

        //���㻺��&��������
        if ( !InitResource())
        {
            return false;
        }

        // ����
        //Clear(ugeColor32::WHITE());

        return true;
    }

    //+-----------------------------------
    //| ��ʼ�����㻺�棬�������棬��Ⱦ״̬��
    //+-----------------------------------
    bool UGEDX9::InitResource()
    {
        // �������㻺�� [4������һ��������]
        if (FAILED(_d3d_device->CreateVertexBuffer(
            VERTEX_BUFFER_SIZE * sizeof(ugeVertex),
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            D3DFVF_UGEVERTEX,
            D3DPOOL_DEFAULT,
            &_vertex_buffer,
            nullptr
        )))
        {
            SetErrMsg("Can't create D3D vertex buffer");
            return false;
        }
        // ������ɫ��
        _d3d_device->SetVertexShader(nullptr);
        // ���������ʽ
        _d3d_device->SetFVF(D3DFVF_UGEVERTEX);
        // ������������Դ
        _d3d_device->SetStreamSource(0, _vertex_buffer, 0, sizeof(ugeVertex));


        // ������������[4������һ����������Ҫ6������]
        if (FAILED(_d3d_device->CreateIndexBuffer(
            VERTEX_BUFFER_SIZE / 4 * 6 * sizeof(uint16_t),
            D3DUSAGE_WRITEONLY,
            D3DFMT_INDEX16,
            D3DPOOL_DEFAULT,
            &_index_buffer,
            nullptr
        )))
        {
            SetErrMsg("Can't create D3D index buffer");
            return false;
        }

        uint16_t* pIndices, n = 0;
        if (FAILED(_index_buffer->Lock(0, 0, (VOID**)&pIndices, 0))) {
            SetErrMsg("Can't lock D3D index buffer");
            return false;
        }
        // 0 1  1 2
        // 3 2  0 3 
        for (int i = 0; i < VERTEX_BUFFER_SIZE / 4; i++) {
            *pIndices++ = n;        //0 4
            *pIndices++ = n + 1;    //1 5
            *pIndices++ = n + 2;    //2 6
            *pIndices++ = n + 2;    //2 6
            *pIndices++ = n + 3;    //3 7
            *pIndices++ = n;        //0 4
            n += 4;
        }

        _index_buffer->Unlock();
        _d3d_device->SetIndices(_index_buffer);

        // ������Ⱦ״̬
        _d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);          //�����޳� ����
        _d3d_device->SetRenderState(D3DRS_LIGHTING, FALSE);                 //�ƹ� �ر�

        _d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);          //��Alpha��Ⱦ
        _d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);     //Դ���Ҫ��
        _d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); //Ŀ����Ҫ��

        _d3d_device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);           //����Alpha����
        _d3d_device->SetRenderState(D3DRS_ALPHAREF, 0x01);                  //Alpha����ֵ
        _d3d_device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);  //���ڵ��ڳɹ�


        _d3d_device->SetTransform(D3DTS_VIEW, &_view_matrix);
        _d3d_device->SetTransform(D3DTS_PROJECTION, &_proj_matrix);


        //���ļ�
        void* ptr1;
        DWORD size;
        if ( ! open_file("image\\cat.jpg", &ptr1, &size)) {
            return false;
        }

        //��������
        if (FAILED(D3DXCreateTextureFromFileInMemory(_d3d_device, ptr1,size,&_texture1)))
        {
            SetErrMsg("��������ʧ��");
            return false;
        }

        D3DSURFACE_DESC desc;
        _texture1->GetLevelDesc(0, &desc);
        Log("��%d���ߣ�%d ��ʽ��%s", desc.Width, desc.Height, format_str(desc.Format));

        //����hum
        byte* dstBuffer = nullptr;
        int dstSize = 0;
        WzlBmpInfo wzlBmp = {};
        WzlBmp wbmp;
        
        if (!wbmp.loadWzl("Data\\hum", 0, &dstBuffer, &dstSize, &wzlBmp)) {
            return false;
        }

        // ��������
        if (!wbmp.textureWzl(wzlBmp, dstBuffer, &_texture2, _d3d_device)) {
            return false;
        }

        delete[] dstBuffer;

        //����hum
        dstBuffer = nullptr;
        dstSize = 0;
        wzlBmp = {};
        if (!wbmp.loadWzl("Data\\hum3", 7362, &dstBuffer, &dstSize, &wzlBmp)) {
            return false;
        }

        //save_bmp("11.bmp", wzlBmp.width, wzlBmp.height, dstSize, dstBuffer);

        // ��������
        if (!wbmp.textureWzl(wzlBmp, dstBuffer, &_texture3, _d3d_device)) {
            return false;
        }
        delete[] dstBuffer;
        
        //����hum4
        dstBuffer = nullptr;
        dstSize = 0;
        wzlBmp = {};
        if (!wbmp.loadWzl("Data\\hum3", 7368, &dstBuffer, &dstSize, &wzlBmp)) {
            return false;
        }

        //save_bmp("12.bmp", wzlBmp.width, wzlBmp.height, dstSize, dstBuffer);

        // ��������
        if (!wbmp.textureWzl(wzlBmp, dstBuffer, &_texture4, _d3d_device)) {
            return false;
        }
        delete[] dstBuffer;

        //-------------------------------------------
        //���ö���
        _vertex_buffer->Lock(0, 0, reinterpret_cast<void**>(&_vert_array), D3DLOCK_DISCARD);

        //����1
        static float x = 0.0f - 0.5f, y = 0.0f - 0.5f;
        _vert_array[0] = { 0.0f,200.0f,0.5f,0.0f,1.0f };
        _vert_array[1] = { 0.0f,0.0f,0.5f,0.0f,0.0f };
        _vert_array[2] = { 200.0f,0.0f,0.5f,1.0f,0.0f };
        _vert_array[3] = { 200.0f,200.0f,0.5f,1.0f,1.0f };


        //����2
        D3DSURFACE_DESC _desc2;
        _texture2->GetLevelDesc(0, &_desc2);

        static float ox = 100.0f - 0.5f, oy = 300.0f - 0.5f;
        static float hx = _desc2.Width + ox, hy = _desc2.Height + oy;
        _vert_array[4] = { ox ,hy   ,0.5f,0.0f,1.0f };
        _vert_array[5] = { ox ,oy   ,0.5f,0.0f,0.0f };
        _vert_array[6] = { hx ,oy   ,0.5f,1.0f,0.0f };
        _vert_array[7] = { hx ,hy   ,0.5f,1.0f,1.0f };

        //����3
        D3DSURFACE_DESC _desc3;
        _texture3->GetLevelDesc(0, &_desc3);

        static float ox3 = 300.0f - 0.5f, oy3 = 300.0f - 0.5f;
        static float hx3 = _desc3.Width + ox3, hy3 = _desc3.Height + oy3;
        _vert_array[8] = { ox3 ,hy3   ,0.5f,0.0f,1.0f };
        _vert_array[9] = { ox3 ,oy3   ,0.5f,0.0f,0.0f };
        _vert_array[10] = { hx3 ,oy3   ,0.5f,1.0f,0.0f };
        _vert_array[11] = { hx3 ,hy3   ,0.5f,1.0f,1.0f };

        //����3
        D3DSURFACE_DESC _desc4;
        _texture4->GetLevelDesc(0, &_desc4);

        static float ox4 = 500.0f - 0.5f, oy4 = 300.0f - 0.5f;
        static float hx4 = _desc4.Width + ox4, hy4 = _desc4.Height + oy4;
        _vert_array[12] = { ox4 ,hy4   ,0.5f,0.0f,1.0f };
        _vert_array[13] = { ox4 ,oy4   ,0.5f,0.0f,0.0f };
        _vert_array[14] = { hx4 ,oy4   ,0.5f,1.0f,0.0f };
        _vert_array[15] = { hx4 ,hy4   ,0.5f,1.0f,1.0f };

        _vertex_buffer->Unlock();

        return true;
    }

    //+-----------------------------------
    //| ��Ⱦ
    //+-----------------------------------
    void UGEDX9::Render()
    {
        //static int number = 7362;
        //static byte* dstBuffer = nullptr;
        //static int dstSize = 0;
        //static WzlBmpInfo wzlBmp = {};

        ////����hum
        //number += 1;
        //loadWzl("Data\\hum3", number, &dstBuffer, &dstSize, &wzlBmp);

        //// ��������
        //textureWzl(wzlBmp, dstBuffer, &_texture4);

        if (GetKeyState(VK_SPACE))
        {

        }

        //���������ζ���
        Clear(ugeColor32::GREY());
        _d3d_device->BeginScene();

        _d3d_device->SetTexture(0, _texture1);
        _d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

        _d3d_device->SetTexture(0, _texture2);
        _d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 6, 2);

        _d3d_device->SetTexture(0, _texture3);
        _d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 12, 2);

        _d3d_device->SetTexture(0, _texture4);
        _d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 18, 2);

        _d3d_device->EndScene();
        _d3d_device->Present(nullptr, nullptr, nullptr, nullptr);

    }

    //+-----------------------------------
    //| ���ô�����Ϣ
    //+-----------------------------------
    void UGEDX9::SetErrMsg(const char* error)
    {
	    err_msg = error;
    }

    //+----------------------------
    //| ���ô�����Ϣ
    //+----------------------------
    std::string UGEDX9::GetErrMsg()
    {
	    return err_msg;
    }

    //+----------------------------
    //| ���ô�����Ϣ
    //+----------------------------
    void UGEDX9::Clear(const ugeColor32 color)
    {
        if (_z_buffer) {
            _d3d_device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                color.argb, 1.0f, 0);
        }
        else {
            _d3d_device->Clear(0, nullptr, D3DCLEAR_TARGET, color.argb, 1.0f, 0);
        }
    }
}
