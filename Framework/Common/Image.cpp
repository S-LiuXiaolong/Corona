#include "Image.h"

using namespace std;

namespace Corona {
Image::Image(Image&& rhs) noexcept {
    Width = rhs.Width;
    Height = rhs.Height;
    data = rhs.data;
    bitcount = rhs.bitcount;
    pitch = rhs.pitch;
    data_size = rhs.data_size;
    compressed = rhs.compressed;
    is_float = rhs.is_float;
    compress_format = rhs.compress_format;
    bitdepth = rhs.bitdepth;
    pixel_format = rhs.pixel_format;
    is_signed = rhs.is_signed;
    rhs.data = nullptr;
}

Image& Image::operator=(Image&& rhs) noexcept {
    if (this != &rhs) {
        Width = rhs.Width;
        Height = rhs.Height;
        data = rhs.data;
        bitcount = rhs.bitcount;
        pitch = rhs.pitch;
        data_size = rhs.data_size;
        compressed = rhs.compressed;
        is_float = rhs.is_float;
        compress_format = rhs.compress_format;
        bitdepth = rhs.bitdepth;
        pixel_format = rhs.pixel_format;
        is_signed = rhs.is_signed;
        rhs.data = nullptr;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& out, COMPRESSED_FORMAT format) {
    switch (format) {
        case COMPRESSED_FORMAT::NONE:
            out << "NONE\n";
            break;
        case COMPRESSED_FORMAT::DXT1:
            out << "DXT1\n";
            break;
        case COMPRESSED_FORMAT::DXT2:
            out << "DXT2\n";
            break;
        case COMPRESSED_FORMAT::DXT3:
            out << "DXT3\n";
            break;
        case COMPRESSED_FORMAT::DXT4:
            out << "DXT4\n";
            break;
        case COMPRESSED_FORMAT::DXT5:
            out << "DXT5\n";
            break;
        case COMPRESSED_FORMAT::DXT10:
            out << "DXT10\n";
            break;
        case COMPRESSED_FORMAT::BC1:
            out << "BC1\n";
            break;
        case COMPRESSED_FORMAT::BC1A:
            out << "BC1A\n";
            break;
        case COMPRESSED_FORMAT::BC2:
            out << "BC2\n";
            break;
        case COMPRESSED_FORMAT::BC3:
            out << "BC3\n";
            break;
        case COMPRESSED_FORMAT::BC4:
            out << "BC4\n";
            break;
        case COMPRESSED_FORMAT::BC5:
            out << "BC5\n";
            break;
        case COMPRESSED_FORMAT::BC6H:
            out << "BC6H\n";
            break;
        case COMPRESSED_FORMAT::BC7:
            out << "BC7\n";
            break;
        case COMPRESSED_FORMAT::PVRTC:
            out << "PVRTC\n";
            break;
        case COMPRESSED_FORMAT::ETC:
            out << "ETC\n";
            break;
        case COMPRESSED_FORMAT::ASTC_4x4:
            out << "ASTC 4x4\n";
            break;
        case COMPRESSED_FORMAT::ASTC_5x4:
            out << "ASTC 5x4\n";
            break;
        case COMPRESSED_FORMAT::ASTC_5x5:
            out << "ASTC 5x5\n";
            break;
        case COMPRESSED_FORMAT::ASTC_6x5:
            out << "ASTC 6x5\n";
            break;
        case COMPRESSED_FORMAT::ASTC_6x6:
            out << "ASTC 6x6\n";
            break;
        case COMPRESSED_FORMAT::ASTC_8x5:
            out << "ASTC 8x5\n";
            break;
        case COMPRESSED_FORMAT::ASTC_8x6:
            out << "ASTC 8x6\n";
            break;
        case COMPRESSED_FORMAT::ASTC_8x8:
            out << "ASTC 8x8\n";
            break;
        case COMPRESSED_FORMAT::ASTC_10x5:
            out << "ASTC 10x5\n";
            break;
        case COMPRESSED_FORMAT::ASTC_10x6:
            out << "ASTC 10x6\n";
            break;
        case COMPRESSED_FORMAT::ASTC_10x8:
            out << "ASTC 10x8\n";
            break;
        case COMPRESSED_FORMAT::ASTC_10x10:
            out << "ASTC 10x10\n";
            break;
        case COMPRESSED_FORMAT::ASTC_12x10:
            out << "ASTC 12x10\n";
            break;
        case COMPRESSED_FORMAT::ASTC_12x12:
            out << "ASTC 12x12\n";
            break;
        case COMPRESSED_FORMAT::ASTC_3x3x3:
            out << "ASTC 3x3x3\n";
            break;
        case COMPRESSED_FORMAT::ASTC_4x3x3:
            out << "ASTC 4x3x3\n";
            break;
        case COMPRESSED_FORMAT::ASTC_4x4x3:
            out << "ASTC 4x4x3\n";
            break;
        case COMPRESSED_FORMAT::ASTC_4x4x4:
            out << "ASTC 4x4x4\n";
            break;
        case COMPRESSED_FORMAT::ASTC_5x4x4:
            out << "ASTC 5x4x4\n";
            break;
        case COMPRESSED_FORMAT::ASTC_5x5x4:
            out << "ASTC 5x5x4\n";
            break;
        case COMPRESSED_FORMAT::ASTC_5x5x5:
            out << "ASTC 5x5x5\n";
            break;
        case COMPRESSED_FORMAT::ASTC_6x5x5:
            out << "ASTC 6x5x5\n";
            break;
        case COMPRESSED_FORMAT::ASTC_6x6x5:
            out << "ASTC 6x6x5\n";
            break;
        case COMPRESSED_FORMAT::ASTC_6x6x6:
            out << "ASTC 6x6x6\n";
            break;
        case COMPRESSED_FORMAT::UNKNOWN:
            out << "UNKNOWN\n";
            break;
        default:
            break;
            //assert(0);
    }

    return out;
}

ostream& operator<<(ostream& out, const Image& image) {
    out << "Image" << endl;
    out << "-----" << endl;
    out << "Width: " << image.Width << endl;
    out << "Height: " << image.Height << endl;
    out << "Bit Count: " << image.bitcount << endl;
    out << "Pitch: " << image.pitch << endl;
    out << "Data Size: " << image.data_size << endl;
    out << "Compressed: " << image.compressed << endl;
    out << "Compressed Format: " << image.compress_format << endl;

    return out;
}

}  // namespace Corona
