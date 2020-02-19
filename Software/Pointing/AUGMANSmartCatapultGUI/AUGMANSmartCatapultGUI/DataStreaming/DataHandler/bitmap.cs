using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AUGMANSmartCatapultGUI.DataStreaming.DataHandler
{

    public unsafe struct fileheader
    {
        fixed int signature[2];
        UInt32 filesize;
        UInt32 reserved;
        UInt32 fileoffset_to_pixelarray;
    };


    public struct bitmapinfoheader
    {
        UInt32 dibheadersize;
        UInt32 width;
        UInt32 height;
        UInt16 planes;
        UInt16 bitsperpixel;
        UInt32 compression;
        UInt32 imagesize;
        UInt32 ypixelpermeter;
        UInt32 xpixelpermeter;
        UInt32 numcolorspallette;
        UInt32 mostimpcolor;
        UInt32 redmask;
        UInt32 greenmask;
        UInt32 bluemask;
    };

    public struct bitmap_header_t
    {
        fileheader fileheader;
        bitmapinfoheader bitmapinfoheader;
    };

    public class bitmap
    {

    }
}
