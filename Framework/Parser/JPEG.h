#pragma once
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <stdio.h>
#include <assert.h>

#include "ImageParser.h"
#include "portable.h"
#include "libjpeg-turbo/jpeglib.h"

struct my_jpeg_error_mgr
{
	struct jpeg_error_mgr pub;
	char                  padding[8];
	jmp_buf               setjmp_buffer; // for return to caller
};
typedef struct my_jpeg_error_mgr my_jpeg_error_mgr;

// Here's the routine that will replace the standard error_exit method:
METHODDEF(void)
my_error_exit(j_common_ptr cinfo)
{
	// cinfo->err really points to a my_jpeg_error_mgr struct, so coerce pointer
	my_jpeg_error_mgr* myerr = (my_jpeg_error_mgr*)cinfo->err;

	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message)(cinfo);

	// Return control to the setjmp point
	longjmp(myerr->setjmp_buffer, 1);
}

namespace Corona
{
    class JpegParser : implements ImageParser
    {
    public:
        Image Parse(Buffer& buf) override 
        {
            // This struct contains the JPEG decompression parameters and pointers to
            // working space (which is allocated as needed by the JPEG library).
            jpeg_decompress_struct cinfo;

            // We use our private extension JPEG error handler.
            // Note that this struct must live as long as the main JPEG parameter
            // struct, to avoid dangling-pointer problems.
            my_jpeg_error_mgr jerr;

            // Step 1: allocate and initialize JPEG decompression object

            // We set up the normal JPEG error routines, then override error_exit.
            cinfo.err = jpeg_std_error(&jerr.pub);
            jerr.pub.error_exit = my_error_exit;
            // Establish the setjmp return context for my_error_exit to use.
            if (setjmp(jerr.setjmp_buffer))
            {
                // If we get here, the JPEG code has signaled an error.
                // We need to clean up the JPEG object, close the input file, and return.
                jpeg_destroy_decompress(&cinfo);
                assert(0);
            }

            // Now we can initialize the JPEG decompression object.
            jpeg_create_decompress(&cinfo);

            // Step 2: specify data source
            unsigned char* pSrcPtr = buf.GetData();
            unsigned long  SrcSize = (unsigned long)buf.GetDataSize();
            jpeg_mem_src(&cinfo, pSrcPtr, SrcSize);

            // Step 3: read file parameters with jpeg_read_header()
            jpeg_read_header(&cinfo, TRUE);
            // We can ignore the return value from jpeg_read_header since
            //   (a) suspension is not possible with the stdio data source, and
            //   (b) we passed TRUE to reject a tables-only JPEG file as an error.
            // See libjpeg.txt for more info.

            // Step 4: set parameters for decompression

            // In this example, we don't need to change any of the defaults set by
            // jpeg_read_header(), so we do nothing here.


            // Step 5: Start decompressor

            jpeg_start_decompress(&cinfo);
            // We can ignore the return value since suspension is not possible
            // with the stdio data source.

            // We may need to do some setup of our own at this point before reading
            // the data.  After jpeg_start_decompress() we have the correct scaled
            // output image dimensions available, as well as the output colormap
            // if we asked for color quantization.

            Image m_Img;
            m_Img.Width = cinfo.output_width;
            m_Img.Height = cinfo.output_height;
            m_Img.pixel_format = PIXEL_FORMAT::RGB8;
            m_Img.bitcount = (size_t)PIXEL_FORMAT::RGB8 * 8;
            m_Img.bitdepth = 8;
            m_Img.pitch = ALIGN(m_Img.Width * (size_t)m_Img.pixel_format, 4);  // for GPU address alignment
            m_Img.data_size = (size_t)(m_Img.pitch * m_Img.Height);

            uint8_t* data = new uint8_t[m_Img.data_size];
            // Step 6: while (scan lines remain to be read)
            //           jpeg_read_scanlines(...);

            // Here we use the library's state variable cinfo.output_scanline as the
            // loop counter, so that we don't have to keep track ourselves.
            while (cinfo.output_scanline < cinfo.output_height)
            {
                // jpeg_read_scanlines expects an array of pointers to scanlines.
                // Here the array is only one element long, but you could ask for
                // more than one scanline at a time if that's more convenient.

                uint8_t* pScanline0 = data;
                uint8_t* pDstScanline = pScanline0 + cinfo.output_scanline * (size_t)m_Img.pitch;
                JSAMPROW RowPtrs[1];
                RowPtrs[0] = (JSAMPROW)pDstScanline;
                jpeg_read_scanlines(&cinfo, RowPtrs, 1);
            }
            m_Img.data = data;

            // Step 7: Finish decompression

            jpeg_finish_decompress(&cinfo);
            // We can ignore the return value since suspension is not possible
            // with the stdio data source.

            // Step 8: Release JPEG decompression object

            // This is an important step since it will release a good deal of memory.
            jpeg_destroy_decompress(&cinfo);

            return m_Img;
        }
    };
}