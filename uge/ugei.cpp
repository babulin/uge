#include "ugei.h"

const char* WINDOW_CLASS_NAME = "UGE__WNDCLASS";

namespace uge {

	UGEI* pUGE = nullptr;

	//+------------------------
	//| ��������
	//+------------------------
	UGE* UGE_CALL ugeCreate(const int ver)
	{
		if (ver == UGE_VERSION)
		{
			return static_cast<UGE *>(UGEI::Create());
		}

		return nullptr;
	}

	//+------------------------
	//| ����ģʽ��ȡһ���������
	//+------------------------
	UGEI* UGEI::Create()
	{
		if (!pUGE)
		{
			pUGE = new UGEI();
		}

		return pUGE;
	}

	//+------------------------
	//| ���湹�캯��
	//+------------------------
	UGEI::UGEI()
	{
		//������Ϣ
		hwnd = nullptr;
		hwnd_parent = nullptr;
		h_instance = GetModuleHandle(nullptr);
		windowed = true;				//����ģʽ
		active = false;					//�Ƿ񱻼���
		win_title = "UGE";				//��������
		rect_windowed = {};				//���ھ���
		screen_width = 800;				//���ڿ�
		screen_height = 600;			//���ڸ�
		rect_windowed = {};				//���ھ���
		style_windowed = 0;				//��������

		//��Դ
		_uge_resource = new UgeResource();

		//��Ϸ����
		frame_func = nullptr;			//֡�ص�����
		update_func = nullptr;			//���»ص�
		splash_screen_enabled = true;	//��������
		game = nullptr;					//Ӧ�ó���

		//time
		_game_time_s = 0.0f;
		_pre_time_ms = 0;
		_fps_time_ms = 0;
		_delay_time_ms = 0;
		_delay_time_s = 0.0f;
		_fixed_delay_ms = 0;			//�̶���ʱ
		_run_fps = 0;
		_fps = 0;


		//dx
		_z_buffer = false;
		_d3d = nullptr;
		_d3d_device = nullptr;
		_proj_matrix = {};
		_view_matrix = {};
		_vertex_buffer = nullptr;
		_index_buffer = nullptr;
		_vert_array = nullptr;

		_n_prim = 0;
	}

	//+------------------------
	//| ������Ļģʽ
	//+------------------------
	void UGE_CALL UGEI::SetWindowed(bool value)
	{
		this->windowed = value;
	}

	//+------------------------
	//| ������Ļ����
	//+------------------------
	void UGE_CALL UGEI::SetScreen(int width, int height)
	{
		this->screen_width = width;
		this->screen_height = height;
	}

	//+------------------------
	//| ���ڱ���
	//+------------------------
	void UGE_CALL UGEI::SetTitle(const char* title)
	{
		this->win_title = title;
	}

	//+------------------------
	//| ��Ⱦ֡�ص�
	//+------------------------
	void UGEI::SetFrameCallback(const ugeCallback value)
	{
		this->frame_func = value;
	}

	//+------------------------
	//| ��Ⱦ֡�ص�
	//+------------------------
	void UGEI::SetUpdateCallback(const ugeCallback value)
	{
		this->update_func = value;
	}

	//+------------------------
	//| Ӧ�ó���
	//+------------------------
	void UGE_CALL UGEI::SetGame(Game* game)
	{
		this->game = game;
	}

	//+------------------------
	//| ��ʼ������
	//+------------------------
	bool UGE_CALL UGEI::Initiate()
	{
		if (!game && (!frame_func || !update_func))
		{
			_SetErrMsg("�����ʼ��:��ϷӦ�û�ص�֡����δ����");
			return false;
		}

		OSVERSIONINFO os_ver;
		SYSTEMTIME time;
		MEMORYSTATUS mem_st;
		WNDCLASS winclass = {};

		Log("+========[Your Game Engine]========+");
		Log("UGE �����ʼ��...");
		// ����汾
		Log("UGE �汾: %X.%X",UGE_VERSION >> 8,UGE_VERSION & 0xFF);
		// ��ȡϵͳʱ��
		GetLocalTime(&time);
		Log("����:%d/%02d/%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		// ��ȡϵͳ�汾
		os_ver.dwOSVersionInfoSize = sizeof(os_ver);
		GetVersionEx(&os_ver);
		Log("ϵͳ: Windows %ld.%ld.%ld", os_ver.dwMajorVersion, os_ver.dwMinorVersion, os_ver.dwBuildNumber);
		// ��ȡ�ڴ���Ϣ
		GlobalMemoryStatus(&mem_st);
		Log("�ڴ�: %ldK ��, %ldK ����", mem_st.dwTotalPhys / 1024L,mem_st.dwAvailPhys / 1024L);

		// ע�ᴰ����
		winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		winclass.lpfnWndProc = UGEI::WindowProc;
		winclass.cbClsExtra = 0;
		winclass.cbWndExtra = 0;
		winclass.hInstance = h_instance;
		winclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		winclass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		winclass.lpszMenuName = nullptr;
		winclass.lpszClassName = WINDOW_CLASS_NAME;

		if (!RegisterClass(&winclass)) {
			_SetErrMsg("�����ʼ��:ע�ᴰ����ʧ��!");
			return false;
		}

		if (screen_width == 0 || screen_height == 0)
		{
			_SetErrMsg("�����ʼ��:���ڵĿ��δ����\n");
			return false;
		}

		// ��ȡ�ͻ�����ʵ�ʴ�С
		const auto scr_width = GetSystemMetrics(SM_CXSCREEN);
		const auto scr_height = GetSystemMetrics(SM_CYSCREEN);
		rect_windowed.left = (scr_width - screen_width) / 2;
		rect_windowed.top = (scr_height - screen_height) / 2;
		rect_windowed.right = rect_windowed.left + screen_width;
		rect_windowed.bottom = rect_windowed.top + screen_height;

		style_windowed = WS_BORDER | WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

		AdjustWindowRect(&rect_windowed, style_windowed, false);

		if (windowed)
		{
			hwnd = CreateWindowEx(0, WINDOW_CLASS_NAME, win_title.c_str(), style_windowed,
				rect_windowed.left, rect_windowed.top,
				rect_windowed.right - rect_windowed.left,
				rect_windowed.bottom - rect_windowed.top,
				hwnd_parent, nullptr, h_instance, nullptr);
		}
		else {

		}

		if (!hwnd) {
			_SetErrMsg("�����ʼ��:��������ʧ��");
			return false;
		}

		// ��ʼ����ϵͳ
		// ��Ϸʱ��
		// �������
		// ͼ����Ⱦ
		if (! _DxInit())
		{
			return false;
		}
		// ��Ƶ����

		Log("��ʼ�����...");
		Log("+========[Your Game Engine]========+");
		ShowWindow(hwnd, SW_SHOW);

		// ʱ��
		_pre_time_ms = _fps_time_ms = timeGetTime();

		// ��ʾ��������
		if (splash_screen_enabled)
		{

		}

		return true;
	}

	//+------------------------
	//| ��������
	//+------------------------
	bool UGE_CALL UGEI::Start()
	{
		MSG msg;

		// ����״̬
		active = true;

		// ��ѭ��
		for (;;)
		{
			if (!hwnd_parent) {
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					if (msg.message == WM_QUIT) {
						break;
					}
					// TranslateMessage(&msg);
					DispatchMessage(&msg);
					continue;
				}
			}

			// �������

			if (active)
			{
				do {
					//��֡���1����
					_delay_time_ms = timeGetTime() - _pre_time_ms;
				} while (_delay_time_ms < 1);


				//�Ƿ���ڹ̶���ʱ
				if (_delay_time_ms >= _fixed_delay_ms)
				{
					// ʱ��
					_delay_time_s = _delay_time_ms / 1000.0f;

					// ʱ������0.2
					if (_fixed_delay_ms > 0) {
						_delay_time_s = _fixed_delay_ms / 1000.0f;
					}
					else {
						_delay_time_s = 0.001f;
					}

					// �ۼ�ʱ��
					_game_time_s += _delay_time_s;

					// ��ʱ��
					_pre_time_ms = timeGetTime();
					if (_pre_time_ms - _fps_time_ms <= 1000) {
						//����1s�ڵ�֡��
						_run_fps++;
					}
					else {
						//1���ۼƽ���
						_fps = _run_fps;
						_run_fps = 0;
						_fps_time_ms = _pre_time_ms;

						// ���ñ���
						static char title[255] = {};
						sprintf_s(title, "%s FPS: %d", win_title.c_str(), _fps);
						SetWindowText(hwnd, title);
					}

					//��ʼ
					_BeginScene();

					// ���»ص�����
					if (game)
					{
						if (!game->Update()) {
							break;
						}

						// �ص�֡����
						if (!game->Show()) {
							break;
						}
					}
					else {
						if (!update_func()) {
							break;
						}

						// �ص�֡����
						if (!frame_func()) {
							break;
						}
					}

					//������ת
					_EndScene();

					if (hwnd_parent) {
						break;
					}
				}
				else 
				{
					if (_fixed_delay_ms && _delay_time_ms + 3 < _fixed_delay_ms) {
						Sleep(1);
					}
				}
			}
			else 
			{
				Sleep(1);
			}
		}

		active = false;

		return true;
	}

	//+------------------------
	//| ϵͳ��Ϣ�ص�
	//+------------------------
    LRESULT CALLBACK UGEI::WindowProc(HWND hwnd, const UINT msg, WPARAM wparam, LPARAM lparam) {
        switch (msg) {
			case WM_CREATE:
				return FALSE;
			case WM_CLOSE:
				DestroyWindow(hwnd);
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				return FALSE;
			case WM_PAINT:
				break;
			default:
				break;
        }
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

	//+------------------------
	//| ������������
	//+------------------------
	UGEI::~UGEI()
	{
		if (_d3d)
			_d3d->Release();

		if (_d3d_device)
			_d3d_device->Release();

		if (_vertex_buffer)
			_vertex_buffer->Release();

		if (_index_buffer)
			_index_buffer->Release();

		if (_uge_resource)
		{
			delete _uge_resource;
			_uge_resource = nullptr;
		}

		Log("UGEI����");
	}

	//+------------------------
	//| �ͷ���Դ
	//+------------------------
	void UGE_CALL UGEI::Release()
	{
		if (pUGE)
		{
			delete pUGE;
			pUGE = nullptr;
		}

		Log("�ͷ����");
	}
}