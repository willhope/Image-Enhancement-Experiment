#ifndef BMP_H_INCLUDED  
#define BMP_H_INCLUDED 

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * 位图文件结构及基本函数定义  打开和保存bmp文件
 */
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned char BYTE;

/* 位图文件头结构 14字节 */
typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;

/* 位图信息头结构 40字节 */
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize; // 结构长度 40B
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes; // 1
    WORD biBitCount; // 表示颜色要用到的位数
    DWORD biCompression; // 压缩格式
    DWORD biSizeImage; // 位图占用字节数=biWidth'（4的整倍数）*biHeight
    LONG biXPelsPerMeter; // 水平分辨率
    LONG biYPelsPerMeter; // 垂直分辨率
    DWORD biClrUsed; // 本图像用到的颜色数
    DWORD biClrImportant; // 本图像的重要颜色数
} BITMAPINFOHEADER;

/* 调色板 4字节 */
typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

/* 定义图像信息 */
typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[1];
} BITMAPINFO;

/* 定义位图图像 */
typedef struct _Bitmap
{
    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmih;
    int width;
    int height;
    int bitCount;    // 8 或者24
    int imageSize;    // 图像数据大小(imageSize=height*widthStep）字节
    BYTE* imageData;//排列的图像数据
    int widthStep;    //排列的图像行大小
}Bitmap;


int CreateBitmap(Bitmap* bmp, int width, int height, int bitCount);
void ReleaseBitmap(Bitmap* bmp);
int CheckIfBmp(char *path);
int CheckFilePath(char *path);
int ReadBitmap(char* path, Bitmap* bmp);
void PrintFileInfo(Bitmap *bmp);
int Printf_pixel(Bitmap* bmp, int width, int height, int bitCount);
void ShowImage(char * filepath);
int SaveBitmap(char* path, Bitmap* bmp);

#endif // BMP_H_INCLUDED  