#pragma once
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <queue>
#include <string>

#include "ImageParser.h"
#include "portable.h"
#include "zlib/zlib.h"
#include "lodepng/lodepng.h"

namespace Corona {

    class PngParser : implements ImageParser 
    {
    protected:
        uint16_t m_Width;
        uint16_t m_Height;
        uint8_t m_BitDepth;
        uint8_t m_ColorType;
        uint8_t m_CompressionMethod;
        uint8_t m_FilterMethod;
        uint8_t m_InterlaceMethod;
        int32_t m_ScanLineSize;
        uint8_t m_BytesPerPixel;

    public:
        Image Parse(Buffer& buf) override 
        {
            Image m_Img;

            const unsigned char* pData = buf.GetData();
            size_t size = (size_t)buf.GetDataSize();

            std::vector<unsigned char> image;
            unsigned int width, height;
            lodepng::State state; //optionally customize this one
            unsigned int error = lodepng::decode(image, width, height, state, pData, size);

            bool imageDataStarted = false;
            bool imageDataEnded = false;
            uint8_t* imageDataStartPos = nullptr;
            uint8_t* imageDataEndPos = nullptr;

            m_Width = width;
            m_Height = height;
            m_BitDepth = state.info_raw.bitdepth;
            m_ColorType = state.info_raw.colortype;
            m_CompressionMethod = state.info_png.compression_method;
            m_FilterMethod = state.info_png.filter_method;
            m_InterlaceMethod = state.info_png.interlace_method;

            switch (m_ColorType) {
                case 0:  // grayscale
                    m_BytesPerPixel = (m_BitDepth + 7) >> 3;
                    switch (m_BytesPerPixel) {
                        case 1:
                            m_Img.pixel_format = PIXEL_FORMAT::R8;
                            break;
                        case 2:
                            m_Img.pixel_format = PIXEL_FORMAT::R16;
                            break;
                        case 4:
                            m_Img.pixel_format = PIXEL_FORMAT::R32;
                            break;
                        default:
                            assert(0);
                    }
                    break;
                case 2:  // rgb true color
                    m_BytesPerPixel = (m_BitDepth * 3 + 7) >> 3;
                    switch (m_BytesPerPixel) {
                        case 3:
                            m_Img.pixel_format = PIXEL_FORMAT::RGB8;
                            break;
                        case 6:
                            m_Img.pixel_format = PIXEL_FORMAT::RGB16;
                            break;
                        case 12:
                            m_Img.pixel_format = PIXEL_FORMAT::RGB32;
                            break;
                        default:
                            assert(0);
                    }
                    break;
                case 3:  // indexed
                    m_BytesPerPixel = (m_BitDepth + 7) >> 3;
                    std::cerr
                        << "Color Type 3 is not supported yet: "
                        << m_ColorType << std::endl;
                    assert(0);
                    break;
                case 4:  // grayscale with alpha
                    m_BytesPerPixel = (m_BitDepth * 2 + 7) >> 3;
                    std::cerr
                        << "Color Type 4 is not supported yet: "
                        << m_ColorType << std::endl;
                    assert(0);
                    break;
                case 6:  // RGBA
                    m_BytesPerPixel = (m_BitDepth * 4 + 7) >> 3;
                    switch (m_BytesPerPixel) {
                        case 4:
                            m_Img.pixel_format = PIXEL_FORMAT::RGBA8;
                            break;
                        case 8:
                            m_Img.pixel_format = PIXEL_FORMAT::RGBA16;
                            break;
                        case 16:
                            m_Img.pixel_format = PIXEL_FORMAT::RGBA32;
                            break;
                        default:
                            assert(0);
                    }
                    break;
                default:
                    std::cerr
                        << "Unkown Color Type: " << m_ColorType
                        << std::endl;
                    assert(0);
            }

            assert(m_Img.pixel_format != PIXEL_FORMAT::UNKNOWN);

            m_ScanLineSize = m_BytesPerPixel * m_Width;

            m_Img.Width = m_Width;
            m_Img.Height = m_Height;
            m_Img.bitcount = m_BytesPerPixel * 8;
            m_Img.bitdepth = m_BitDepth;
            m_Img.pitch = ALIGN(m_Width * m_BytesPerPixel,
                                4);  // for GPU address alignment
            m_Img.data_size = (size_t)m_Img.pitch * m_Img.Height;
            // TODO: you need to have image copied to pass in to m_Img.data or will get access violation
            uint8_t* data = new uint8_t[image.size()];
            for (int i = 0; i < image.size(); i++)
            {
                data[i] = image[i];
            }
            m_Img.data = data;

            return m_Img;
        }
    };
}  // namespace Corona
