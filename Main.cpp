#include "header.h"
#pragma region globVar
static TCHAR szWindowClass[] = _T("Main");
static TCHAR szTitle[] = _T("Dota 2 Timing Tool");
static TCHAR szWindowClass2[] = _T("WorkMain");
HINSTANCE hInst;
HWND clntBtn;
HWND hostBtn;
HWND snglBtn;
HWND applyBtn;
HWND window1;
HWND window2;
HMENU hMenu;
RECT wr = {0, 0, 720, 790};    // set the size, but not the position
HWND zeroBoxText[10];
HWND zeroBoxTime[10];
HWND NUM[10];
HWND NUMclear[10];
HWND NUMactive[10];
HWND timeLeft[10];
HWND vol;
HWND speed;
HWND gender;
HANDLE hThrd[10];
DWORD ThrdID[10];
HGDIOBJ hFont;
HWND hTxtDest;
TCHAR opt[100];
HRESULT hr;
ISpVoice * pVoice = NULL;
bool allowed=true;
#define KEYUP(vkCode) ((GetAsyncKeyState(vkCode) & 0x8000) ? false : true)
#define KEYDOWN(vkCode) ((GetAsyncKeyState(vkCode) & 0x8000) ? 1 : 0)
#pragma endregion globVar
#pragma region functions
LRESULT CALLBACK WndProc1(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc2(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI Threads(LPVOID val);
INT_PTR CALLBACK About(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
INT_PTR CALLBACK FAQ(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
INT_PTR CALLBACK Options(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
typedef struct
{byte length;
TCHAR stdTime[4];
TCHAR text[130];
TCHAR speech[130];
}TextStruct;
class ThreadJob
{
public:
	~ThreadJob()
	{
	}
	VOID closeThread();
	VOID changeTimeLeft(short time)
	{
		TCHAR buff[4];
		_itow_s(time, buff, 10);
		SetWindowText(timeLeft[numberr], buff);
	}
	byte numberr;
};
class Text
{
	friend void changeBeginned(byte);
	friend void checkBeginned(byte);
	friend void loadOptions();
	friend class ThreadJob;
public:
	Text()
	{
		ZeroMemory(&retStruct.speech, 130);
		ZeroMemory(&retStruct.text, 130);
		ZeroMemory(&retStruct.stdTime, 4);
		ZeroMemory(&onOffbuf, 4);
		retStruct.length=0;
		beginned=false;
	}
	~Text()
	{
	}
	VOID init(byte cl)
	{
		number = cl;
	}
	VOID apply()
	{
		if(GetWindowTextLength(zeroBoxText[number])==0||GetWindowTextLength(zeroBoxTime[number])==0)
		{
			SetWindowText(zeroBoxText[number], L"");
			SetWindowText(zeroBoxTime[number], L"");
			applied=false;
		}
		else
		{
			if(beginned==false)
			{
				getMainText();
			}
			else
			{
				SetWindowText(zeroBoxText[number], retStruct.text);
				SetWindowText(zeroBoxTime[number], retStruct.stdTime);
			}
		}
	}
	VOID readText()
	{
		if(applied!=true)
			return;
		getText();
		setTrue();
		hr = pVoice->Speak(retStruct.speech, SPF_IS_XML|SPF_ASYNC, 0);
		ZeroMemory(&retStruct.speech, 130);
		if(beginned==false)
			beginThread();
	}
protected:
	VOID setTrue()
	{
		SetWindowText(zeroBoxText[number], retStruct.text);
		SetWindowText(zeroBoxTime[number], retStruct.stdTime);
	}
	VOID getMainText()
	{
		ZeroMemory(&retStruct.text, 130);
		ZeroMemory(&retStruct.stdTime, 4);
		retStruct.length=GetWindowTextLength(zeroBoxText[number]);
		TCHAR* buf=new TCHAR[retStruct.length+1];
		GetWindowText(zeroBoxText[number], buf, retStruct.length+1);
		_tcscat_s(retStruct.text, 130, buf);
		delete [] buf;
		GetWindowText(zeroBoxTime[number], retStruct.stdTime, 4);
		applied=true;
	}

	VOID getText()
	{
		TCHAR sleep[] = L"<silence msec=\"100\"/>";
		onOff();
		TCHAR seconds[] = L" seconds";
		_tcscat_s(retStruct.speech, 130, opt);
		_tcscat_s(retStruct.speech, 130, retStruct.text); 
		_tcscat_s(retStruct.speech, 130, sleep);
		_tcscat_s(retStruct.speech, 130, onOffbuf);
		_tcscat_s(retStruct.speech, 130, seconds);

	}
	VOID onOff()
	{
		ZeroMemory(&onOffbuf, 4);
		if(GetWindowTextLength(NUMactive[number])<11)
		{
			GetWindowText(timeLeft[number], onOffbuf, 4);
		}
		else
		{
			_tcscat_s(onOffbuf, 4, retStruct.stdTime); 
		}
	}
	VOID beginThread()
	{
		beginned=true;
		SetWindowText(timeLeft[number], onOffbuf);
		SetWindowText(NUMactive[number], L"Activated");
		byte *send = &number;
		hThrd[number] = CreateThread(NULL,0,Threads, send,0, &ThrdID[number]);
	}

	bool applied;
	byte number;
	bool beginned;
	TCHAR onOffbuf[4];
	TextStruct retStruct;
}TextO[9];
void loadOptions()
{
	ZeroMemory(&opt, 100);
	_tcscat_s(opt, 100, CReader::RDLL::ReadConfig());
	if(CompareStringEx(LOCALE_NAME_INVARIANT, SORT_STRINGSORT, opt, _tcslen(opt), L"", _tcslen(L""), NULL, NULL, 0)==CSTR_EQUAL)
	{
		CReader::RDLL::CreateConfig();
		loadOptions();
	}
}
VOID wrkWndw()
{
	WNDCLASSEX wcex2;
	ZeroMemory(&wcex2, sizeof(WNDCLASSEX));
	wcex2.cbSize = sizeof(WNDCLASSEX);
	wcex2.style          = CS_HREDRAW | CS_VREDRAW;
	wcex2.lpfnWndProc    = WndProc2;
	wcex2.hInstance      = hInst;
	wcex2.hIcon          = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	wcex2.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex2.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
	wcex2.lpszClassName  = szWindowClass2;
	wcex2.hIconSm        = LoadIcon(wcex2.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	if (!RegisterClassEx(&wcex2))
	{MessageBox(NULL,_T("Call to RegisterClassEx failed!"),_T("Bad news:"),NULL);
	}
	AdjustWindowRect(&wr, WS_OVERLAPPED|WS_SYSMENU, TRUE);
	window2 = CreateWindow(
		szWindowClass2,
		szTitle,
		WS_OVERLAPPED | WS_SYSMENU,
		(GetSystemMetrics(SM_CXSCREEN)-wr.right - wr.left)/2, (GetSystemMetrics(SM_CYSCREEN)-wr.bottom - wr.top)/2,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInst,
		NULL
		);
	if (!window2)
	{
		MessageBox(NULL,_T("Call to CreateWindow failed!"),_T("Bad news:"),
			NULL);
		SendMessage(window1, WM_DESTROY, 0, 0);
	}
	hMenu = CreateMenu();
	HMENU menu_help = CreatePopupMenu();
	HMENU menu_options = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)menu_help, L"&Help");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)menu_options, L"&Options");
	AppendMenu(menu_options, MF_STRING, IDR_MENU_OPTIONS, L"Voice Options");
	AppendMenu(menu_help, MF_STRING, IDR_MENU_FAQ, L"FAQ");
	AppendMenu(menu_help, MF_STRING, IDR_MENU_ABOUT, L"About");
	SetMenu(window2, hMenu);
	ShowWindow(window1,SW_HIDE);
	ShowWindow(window2,SW_SHOW);
	UpdateWindow(window2);
	DrawMenuBar(window2);
}
VOID ThreadJob::closeThread()
{
	byte length = GetWindowTextLength(zeroBoxText[numberr]);
	TCHAR* buf=new TCHAR[length+1];
	GetWindowText(zeroBoxText[numberr], buf, length+1);
	Beep(600, 800);
	hr = pVoice->Speak(buf, SPF_IS_XML|SPF_ASYNC, 0);
	SetWindowText(NUMactive[numberr], L"Deactivated");
	delete [] buf;
	TextO[numberr].beginned=false;
	CloseHandle(hThrd[numberr]);
	TerminateThread(hThrd[numberr],0);
}
void changeBeginned(byte v)
{
	TextO[v].beginned=false;
}
void checkBeginned(byte v)
{
	if(TextO[v].beginned==false)
	{
		SetWindowText(zeroBoxText[v], L"");
		SetWindowText(zeroBoxTime[v], L"");
		TextO[v].applied=false;
	}
	return;
}
VOID CALLBACK TimerCallBack(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
	allowed=true;
	KillTimer(window2, IDT_INPUT_TIMER);
}
void RegKeys()
{
	RegisterHotKey(NULL, 0, MOD_NOREPEAT, VK_NUMPAD0);
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, VK_NUMPAD1);
	RegisterHotKey(NULL, 2, MOD_NOREPEAT, VK_NUMPAD2);
	RegisterHotKey(NULL, 3, MOD_NOREPEAT, VK_NUMPAD3);
	RegisterHotKey(NULL, 4, MOD_NOREPEAT, VK_NUMPAD4);
	RegisterHotKey(NULL, 5, MOD_NOREPEAT, VK_NUMPAD5);
	RegisterHotKey(NULL, 6, MOD_NOREPEAT, VK_NUMPAD6);
	RegisterHotKey(NULL, 7, MOD_NOREPEAT, VK_NUMPAD7);
	RegisterHotKey(NULL, 8, MOD_NOREPEAT, VK_NUMPAD8);
	RegisterHotKey(NULL, 9, MOD_NOREPEAT, VK_NUMPAD9);
	RegisterHotKey(NULL, 10, MOD_NOREPEAT, VK_SUBTRACT);
}
#pragma endregion functions
int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	WNDCLASSEX wcex;

	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc1;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	if (!RegisterClassEx(&wcex))
	{MessageBox(NULL,_T("Call to RegisterClassEx failed!"),_T("Bad news:"),NULL);
	return 1;
	}

	hInst = hInstance;
	window1 = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPED | WS_SYSMENU,
		(GetSystemMetrics(SM_CXSCREEN)-550)/2, (GetSystemMetrics(SM_CYSCREEN)-200)/2,
		550, 200,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!window1)
	{
		MessageBox(NULL,_T("Call to CreateWindow failed!"),_T("Bad news:"),
			NULL);

		return 1;
	}
	ShowWindow(window1,nCmdShow);
	UpdateWindow(window1);
	RegKeys();
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(msg.message==WM_HOTKEY)
		{
			switch (msg.wParam)
			{
			case 0:
				SendMessage(window2, WM_COMMAND, ZERO_NUM, NULL);
				break;
			case 1:
				SendMessage(window2, WM_COMMAND, ZERO_NUM, NULL);
				break;
			case 2:
				SendMessage(window2, WM_COMMAND, ZERO_NUM, NULL);
				break;
			case 3:
				SendMessage(window2, WM_COMMAND, ZERO_NUM, NULL);
				break;
			case 4:
				SendMessage(window2, WM_COMMAND, ZERO_NUM, NULL);
				break;
			case 5:
				SendMessage(window2, WM_COMMAND, ZERO_NUM, NULL);
				break;
			case 6:
				SendMessage(window2, WM_COMMAND, ZERO_NUM, NULL);
				break;
			case 7:
				SendMessage(window2, WM_COMMAND, ZERO_NUM, NULL);
				break;
			case 8:
				SendMessage(window2, WM_COMMAND, ZERO_NUM, NULL);
				break;
			case 9:
				SendMessage(window2, WM_COMMAND, ZERO_NUM, NULL);
				break;
			case 10:
				GetMessage(&msg, NULL, 0, 0);
				if(msg.message==WM_HOTKEY)
				{
					switch (msg.wParam)
					{
					case 0:
						SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)0);
						break;
					case 1:
						SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)1);
						break;
					case 2:
						SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)2);
						break;
					case 3:
						SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)3);
						break;
					case 4:
						SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)4);
						break;
					case 5:
						SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)5);
						break;
					case 6:
						SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)6);
						break;
					case 7:
						SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)7);
						break;
					case 8:
						SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)8);
						break;
					case 9:
						SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)9);
						break;
					default:
						break;
					}
				}
				break;
			default:
				break;
			}
		}
	}
	return (int) msg.wParam;
}
LRESULT CALLBACK WndProc1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	SIZE sz;
	switch (message)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case SINGLE_BUTTON:
			{
				wrkWndw();
			}
			break;
		case HOST_BUTTON:
			{
				MessageBox(hWnd, L"Coming in next version", L"I`m sorry...", 0);
			}
			break;
		case CLIENT_BUTTON:
			{
				MessageBox(hWnd, L"Coming in next version", L"I`m sorry...", 0);
			}
			break;
		}
		break;
	case WM_CREATE:
		{
			loadOptions();
			hdc = BeginPaint(hWnd, &ps);
			GetTextExtentPoint32(hdc, _T("Choose Your Destiny:"), lstrlen(_T("Choose Your Destiny:")), &sz);
			EndPaint(hWnd, &ps);
			snglBtn = CreateWindowEx(NULL,L"BUTTON",L"Single",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,20,sz.cy+80,110,35,hWnd,(HMENU)SINGLE_BUTTON, NULL,NULL);
			hostBtn = CreateWindowEx(NULL,L"BUTTON",L"Host",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,212,sz.cy+80,110,35,hWnd,(HMENU)HOST_BUTTON, NULL,NULL);
			clntBtn = CreateWindowEx(NULL,L"BUTTON",L"Client",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,404,sz.cy+80,110,35,hWnd,(HMENU)CLIENT_BUTTON, NULL,NULL);
			hFont = CreateFont(48, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Times New Roman");
			hTxtDest = CreateWindowEx(NULL, L"STATIC", L"Choose Your Destiny:", WS_CHILD|WS_VISIBLE, 225-(sz.cx), 20, 500, 48, hWnd, NULL, NULL, NULL);
			SendMessage(hTxtDest,WM_SETFONT,(WPARAM)hFont,MAKELPARAM(FALSE,0));
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}
LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	SIZE sz, szDeact, szZero;
	HWND hFcsd=GetFocus();
	short wmID;

	switch(message)
	{
	case WM_CREATE:
		{
			hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
			CoInitialize(0);
			if(!SUCCEEDED(hr) )
				SendMessage(hWnd, WM_DESTROY, 0, 0);
#pragma region interface
			short y = 105;
			hdc = BeginPaint(hWnd, &ps);
			GetTextExtentPoint32(hdc, _T("Button"), lstrlen(_T("Button")), &sz);
			CreateWindowEx(NULL, L"STATIC", L"Button", ES_CENTER|WS_CHILD|WS_VISIBLE, (100-sz.cx)/2+10, 10, sz.cx, sz.cy, hWnd, NULL, NULL, NULL);
			GetTextExtentPoint32(hdc, _T("Text"), lstrlen(_T("Text")), &sz);
			CreateWindowEx(NULL, L"STATIC", L"Text", ES_CENTER|WS_CHILD|WS_VISIBLE, (200-sz.cx)/2+140, 10, sz.cx, sz.cy, hWnd, NULL, NULL, NULL);
			GetTextExtentPoint32(hdc, _T("Time\n(seconds)"), lstrlen(_T("Time\n(seconds)")), &sz);
			CreateWindowEx(NULL, L"STATIC", L"Time\n(seconds)", ES_CENTER|WS_CHILD|WS_VISIBLE, (50-sz.cx)/2+360, 5, sz.cx, sz.cy+sz.cy, hWnd, NULL, NULL, NULL);
			GetTextExtentPoint32(hdc, _T("Status"), lstrlen(_T("Status")), &sz);
			GetTextExtentPoint32(hdc, _T("Deactivated"), lstrlen(_T("Deactivated")), &szDeact);
			CreateWindowEx(NULL, L"STATIC", L"Status", ES_CENTER|WS_CHILD|WS_VISIBLE, (szDeact.cx-sz.cx)/2+430, 10, sz.cx, sz.cy, hWnd, NULL, NULL, NULL);
			GetTextExtentPoint32(hdc, _T("Time left"), lstrlen(_T("Time left")), &sz);
			CreateWindowEx(NULL, L"STATIC", L"Time left", ES_CENTER|WS_CHILD|WS_VISIBLE, 470+szDeact.cx, 10, sz.cx, sz.cy, hWnd, NULL, NULL, NULL);
			GetTextExtentPoint32(hdc, _T("000"), lstrlen(_T("000")), &szZero);
			EndPaint(hWnd, &ps);
			NUM[0]	= CreateWindowEx(NULL,L"BUTTON",L"0",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,10,40,100,35,hWnd,(HMENU)ZERO_NUM, NULL,NULL);
			NUM[1]	= CreateWindowEx(NULL,L"BUTTON",L"1",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,37,95,45,45,hWnd,(HMENU)ONE_NUM, NULL,NULL);
			NUM[2]	= CreateWindowEx(NULL,L"BUTTON",L"2",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,37,160,45,45,hWnd,(HMENU)TWO_NUM, NULL,NULL);
			NUM[3]	= CreateWindowEx(NULL,L"BUTTON",L"3",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,37,225,45,45,hWnd,(HMENU)THREE_NUM, NULL,NULL);
			NUM[4]	= CreateWindowEx(NULL,L"BUTTON",L"4",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,37,290,45,45,hWnd,(HMENU)FOUR_NUM, NULL,NULL);
			NUM[5]	= CreateWindowEx(NULL,L"BUTTON",L"5",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,37,355,45,45,hWnd,(HMENU)FIVE_NUM, NULL,NULL);
			NUM[6]	= CreateWindowEx(NULL,L"BUTTON",L"6",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,37,420,45,45,hWnd,(HMENU)SIX_NUM, NULL,NULL);
			NUM[7]	= CreateWindowEx(NULL,L"BUTTON",L"7",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,37,485,45,45,hWnd,(HMENU)SEVEN_NUM, NULL,NULL);
			NUM[8]	= CreateWindowEx(NULL,L"BUTTON",L"8",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,37,550,45,45,hWnd,(HMENU)EIGHT_NUM, NULL,NULL);
			NUM[9]	= CreateWindowEx(NULL,L"BUTTON",L"9",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,37,615,45,45,hWnd,(HMENU)NINE_NUM, NULL,NULL);
			NUMclear[0]	= CreateWindowEx(NULL,L"BUTTON",L"",BS_DEFPUSHBUTTON|BS_ICON|WS_CHILD|WS_VISIBLE,112,45,25,25,hWnd,(HMENU)ZERO_CLEAR, NULL,NULL);
			NUMclear[1]	= CreateWindowEx(NULL,L"BUTTON",L"",BS_DEFPUSHBUTTON|BS_ICON|WS_CHILD|WS_VISIBLE,112,105,25,25,hWnd,(HMENU)ONE_CLEAR, NULL,NULL);
			NUMclear[2]	= CreateWindowEx(NULL,L"BUTTON",L"",BS_DEFPUSHBUTTON|BS_ICON|WS_CHILD|WS_VISIBLE,112,170,25,25,hWnd,(HMENU)TWO_CLEAR, NULL,NULL);
			NUMclear[3]	= CreateWindowEx(NULL,L"BUTTON",L"",BS_DEFPUSHBUTTON|BS_ICON|WS_CHILD|WS_VISIBLE,112,235,25,25,hWnd,(HMENU)THREE_CLEAR, NULL,NULL);
			NUMclear[4]	= CreateWindowEx(NULL,L"BUTTON",L"",BS_DEFPUSHBUTTON|BS_ICON|WS_CHILD|WS_VISIBLE,112,300,25,25,hWnd,(HMENU)FOUR_CLEAR, NULL,NULL);
			NUMclear[5]	= CreateWindowEx(NULL,L"BUTTON",L"",BS_DEFPUSHBUTTON|BS_ICON|WS_CHILD|WS_VISIBLE,112,365,25,25,hWnd,(HMENU)FIVE_CLEAR, NULL,NULL);
			NUMclear[6]	= CreateWindowEx(NULL,L"BUTTON",L"",BS_DEFPUSHBUTTON|BS_ICON|WS_CHILD|WS_VISIBLE,112,430,25,25,hWnd,(HMENU)SIX_CLEAR, NULL,NULL);
			NUMclear[7]	= CreateWindowEx(NULL,L"BUTTON",L"",BS_DEFPUSHBUTTON|BS_ICON|WS_CHILD|WS_VISIBLE,112,495,25,25,hWnd,(HMENU)SEVEN_CLEAR, NULL,NULL);
			NUMclear[8]	= CreateWindowEx(NULL,L"BUTTON",L"",BS_DEFPUSHBUTTON|BS_ICON|WS_CHILD|WS_VISIBLE,112,560,25,25,hWnd,(HMENU)EIGHT_CLEAR, NULL,NULL);
			NUMclear[9]	= CreateWindowEx(NULL,L"BUTTON",L"",BS_DEFPUSHBUTTON|BS_ICON|WS_CHILD|WS_VISIBLE,112,625,25,25,hWnd,(HMENU)NINE_CLEAR, NULL,NULL);
			SendMessage(NUMclear[0], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP2)));
			SendMessage(NUMclear[1], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP2)));
			SendMessage(NUMclear[2], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP2)));
			SendMessage(NUMclear[3], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP2)));
			SendMessage(NUMclear[4], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP2)));
			SendMessage(NUMclear[5], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP2)));
			SendMessage(NUMclear[6], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP2)));
			SendMessage(NUMclear[7], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP2)));
			SendMessage(NUMclear[8], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP2)));
			SendMessage(NUMclear[9], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP2)));
			applyBtn= CreateWindow(L"BUTTON",L"Apply",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,594,700,100,35,hWnd,(HMENU)APPLY_BUTTON, NULL,NULL);
			CreateWindowEx(NULL,L"BUTTON",L"Stop",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,624,40,70,35,hWnd,(HMENU)ZERO_BOX_STOP, NULL,NULL);
			CreateWindowEx(NULL,L"BUTTON",L"Stop",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,624,100,70,35,hWnd,(HMENU)ONE_BOX_STOP, NULL,NULL);
			CreateWindowEx(NULL,L"BUTTON",L"Stop",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,624,165,70,35,hWnd,(HMENU)TWO_BOX_STOP, NULL,NULL);
			CreateWindowEx(NULL,L"BUTTON",L"Stop",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,624,230,70,35,hWnd,(HMENU)THREE_BOX_STOP, NULL,NULL);
			CreateWindowEx(NULL,L"BUTTON",L"Stop",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,624,295,70,35,hWnd,(HMENU)FOUR_BOX_STOP, NULL,NULL);
			CreateWindowEx(NULL,L"BUTTON",L"Stop",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,624,360,70,35,hWnd,(HMENU)FIVE_BOX_STOP, NULL,NULL);
			CreateWindowEx(NULL,L"BUTTON",L"Stop",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,624,425,70,35,hWnd,(HMENU)SIX_BOX_STOP, NULL,NULL);
			CreateWindowEx(NULL,L"BUTTON",L"Stop",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,624,490,70,35,hWnd,(HMENU)SEVEN_BOX_STOP, NULL,NULL);
			CreateWindowEx(NULL,L"BUTTON",L"Stop",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,624,555,70,35,hWnd,(HMENU)EIGHT_BOX_STOP, NULL,NULL);
			CreateWindowEx(NULL,L"BUTTON",L"Stop",BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE,624,620,70,35,hWnd,(HMENU)NINE_BOX_STOP, NULL,NULL);
			zeroBoxText[0]	= CreateWindowEx(WS_EX_CLIENTEDGE,L"EDIT",L"Roshan",ES_LEFT|WS_CHILD|WS_VISIBLE,140,45,200,25,hWnd,NULL, NULL,NULL);
			for(byte i=1; i!=10;i++)
			{
				zeroBoxText[i]	= CreateWindowEx(WS_EX_CLIENTEDGE,L"EDIT",L"",ES_LEFT|WS_CHILD|WS_VISIBLE,140,y,200,25,hWnd,NULL, NULL,NULL);
				zeroBoxTime[i]	= CreateWindowEx(WS_EX_CLIENTEDGE,L"EDIT",L"",ES_CENTER|ES_NUMBER|WS_CHILD|WS_VISIBLE,360,y,50,25,hWnd,NULL, NULL,NULL);
				NUMactive[i]	= CreateWindowEx(NULL, L"STATIC", L"Deactivated", ES_CENTER|WS_CHILD|WS_VISIBLE, 430, y+3, szDeact.cx, szDeact.cy, hWnd, NULL, NULL, NULL);
				timeLeft[i]		= CreateWindowEx(NULL, L"STATIC", L"0", ES_CENTER|WS_CHILD|WS_VISIBLE, (sz.cx-szZero.cx)/2+470+szDeact.cx, y+3, szZero.cx, szZero.cy, hWnd, NULL, NULL, NULL);
				SendMessage(zeroBoxTime[i], EM_SETLIMITTEXT, 3, 0); 
				y+=65;
			}
			zeroBoxTime[0]	= CreateWindowEx(WS_EX_CLIENTEDGE,L"EDIT",L"600",ES_CENTER|ES_NUMBER|WS_CHILD|WS_VISIBLE,360,45,50,25,hWnd,NULL, NULL,NULL);
			NUMactive[0]	= CreateWindowEx(NULL, L"STATIC", L"Deactivated", ES_CENTER|WS_CHILD|WS_VISIBLE, 430, 48, szDeact.cx, szDeact.cy, hWnd, NULL, NULL, NULL);
			timeLeft[0]		= CreateWindowEx(NULL, L"STATIC", L"0", ES_CENTER|WS_CHILD|WS_VISIBLE, (sz.cx-szZero.cx)/2+470+szDeact.cx, 48, szZero.cx, szZero.cy, hWnd, NULL, NULL, NULL);
			SendMessage(zeroBoxTime[0], EM_SETLIMITTEXT, 3, 0); 
#pragma endregion interface
			for(byte i=0; i!=10; i++)
			{
				TextO[i].init(i);
			}
			TextO[0].apply();
		}
		break;
	case WM_STOP_TIMER_MSG:
		{
			DWORD dwExitCode = 0;
			if( GetExitCodeThread(hThrd[(byte)lParam], &dwExitCode))
			{
				TerminateThread(hThrd[(byte)lParam],0);
				CloseHandle(hThrd[(byte)lParam]);
				SetWindowText(NUMactive[(byte)lParam], L"Deactivated");
				SetWindowText(timeLeft[(byte)lParam], L"0");
				changeBeginned((byte)lParam);
				TextStruct tstr;
				ZeroMemory(tstr.speech, 120);
				tstr.length = GetWindowTextLength(zeroBoxText[(byte)lParam]);
				TCHAR* buf=new TCHAR[tstr.length+1];
				TCHAR text[] = L" stopped";
				GetWindowText(zeroBoxText[(byte)lParam], buf, tstr.length+1);
				_tcscat_s(tstr.speech, 120, buf);
				_tcscat_s(tstr.speech, 120, text);
				hr = pVoice->Speak(tstr.speech, SPF_IS_XML|SPF_ASYNC, 0);
				delete [] buf;
			}
		}
		break;
	case WM_COMMAND:
		wmID = LOWORD(wParam);
		switch (wmID)
		{
		case IDR_MENU_FAQ:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, FAQ);
			break;
		case IDR_MENU_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), 0, About);
			break;
		case IDR_MENU_OPTIONS:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), 0, Options);
			break;
		default:
			break;
		}
		switch (wParam)
		{
		case ZERO_BOX_STOP:
			SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)0);
			break;
		case ONE_BOX_STOP:
			SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)1);
			break;
		case TWO_BOX_STOP:
			SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)2);
			break;
		case THREE_BOX_STOP:
			SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)3);
			break;
		case FOUR_BOX_STOP:
			SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)4);
			break;
		case FIVE_BOX_STOP:
			SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)5);
			break;
		case SIX_BOX_STOP:
			SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)6);
			break;
		case SEVEN_BOX_STOP:
			SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)7);
			break;
		case EIGHT_BOX_STOP:
			SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)8);
			break;
		case NINE_BOX_STOP:
			SendMessage(window2, WM_STOP_TIMER_MSG, 0,(LPARAM)9);
			break;
		case APPLY_BUTTON:
			for(byte i = 0; i!=10; i++)
			{
				TextO[i].apply();
			}
			break;
		case ZERO_CLEAR:
			checkBeginned(0);
			break;
		case ONE_CLEAR:
			checkBeginned(1);
			break;
		case TWO_CLEAR:
			checkBeginned(2);
			break;
		case THREE_CLEAR:
			checkBeginned(3);
			break;
		case FOUR_CLEAR:
			checkBeginned(4);
			break;
		case FIVE_CLEAR:
			checkBeginned(5);
			break;
		case SIX_CLEAR:
			checkBeginned(6);
			break;
		case SEVEN_CLEAR:
			checkBeginned(7);
			break;
		case EIGHT_CLEAR:
			checkBeginned(8);
			break;
		case NINE_CLEAR:
			checkBeginned(9);
			break;
		default:
			break;
		}
		for(byte i = 0; i!= 10; i++)
		{
			if(hFcsd==zeroBoxText[i])
				return 1;
			if(hFcsd==zeroBoxTime[i])
				return 1;
		}
		if(allowed==true)
		{
			allowed=false;
			switch(wParam)
			{
			case ZERO_NUM:
				SetTimer(hWnd, IDT_INPUT_TIMER, 3000, (TIMERPROC) TimerCallBack);
				TextO[0].readText();
				break;
			case ONE_NUM:
				SetTimer(hWnd, IDT_INPUT_TIMER, 3000, (TIMERPROC) TimerCallBack);
				TextO[1].readText();
				break;
			case TWO_NUM:
				SetTimer(hWnd, IDT_INPUT_TIMER, 3000, (TIMERPROC) TimerCallBack);
				TextO[2].readText();
				break;
			case THREE_NUM:
				SetTimer(hWnd, IDT_INPUT_TIMER, 3000, (TIMERPROC) TimerCallBack);
				TextO[3] .readText();
				break;
			case FOUR_NUM:
				SetTimer(hWnd, IDT_INPUT_TIMER, 3000, (TIMERPROC) TimerCallBack);
				TextO[4].readText();
				break;
			case FIVE_NUM:
				SetTimer(hWnd, IDT_INPUT_TIMER, 3000, (TIMERPROC) TimerCallBack);
				TextO[5] .readText();
				break;
			case SIX_NUM:
				SetTimer(hWnd, IDT_INPUT_TIMER, 3000, (TIMERPROC) TimerCallBack);
				TextO[6].readText();
				break;
			case SEVEN_NUM:
				SetTimer(hWnd, IDT_INPUT_TIMER, 3000, (TIMERPROC) TimerCallBack);
				TextO[7] .readText();
				break;
			case EIGHT_NUM:
				SetTimer(hWnd, IDT_INPUT_TIMER, 3000, (TIMERPROC) TimerCallBack);
				TextO[8].readText();
				break;
			case NINE_NUM:
				SetTimer(hWnd, IDT_INPUT_TIMER, 3000, (TIMERPROC) TimerCallBack);
				TextO[9] .readText();
				break;
			default:
				allowed=true;
				break;
			}
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, IDT_INPUT_TIMER);
		pVoice->Release();
		pVoice = NULL;
		CoUninitialize();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}
DWORD WINAPI Threads(LPVOID val)
{
	byte vval = *static_cast<byte*>(val);
	ThreadJob ThreadJobO;
	ThreadJobO.numberr=vval;
	TCHAR time[4];
	GetWindowText(timeLeft[vval], time, 4);
	short count = _wtoi(time);
	for(short i = count-1; i != -1; i--)
	{
		Sleep(1000);
		ThreadJobO.changeTimeLeft(i);
	}
	ThreadJobO.closeThread();
	return 0;
}
INT_PTR CALLBACK FAQ(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hwndDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_SYSCOMMAND:
		if(wParam==SC_CLOSE)
		{
			EndDialog(hwndDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK About(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (uMsg)
	{
	case WM_INITDIALOG:{
		HFONT hFont = CreateFont(12, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, 0x1, 
	  OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
	  DEFAULT_PITCH | FF_DONTCARE, TEXT("MS Shell Dlg"));
		TCHAR dVer [4];
		ZeroMemory(&dVer, 4);
		_tcscat_s(dVer,4, CReader::RDLL::returnVersion());
		TCHAR dVerT [17];
		_tcscat_s(dVerT, 17, L"DLL version ");
		_tcscat_s(dVerT, 17, dVer);
		HWND DLLver = CreateWindowEx(NULL, L"STATIC", dVerT, ES_LEFT|WS_CHILD|WS_VISIBLE, 94,45,80,14, hwndDlg, NULL, NULL, NULL);
		SendMessage(DLLver, WM_SETFONT, (WPARAM)hFont, TRUE);

		return (INT_PTR)TRUE;
					   }
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hwndDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_SYSCOMMAND:
		if(wParam==SC_CLOSE)
		{
			EndDialog(hwndDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Options(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	const TCHAR *speedComboBoxItems[] = {L"-2", L"-1", L"0", L"+1", L"+2"};
	const TCHAR *genderComboBoxItems[] = {L"Male", L"Female"};
	UNREFERENCED_PARAMETER(lParam);
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			speed = CreateWindow(L"COMBOBOX", NULL,WS_CHILD | WS_VISIBLE | WS_TABSTOP|CBS_DROPDOWNLIST,100, 8, 136, 16,hwndDlg, NULL,hInst,NULL);
			gender = CreateWindow(L"COMBOBOX", NULL,WS_CHILD | WS_VISIBLE | WS_TABSTOP|CBS_DROPDOWNLIST,100, 38, 136, 16,hwndDlg, NULL,hInst,NULL);
			SendMessage(speed,
				CB_ADDSTRING,
				0,
				reinterpret_cast<LPARAM>((LPCWSTR)speedComboBoxItems[0]));
			SendMessage(speed,
				CB_ADDSTRING,
				0,
				reinterpret_cast<LPARAM>((LPCWSTR)speedComboBoxItems[1]));
			SendMessage(speed,
				CB_ADDSTRING,
				0,
				reinterpret_cast<LPARAM>((LPCWSTR)speedComboBoxItems[2]));
			SendMessage(speed,
				CB_ADDSTRING,
				0,
				reinterpret_cast<LPARAM>((LPCWSTR)speedComboBoxItems[3]));
			SendMessage(speed,
				CB_ADDSTRING,
				0,
				reinterpret_cast<LPARAM>((LPCWSTR)speedComboBoxItems[4]));
			SendMessage(gender,
				CB_ADDSTRING,
				0,
				reinterpret_cast<LPARAM>((LPCWSTR)genderComboBoxItems[0]));
			SendMessage(gender,
				CB_ADDSTRING,
				0,
				reinterpret_cast<LPARAM>((LPCWSTR)genderComboBoxItems[1]));
			TCHAR cBuf[50];
			ZeroMemory(&cBuf, 50);
			_tcscat_s(cBuf, 50, CReader::RDLL::ReadConfigForWindow());
			if(CompareStringEx(LOCALE_NAME_INVARIANT, SORT_STRINGSORT, cBuf, _tcslen(cBuf), L"", _tcslen(L""), NULL, NULL, 0)==CSTR_EQUAL)
			{
				CReader::RDLL::CreateConfig();
				ZeroMemory(&cBuf, 50);
				_tcscat_s(cBuf, 50, CReader::RDLL::ReadConfigForWindow());
			}
			byte i = 0;
			byte entry=0;
			byte count=0;
			byte v;
			TCHAR tBufvol[30];
			ZeroMemory(&tBufvol, 30);
			while(i!=_tcslen(cBuf))
			{
				if(cBuf[i]=='|')
				{
					++count;
					_tcsncat_s(tBufvol, 30, &cBuf[entry], i-entry);
					entry=i+1;
					switch (count)
					{
					case 1:
						v = _wtoi(tBufvol);
						v /= 10;
						break;
					case 2:
						{
							TCHAR bf[3];
							ZeroMemory(&bf,3);
							_tcscat_s(bf,3,tBufvol);
							int x = _wtoi(tBufvol);
							switch (x)
							{
							case -2:
								SendMessage(speed, CB_SETCURSEL, 0, 0);
								break;
							case -1:
								SendMessage(speed, CB_SETCURSEL, 1, 0);
								break;
							case 0:
								SendMessage(speed, CB_SETCURSEL, 2, 0);
								break;
							case 1:
								SendMessage(speed, CB_SETCURSEL, 3, 0);
								break;
							case 2:
								SendMessage(speed, CB_SETCURSEL, 4, 0);
								break;
							default:
								break;
							}
						}
						break;
					case 3:
						if(CompareStringEx(LOCALE_NAME_INVARIANT, SORT_STRINGSORT, tBufvol, _tcslen(tBufvol), L"Gender=Female;Age=Adult", _tcslen(L"Gender=Female;Age=Adult"), NULL, NULL, 0)==CSTR_EQUAL)
						{
							SendMessage(gender, CB_SETCURSEL, 1, 0);
						}else{SendMessage(gender, CB_SETCURSEL, 0, 0);
						}
						break;
					default:
						break;
					}
					ZeroMemory(&tBufvol, 30);
				}
				++i;
			}


			vol = CreateWindowEx(0, TRACKBAR_CLASS, L"", WS_CHILD|WS_VISIBLE|TBS_HORZ|TBS_BOTH|TBS_AUTOTICKS, 100, 68, 200, 42,hwndDlg, (HMENU)VOL_BAR, hInst, NULL);
			SendMessage(vol, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(1, 10));
			SendMessage(vol, TBM_SETPAGESIZE,0, (LPARAM) 1);
			SendMessage(vol, TBM_SETPOS,(WPARAM) TRUE,(LPARAM) v); 

			return (INT_PTR)TRUE;
		}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			TCHAR tBuf[100];
			ZeroMemory(&tBuf, 100);
			TCHAR tvol[4];
			int ivol = SendMessage(vol, TBM_GETPOS,NULL,NULL);
			ivol*=10;
			_itow_s(ivol, tvol, 10);
			TCHAR tgender[7];
			GetWindowText(gender, tgender, 7);
			TCHAR tspeed[3];
			GetWindowText(speed, tspeed, 3);
			swprintf_s(tBuf, L"<volume level=\"%ls\"/>\r\n<rate speed=\"%ls\"/>\r\n<voice required=\"Gender=%ls;Age=Adult\"/>", tvol, tspeed, tgender);
			CReader::RDLL::ChangeConfig(tBuf, _tcslen(tBuf));
			loadOptions();
			EndDialog(hwndDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hwndDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_SYSCOMMAND:
		if(wParam==SC_CLOSE)
		{
			EndDialog(hwndDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
