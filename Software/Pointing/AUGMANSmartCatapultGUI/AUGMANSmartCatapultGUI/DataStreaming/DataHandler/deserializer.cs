using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

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
        public UInt32 counter;
        public UInt32 frameTick;
        /**/
        public bool targetInFrame;
        public UInt32 targetConfident;
        /**/
        public UInt32 sizeofBuffer;
        public UInt32 sizeofHeader;
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
    };

    public struct bitmap_header_t
    {
        fileheader fileheader;
        bitmapinfoheader bitmapinfoheader;
    };

    public class deserializer
    {
        const int _HEADER_OFFSET = 0x0;

        public static unsafe T ByteArrayToStructure<T>(byte[] bytes, int offset) where T : struct
        {
            fixed (byte* ptr = &bytes[offset])
            {
                return (T)Marshal.PtrToStructure((IntPtr)ptr, typeof(T));
            }
        }

        public static streamerPacketHeader getPacketHeader(byte[] bytes)
        {
            return ByteArrayToStructure<streamerPacketHeader>(bytes, 0x0);
        }


        public static byte[] PadLines(byte[] bytes, int rows, int columns)
        {
            int currentStride = columns; // 3
            int newStride = columns;  // 4
            byte[] newBytes = new byte[newStride * rows];
            for (int i = 0; i < rows; i++)
                Buffer.BlockCopy(bytes, currentStride * i, newBytes, newStride * i, currentStride);
            return newBytes;
        }

        public static Bitmap CreateBitmap(int imageWidth, int imageHeight,byte[] imageData, PixelFormat format)
        {
            int columns = imageWidth;
            int rows = imageHeight;
            int bpp = 2;
            int stride = columns * bpp;

            Bitmap im = new Bitmap(columns, rows, stride,
                     format,
                     Marshal.UnsafeAddrOfPinnedArrayElement(imageData, 0));
            //im.Save(Path);
            return im;
        }

        public static BitmapImage ConvertBitmapToSource(Bitmap src)
        {
            MemoryStream ms = new MemoryStream();
            ((System.Drawing.Bitmap)src).Save(ms, System.Drawing.Imaging.ImageFormat.Bmp);
            BitmapImage image = new BitmapImage();
            image.BeginInit();
            ms.Seek(0, SeekOrigin.Begin);
            image.StreamSource = ms;
            image.EndInit();
            return image;
        }

    }


    /* https://stackoverflow.com/questions/21555394/how-to-create-bitmap-from-byte-array*/


}
