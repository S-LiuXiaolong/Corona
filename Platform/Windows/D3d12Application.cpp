#include "D3d12Application.h"
#include <tchar.h>

namespace Corona
{
    void D3d12Application::Tick()
    {
        WindowsApplication::Tick();
        g_pGraphicsManager->Clear();
        g_pGraphicsManager->Draw();
        
        // Present the back buffer to the screen since rendering is complete.
        HDC hdc = GetDC(m_hWnd);
        SwapBuffers(hdc);
        ReleaseDC(m_hWnd, hdc);
    }

}

