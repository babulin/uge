#include "ugei.h"
#include "wzlbmp.h"

namespace uge {

	bool UGEI::_DxInit()
	{
		D3DADAPTER_IDENTIFIER9 ad_id;
		D3DDISPLAYMODE display_mode;
		D3DPRESENT_PARAMETERS d3dpp;

		//���� D3D
		_d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if (_d3d == nullptr)
		{
			_SetErrMsg("��ʼ��D3Dʧ��!");
			return false;
		}

		// ��ȡ��������Ϣ
		_d3d->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &ad_id);
		Log("D3D ����:%s", ad_id.Driver);
		Log("�Կ�����: %s", ad_id.Description);
		Log("�汾��: %d.%d.%d.%d",
			HIWORD(ad_id.DriverVersion.HighPart),
			LOWORD(ad_id.DriverVersion.HighPart),
			HIWORD(ad_id.DriverVersion.LowPart),
			LOWORD(ad_id.DriverVersion.LowPart));

		// ������ʾģʽ
		if (FAILED(_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode)) || display_mode.Format == D3DFMT_UNKNOWN)
		{
			_SetErrMsg("�޷�ȷ��������Ƶģʽ");
			return false;
		}

		// ��ʾ����[����]
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.BackBufferWidth = screen_width;
		d3dpp.BackBufferHeight = screen_height;
		d3dpp.BackBufferFormat = display_mode.Format;   //D3DFMT_X8R8G8B8 (32λ)
		d3dpp.BackBufferCount = 1;                      //�󱸻�����
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.hDeviceWindow = hwnd;
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

		if (FAILED(_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vp, &d3dpp, &_d3d_device)))
		{
			_SetErrMsg("�޷�����D3D device!");
			return false;
		}

		Log("ģʽ: %d * %d - %s", screen_width, screen_height, _FormatStr(display_mode.Format));

		// ��ʼ���任����
		_SetProjectionMatrix();
		D3DXMatrixIdentity(&_view_matrix);

		//���㻺��&��������
		if (!_InitResource())
		{
			return false;
		}

		// ����
		//Clear(ugeColor32::WHITE());

		return true;
	}

	//+-----------------------------------
	//| ����ͶӰ����
	//+-----------------------------------
	void UGEI::_SetProjectionMatrix() {
		D3DXMATRIX tmp;
		D3DXMatrixScaling(&_proj_matrix, 1.0f, -1.0f, 1.0f);
		D3DXMatrixTranslation(&tmp, -0.5f, (float)screen_height + 0.5f, 0.0f);
		D3DXMatrixMultiply(&_proj_matrix, &_proj_matrix, &tmp);
		D3DXMatrixOrthoOffCenterLH(&tmp, 0, static_cast<float>(screen_width), 0, static_cast<float>(screen_height), 0.0f, 1.0f);
		D3DXMatrixMultiply(&_proj_matrix, &_proj_matrix, &tmp);
	}

    //+-----------------------------------
    //| ��ȡ�����ʽ
    //+-----------------------------------
    const char* UGEI::_FormatStr(const D3DFORMAT fmt)
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
    //| ��ʼ�����㻺�棬�������棬��Ⱦ״̬��
    //+-----------------------------------
    bool UGEI::_InitResource()
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
            _SetErrMsg("Can't create D3D vertex buffer");
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
            _SetErrMsg("Can't create D3D index buffer");
            return false;
        }

        uint16_t* pIndices, n = 0;
        if (FAILED(_index_buffer->Lock(0, 0, (VOID**)&pIndices, 0))) {
            _SetErrMsg("Can't lock D3D index buffer");
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

        return true;
    }

    void UGEI::DxRenderQuad(ugeQuad* quad)
    {
        //-------------------------------------------
        //���ö���
        _vertex_buffer->Lock(0, 0, reinterpret_cast<void**>(&_vert_array), D3DLOCK_DISCARD);
        
        _d3d_device->SetTexture(0, reinterpret_cast<UgeTexture*>(quad->texture->tex));

        quad->v[0].x = quad->x;
        quad->v[0].y = quad->y + quad->texture->height;
        quad->v[0].z = 0.5f;
        quad->v[0].tx = 0.0f;
        quad->v[0].ty = 1.0f;

        quad->v[1].x = quad->x;
        quad->v[1].y = quad->y;
        quad->v[1].z = 0.5f;
        quad->v[1].tx = 0.0f;
        quad->v[1].ty = 0.0f;

        quad->v[2].x = quad->x + quad->texture->width;
        quad->v[2].y = quad->y;
        quad->v[2].z = 0.5f;
        quad->v[2].tx = 1.0f;
        quad->v[2].ty = 0.0f;

        quad->v[3].x = quad->x + quad->texture->width;
        quad->v[3].y = quad->y + quad->texture->height;
        quad->v[3].z = 0.5f;
        quad->v[3].tx = 1.0f;
        quad->v[3].ty = 1.0f;

        memcpy(&_vert_array[_n_prim * UGEPRIM_QUADS], quad->v,sizeof(ugeVertex) * UGEPRIM_QUADS);

        _vertex_buffer->Unlock();

        _Render();
    }

    //+-----------------------------------
    //| ��Ⱦ
    //+-----------------------------------
    void UGEI::_Render()
    {
        _d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, _n_prim * 6, 2);
        _n_prim++;

        //�ඥ�㵥��������Ⱦ
        //_d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _n_prim << 2, 0, _n_prim << 1);

        //_d3d_device->SetTexture(0, _texture2);
        //_d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 6, 2);

        //_d3d_device->SetTexture(0, _texture3);
        //_d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 12, 2);

        //_d3d_device->SetTexture(0, _texture4);
        //_d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 18, 2);
    }

    void UGEI::_BeginScene()
    {
        //���������ζ���
        _Clear(ugeColor32::GREY());
        _d3d_device->BeginScene();
        _n_prim = 0;
    }

    void UGEI::_EndScene()
    {
        _d3d_device->EndScene();
        _d3d_device->Present(nullptr, nullptr, nullptr, nullptr);
    }

    //+----------------------------
    //| ���ô�����Ϣ
    //+----------------------------
    void UGEI::_Clear(const ugeColor32 color)
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