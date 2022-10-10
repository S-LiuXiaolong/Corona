#include "WindowsApplication.h"
#include <tchar.h>

using namespace Corona;

void WindowsApplication::CreateMainWindow()
{
    // get the HINSTANCE of the Console Program
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // the handle for the window, filled by a function
    HWND hWnd;
    // this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = _T("Corona Engine");

    // register the window class
    RegisterClassEx(&wc);

    // create the window and use the result as the handle
    hWnd = CreateWindowEx(0,
                        _T("Corona Engine"),              // name of the window class
                        m_Config.appName,                 // title of the window
                        WS_OVERLAPPEDWINDOW,              // window style
                        CW_USEDEFAULT,                    // x-position of the window
                        CW_USEDEFAULT,                    // y-position of the window
                        m_Config.screenWidth,             // width of the window
                        m_Config.screenHeight,            // height of the window
                        NULL,                             // we have no parent window, NULL
                        NULL,                             // we aren't using menus, NULL
                        hInstance,                        // application handle
                        this);                            // pass pointer to current object (if NULL, pThis in OnDraw will be nullptr)

    // display the window on the screen
    ShowWindow(hWnd, SW_SHOW);

    m_hWnd = hWnd;
}

int WindowsApplication::Initialize()
{
    int result;

    CreateMainWindow();

	// first call base class initialization
    result = BaseApplication::Initialize();

    if (result != 0)
        exit(result);

    return result;
}

void WindowsApplication::Finalize()
{
}

void WindowsApplication::Tick()
{
    BaseApplication::Tick();
    // this struct holds Windows event messages
    MSG msg;

    // we use PeekMessage instead of GetMessage here
    // because we should not block the thread at anywhere
    // except the engine execution driver module 
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        // translate keystroke messages into the right format
        TranslateMessage(&msg);

        // send the message to the WindowProc function
        DispatchMessage(&msg); 
    }
}

// this is the main message handler for the program
LRESULT CALLBACK WindowsApplication::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // ?
	WindowsApplication* pThis;
	if (message == WM_NCCREATE)
	{
		pThis = static_cast<WindowsApplication*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

		SetLastError(0);
		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
		{
			if (GetLastError() != 0)
				return FALSE;
		}
	}
	else
	{
		pThis = reinterpret_cast<WindowsApplication*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

    // sort through and find what code to run for the message given
    switch(message)
    {
        // pThis is nullptr and OnDraw make mistakes
	case WM_PAINT:
	    {
            pThis->OnDraw();
	    } 
        break;

    case WM_KEYUP:
        {
            switch(wParam)
            {
                case VK_LEFT:
                    g_pInputManager->LeftArrowKeyUp();
                    break;
                case VK_RIGHT:
                    g_pInputManager->RightArrowKeyUp();
                    break;
                case VK_UP:
                    g_pInputManager->UpArrowKeyUp();
                    break;
                case VK_DOWN:
                    g_pInputManager->DownArrowKeyUp();
                    break;
                default:
                    break;
            }
        } 
        break;
    case WM_KEYDOWN:
        {
            switch(wParam)
            {
                case VK_LEFT:
                    g_pInputManager->LeftArrowKeyDown();
                    break;
                case VK_RIGHT:
                    g_pInputManager->RightArrowKeyDown();
                    break;
                case VK_UP:
                    g_pInputManager->UpArrowKeyDown();
                    break;
                case VK_DOWN:
                    g_pInputManager->DownArrowKeyDown();
                    break;

                case 'A': // VK_A
                    g_pInputManager->AKeyDown();
                    break;
                case 'D': // VK_D
                    g_pInputManager->DKeyDown();
                    break;
                case 'W': // VK_W
                    g_pInputManager->WKeyDown();
                    break;
                case 'S': // VK_S
                    g_pInputManager->SKeyDown();
                    break;
				case 'Q': // VK_W
					g_pInputManager->QKeyDown();
					break;
				case 'E': // VK_S
					g_pInputManager->EKeyDown();
					break;

				case 'J': // VK_J
					g_pInputManager->JKeyDown();
					break;
				case 'L': // VK_L
					g_pInputManager->LKeyDown();
					break;
				case 'I': // VK_I
					g_pInputManager->IKeyDown();
					break;
				case 'K': // VK_K
					g_pInputManager->KKeyDown();
					break;
				default:
					break;
            }
        } 
        break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
        g_pInputManager->OnMouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;

	case WM_MOUSEMOVE:
        switch (wParam)
        {
        case MK_LBUTTON:
            g_pInputManager->OnMouseMoveL(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        case MK_RBUTTON:
            g_pInputManager->OnMouseMoveR(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        default:
            break;
        }
        break;

    case WM_MOUSEWHEEL:
        g_pInputManager->OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
        break;

        // this message is read when the window is closed
    case WM_DESTROY:
        {
            // close the application entirely
            PostQuitMessage(0);
            m_bQuit = true;
            return 0;
        }
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc (hWnd, message, wParam, lParam);
}
