#ifndef BMP_H_INCLUDED  
#define BMP_H_INCLUDED 

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * λͼ�ļ��ṹ��������������  �򿪺ͱ���bmp�ļ�
 */
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned char BYTE;

/* λͼ�ļ�ͷ�ṹ 14�ֽ� */
typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;

/* λͼ��Ϣͷ�ṹ 40�ֽ� */
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize; // �ṹ���� 40B
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes; // 1
    WORD biBitCount; // ��ʾ��ɫҪ�õ���λ��
    DWORD biCompression; // ѹ����ʽ
    DWORD biSizeImage; // λͼռ���ֽ���=biWidth'��4����������*biHeight
    LONG biXPelsPerMeter; // ˮƽ�ֱ���
    LONG biYPelsPerMeter; // ��ֱ�ֱ���
    DWORD biClrUsed; // ��ͼ���õ�����ɫ��
    DWORD biClrImportant; // ��ͼ�����Ҫ��ɫ��
} BITMAPINFOHEADER;

/* ��ɫ�� 4�ֽ� */
typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

/* ����ͼ����Ϣ */
typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[1];
} BITMAPINFO;

/* ����λͼͼ�� */
typedef struct _Bitmap
{
    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmih;
    int width;
    int height;
    int bitCount;    // 8 ����24
    int imageSize;    // ͼ�����ݴ�С(imageSize=height*widthStep���ֽ�
    BYTE* imageData;//���е�ͼ������
    int widthStep;    //���е�ͼ���д�С
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