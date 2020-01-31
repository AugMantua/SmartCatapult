//https://stackoverflow.com/a/23303847
#include "bitmap.h"
#include <string.h>
#include <stdlib.h>


bitmap_header_t *bmp_create_header(int w, int h)
{
	bitmap_header_t *pbitmap  = (bitmap_header_t*)calloc(1, sizeof(bitmap_header_t));
	int _pixelbytesize = w * h * _bitsperpixel/8;
	int _filesize = _pixelbytesize+sizeof(bitmap_header_t);
	strcpy((char*)pbitmap->fileheader.signature, "BM");
	pbitmap->fileheader.filesize = _filesize;
	pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap_header_t);
	pbitmap->bitmapinfoheader.dibheadersize = sizeof(bitmapinfoheader);
	pbitmap->bitmapinfoheader.width = w;
	pbitmap->bitmapinfoheader.height = h;
	pbitmap->bitmapinfoheader.planes = _planes;
	pbitmap->bitmapinfoheader.bitsperpixel = _bitsperpixel;
	pbitmap->bitmapinfoheader.compression = _compression;
	pbitmap->bitmapinfoheader.imagesize = _pixelbytesize;
	pbitmap->bitmapinfoheader.ypixelpermeter = _ypixelpermeter ;
	pbitmap->bitmapinfoheader.xpixelpermeter = _xpixelpermeter ;
	pbitmap->bitmapinfoheader.numcolorspallette = 0;
	return pbitmap;
}


bitmap_header_t *bmp_create_header_bi_bitfields(int w, int h, int bitsPerPixel)
{
	bitmap_header_t *pbitmap  = (bitmap_header_t*)calloc(1, sizeof(bitmap_header_t));
	int _pixelbytesize = w * h * bitsPerPixel/8;
	int _filesize = _pixelbytesize+sizeof(bitmap_header_t);
	strcpy((char*)pbitmap->fileheader.signature, "BM");
	pbitmap->fileheader.filesize = _filesize;
	pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap_header_t);
	pbitmap->bitmapinfoheader.dibheadersize = sizeof(bitmapinfoheader);
	pbitmap->bitmapinfoheader.width = w;
	pbitmap->bitmapinfoheader.height = h;
	pbitmap->bitmapinfoheader.planes = _planes;
	pbitmap->bitmapinfoheader.bitsperpixel = bitsPerPixel;
	pbitmap->bitmapinfoheader.compression = BI_BITFIELDS;
	pbitmap->bitmapinfoheader.imagesize = 0;
	pbitmap->bitmapinfoheader.ypixelpermeter = _ypixelpermeter ;
	pbitmap->bitmapinfoheader.xpixelpermeter = _xpixelpermeter ;
	pbitmap->bitmapinfoheader.numcolorspallette = 0;
	pbitmap->bitmapinfoheader.redmask 	= bits565[0];
	pbitmap->bitmapinfoheader.greenmask = bits565[1];
	pbitmap->bitmapinfoheader.bluemask 	= bits565[2];
	return pbitmap;
}

