#include <objbase.h>
#include "D2dGraphicsManager.h"
#include "WindowsApplication.h"
#include "Utility.h"

namespace Corona 
{
    extern IApplication* g_pApp;

    HRESULT D2dGraphicsManager::CreateGraphicsResources()
    {
        HRESULT hr = S_OK;

        HWND hWnd = reinterpret_cast<WindowsApplication*>(g_pApp)->GetMainWindow();

        if (m_pRenderTarget == nullptr)
        {
            RECT rc;
            GetClientRect(hWnd, &rc);

            D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left,
                            rc.bottom - rc.top);

            hr = m_pFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(hWnd, size),
                &m_pRenderTarget);
        }

        return hr;
    }

    int  D2dGraphicsManager::Initialize()
    {
        int result = 0;

        // initialize COM
        if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))) return -1;

        if (FAILED(D2D1CreateFactory(
                        D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory)))
                return -1;

        result = static_cast<int>(CreateGraphicsResources());

        return result;
    }

    void D2dGraphicsManager::Tick()
    {
    }

    void D2dGraphicsManager::Finalize()
    {
        SafeRelease(&m_pRenderTarget);
        SafeRelease(&m_pFactory);

        CoUninitialize();
    }
}
