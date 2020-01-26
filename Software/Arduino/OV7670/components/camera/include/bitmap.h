#ifndef _BITMAP_H_
#define _BITMAP_H_
#include <stdint.h>

#define _bitsperpixel 24
#define _planes 1
#define _compression 0
#define _xpixelpermeter 0x130B //2835 , 72 DPI
#define _ypixelpermeter 0x130B //2835 , 72 DPI
#define pixel 0xFF

/*Compression methods for bitmap header*/
#define BI_RGB 0
#define BI_RLE8 1
#define BI_RLE4 2
#define BI_BITFIELDS 3
#define BI_JPEG 4
#define BI_PNG 5
#define BI_ALPHABITFIELDS 6
#define BI_CMYK 11
#define BI_CMYKRLE8 12
#define BI_CMYKRLE4 13

//RGB Bitmasks
static const uint32_t bits555[] = {0x007C00,0x0003E0,0x00001F};
static const uint32_t bits565[] = {0x00F800, 0x0007E0, 0x00001F};
static const uint32_t bits888[] = {0xFF0000, 0x00FF00, 0x0000FF};

typedef struct __attribute__((packed, aligned(1)))
{
    uint8_t signature[2];
    uint32_t filesize;
    uint32_t reserved;
    uint32_t fileoffset_to_pixelarray;
} fileheader;
typedef struct __attribute__((packed, aligned(1)))
{
    uint32_t dibheadersize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitsperpixel;
    uint32_t compression;
    uint32_t imagesize;
    uint32_t ypixelpermeter;
    uint32_t xpixelpermeter;
    uint32_t numcolorspallette;
    uint32_t mostimpcolor;
    uint32_t redmask;
    uint32_t greenmask;
    uint32_t bluemask;
} bitmapinfoheader;

typedef struct
{
    fileheader fileheader;
    bitmapinfoheader bitmapinfoheader;
} bitmap_header_t;

bitmap_header_t *bmp_create_header(int w, int h);
bitmap_header_t *bmp_create_header_generic(int w, int h, int bitsperpixel);
#endif
