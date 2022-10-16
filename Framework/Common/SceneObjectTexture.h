#pragma once
#include "BaseSceneObject.h"
#include "geommath.h"
#include "JPEG.h"
#include "PNG.h"
#include "BMP.h"
#include "AssetLoader.h"

namespace Corona
{
    class SceneObjectTexture : public BaseSceneObject
    {
    protected:
        // m_Name here is the path of image (like "Scene/DamagedHelmet/DamagedHelmet_albedo.jpg")
        std::string m_Name;
        std::shared_ptr<Image> m_pImage;

    public:
        SceneObjectTexture() : BaseSceneObject(SceneObjectType::kSceneObjectTypeTexture) {};
        SceneObjectTexture(const std::string& name) : BaseSceneObject(SceneObjectType::kSceneObjectTypeTexture), m_Name(name) {};
        SceneObjectTexture(std::shared_ptr<Image>& image) : BaseSceneObject(SceneObjectType::kSceneObjectTypeTexture), m_pImage(image) {};
        SceneObjectTexture(std::shared_ptr<Image>&& image) : BaseSceneObject(SceneObjectType::kSceneObjectTypeTexture), m_pImage(std::move(image)) {};
        SceneObjectTexture(SceneObjectTexture&) = default;
        SceneObjectTexture(SceneObjectTexture&&) = default;
        void SetName(const std::string& name) { m_Name = name; };
        void SetName(std::string&& name) { m_Name = std::move(name); };
        const std::string& GetName() const { return m_Name; };
        void LoadTexture() 
        {
            if (!m_pImage)
            {
                // we should lookup if the texture has been loaded already to prevent
                // duplicated load. This could be done in Asset Loader Manager.
                Buffer buf = g_pAssetLoader->SyncOpenAndReadBinary(m_Name.c_str());
                std::string ext = m_Name.substr(m_Name.find_last_of("."));
                if (ext == ".jpg" || ext == ".jpeg")
                {
                    JpegParser jfif_parser;
                    m_pImage = std::make_shared<Image>(jfif_parser.Parse(buf));
                }
                else if (ext == ".png")
                {
                    PngParser png_parser;
                    m_pImage = std::make_shared<Image>(png_parser.Parse(buf));
                }
                else if (ext == ".bmp")
                {
                    BmpParser bmp_parser;
                    m_pImage = std::make_shared<Image>(bmp_parser.Parse(buf));
                }
            }
        }

        const Image& GetTextureImage() 
        { 
            if (!m_pImage)
            {
                LoadTexture();
            }

            return *m_pImage; 
        };

        friend std::ostream& operator<<(std::ostream& out, const SceneObjectTexture& obj);
    };
}