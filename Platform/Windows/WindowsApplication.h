#include <windows.h>
#include <windowsx.h>
#include "BaseApplication.h"

namespace Corona
{
    class WindowsApplication : public BaseApplication
    {
    public:
        WindowsApplication(GfxConfiguration& config)
            : BaseApplication(config) {};

        virtual int Initialize();
        virtual void Finalize();
        // one cycle of the main loop
        virtual void Tick();

        inline HWND GetMainWindow() const { return m_hWnd; }
        
    private:
        static LRESULT CALLBACK WindowProc(HWND hWnd,
                                        UINT message,
                                        WPARAM wParam,
                                        LPARAM lParam);

    private:
        HWND m_hWnd;
    };
}