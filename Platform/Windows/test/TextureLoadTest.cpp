#include <iostream>
#include <tchar.h>
#include "WindowsApplication.h"
#include "D2d/D2dGraphicsManager.h"
#include "MemoryManager.h"
#include "AssetLoader.h"
#include "utility.h"
#include "BMP.h"

using namespace Corona;
using namespace std;

namespace Corona 
{
    class TestGraphicsManager : public D2dGraphicsManager
    {
        public:
            using D2dGraphicsManager::D2dGraphicsManager;
            void DrawBitmap(const Image& image);
        private:
            ID2D1Bitmap* m_pBitmap = nullptr;
    };

    class TestApplication : public WindowsApplication
    {
    public:
        using WindowsApplication::WindowsApplication;

        virtual int Initialize();

        virtual void OnDraw();

    private:
        Image m_Image;
    };
}

namespace Corona {
    GfxConfiguration config(8, 8, 8, 8, 32, 0, 0, 1920, 1080, _T("Texture Load Test (Windows)"));
	IApplication* g_pApp                = static_cast<IApplication*>(new TestApplication(config));
    GraphicsManager* g_pGraphicsManager = static_cast<GraphicsManager*>(new TestGraphicsManager);
    MemoryManager*   g_pMemoryManager   = static_cast<MemoryManager*>(new MemoryManager);
    AssetLoader*     g_pAssetLoader     = static_cast<AssetLoader*>(new AssetLoader);
    SceneManager*    g_pSceneManager    = static_cast<SceneManager*>(new SceneManager);
    InputManager*    g_pInputManager    = static_cast<InputManager*>(new InputManager);
}

int TestApplication::Initialize()
{
    int result;

    result = WindowsApplication::Initialize();

    if (result == 0) {
        Buffer buf;

        if (m_nArgC > 1) {
            buf = g_pAssetLoader->SyncOpenAndReadBinary(m_ppArgV[1]);
        } else {
            buf = g_pAssetLoader->SyncOpenAndReadBinary("Textures/test.jfif");
        }

		PngParser  png_parser;
		JfifParser jfif_parser;

        // m_Image = png_parser.Parse(buf);
        m_Image = jfif_parser.Parse(buf);
    }

    return result;
}

void TestApplication::OnDraw()
{
    dynamic_cast<TestGraphicsManager*>(g_pGraphicsManager)->DrawBitmap(m_Image);
}

void TestGraphicsManager::DrawBitmap(const Image& image)
{
	HRESULT hr;

    // start build GPU draw command
    m_pRenderTarget->BeginDraw();

    D2D1_BITMAP_PROPERTIES props;
    props.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
    props.dpiX = 72.0f;
    props.dpiY = 72.0f;
    SafeRelease(&m_pBitmap);
    hr = m_pRenderTarget->CreateBitmap(D2D1::SizeU(image.Width, image.Height), 
                                                    image.data, image.pitch, props, &m_pBitmap);

    D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
    D2D1_SIZE_F bmpSize = m_pBitmap->GetSize();

    D2D1_RECT_F source_rect = D2D1::RectF(
                     0,
                     0,
                     bmpSize.width,
                     bmpSize.height
                     );

    float aspect = bmpSize.width / bmpSize.height;
	float dest_height = rtSize.height;
	float dest_width = rtSize.height * aspect;

    D2D1_RECT_F dest_rect = D2D1::RectF(
                     0,
                     0,
                     dest_width,
                     dest_height 
                     );

    m_pRenderTarget->DrawBitmap(m_pBitmap, dest_rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, source_rect);

    // end GPU draw command building
    m_pRenderTarget->EndDraw();
}


