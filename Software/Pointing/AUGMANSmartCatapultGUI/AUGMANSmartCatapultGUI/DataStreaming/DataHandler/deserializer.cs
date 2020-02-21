using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;



namespace AUGMANSmartCatapultGUI.DataStreaming.DataHandler
{

    /**
    * 
    * header structure definition
    * this header must contain timing information of the bitmap
    * master will reply to the stream with position of the target if found
    * and time of the original frame in order to understand in case speed/acceleration ecc
    * If there is a target into the frame master will set targetInFrame and give % of confident
    */
    public struct streamerPacketHeader
    {
        /**/
        UInt32 counter;
        UInt32 frameTick;
        /**/
        bool targetInFrame;
        UInt32 targetConfident;
        /**/
        UInt32 sizeofBuffer;
        UInt32 sizeofHeader;
    };

    /*Bitmap structure*/

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
    } ;

    public struct bitmap_header_t
    {
        fileheader fileheader;
        bitmapinfoheader bitmapinfoheader;
    };

    public class deserializer
    {
        const int _HEADER_OFFSET = 0x0;

        public static unsafe T ByteArrayToStructure<T>(byte[] bytes,int offset) where T : struct
        {
            fixed (byte* ptr = &bytes[offset])
            {
                return (T)Marshal.PtrToStructure((IntPtr)ptr, typeof(T));
            }
        }

        public static streamerPacketHeader getPacketHeader(byte[]bytes)
        {
            return ByteArrayToStructure<streamerPacketHeader>(bytes, 0x0);
        }
    }

    /* https://stackoverflow.com/questions/21555394/how-to-create-bitmap-from-byte-array

    static byte[] PadLines(byte[] bytes, int rows, int columns)
    {
        int currentStride = columns; // 3
        int newStride = columns;  // 4
        byte[] newBytes = new byte[newStride * rows];
        for (int i = 0; i < rows; i++)
            Buffer.BlockCopy(bytes, currentStride * i, newBytes, newStride * i, currentStride);
        return newBytes;
    }

    int columns = imageWidth;
    int rows = imageHeight;
    int stride = columns;
    byte[] newbytes = PadLines(imageData, rows, columns);

    Bitmap im = new Bitmap(columns, rows, stride,
             PixelFormat.Format8bppIndexed,
             Marshal.UnsafeAddrOfPinnedArrayElement(newbytes, 0));

    im.Save("C:\\Users\\musa\\Documents\\Hobby\\image21.bmp");

    */
}
