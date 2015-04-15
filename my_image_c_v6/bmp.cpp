/** Descriptions:       位图文件结构及基本函数定义  打开和保存bmp文件
**
** Created by:          zengwh
** Created date:        2014-03-14
** Version:             1.0
** Descriptions:        Preliminary version.
**
**------------------------------------------------------------------------------------------------------
*/


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "BMP.h"

/**
 * 位图创建函数  创建一个Bitmap结构，并为图像数据分配空间
 *
 * 使用方法：
 *     Bitmap *bmp=(Bitmap*)malloc(sizeof(Bitmap));
 *     ret=CreateBitmap(bmp,50,50,3);
 */
int CreateBitmap(Bitmap* bmp, int width, int height, int bitCount)
{
    bmp->width=width;
    bmp->height=height;
    bmp->bmih.biWidth=width;
    bmp->bmih.biHeight=height;

    bmp->widthStep=(int)((width*bitCount+31)/32)*4;        //计算排列的宽度
    bmp->imageSize=bmp->height*bmp->widthStep*sizeof(BYTE);//计算排列的图像大小

    if(bitCount==8)
    {
        bmp->bitCount=8;
        bmp->bmfh.bfType=0x4d42;    //注意是4d42 这个地方折磨我一下午啊
        bmp->bmfh.bfReserved1=0;
        bmp->bmfh.bfReserved2=0;
        bmp->bmih.biBitCount=8;
        bmp->bmih.biSize=40;
        bmp->bmih.biPlanes=1;
        bmp->bmfh.bfSize=54+256*4+height*bmp->widthStep;
        bmp->bmfh.bfOffBits=1078;
        bmp->bmih.biBitCount=8;
        bmp->bmih.biCompression=0;
        bmp->bmih.biSizeImage=bmp->imageSize;
        bmp->bmih.biClrUsed=0;
        bmp->bmih.biClrImportant=0;
        bmp->bmih.biXPelsPerMeter=0;
        bmp->bmih.biYPelsPerMeter=0;
    }
    else if (bitCount==24)
    {
        bmp->bitCount=24;
        bmp->bmfh.bfType=0x4d42;
        bmp->bmih.biBitCount=24;
        bmp->bmfh.bfReserved1=0;
        bmp->bmfh.bfReserved2=0;
        bmp->bmih.biSize=40;
        bmp->bmih.biPlanes=1;
        bmp->bmfh.bfSize=54+height*bmp->widthStep;
        bmp->bmfh.bfOffBits=54;
        bmp->bmih.biBitCount=24;
        bmp->bmih.biSizeImage=bmp->imageSize;
        bmp->bmih.biClrUsed=0;
        bmp->bmih.biCompression=0;
        bmp->bmih.biClrImportant=0;
        bmp->bmih.biXPelsPerMeter=0;
        bmp->bmih.biYPelsPerMeter=0;
    }
    else
    {
        printf("Error(CreateBitmap): only supported 8 or 24 bits bitmap.\n");
        return -1;
    }

    bmp->imageData=(BYTE*)malloc(bmp->imageSize);        //分配数据空间
    if(!(bmp->imageData))
    {
        printf("Error(CreateBitmap): can not allocate bitmap memory.\n");
        return -1;
    }
    return 0;
}

/**
 * 位图指针释放函数  释放位图数据空间
 *
 * 使用方法：
 *     ReleaseBitmap(bmp);
 */
void ReleaseBitmap(Bitmap* bmp)
{
    free(bmp->imageData);
    bmp->imageData=NULL;
    free(bmp);
    bmp=NULL;
}


int CheckIfBmp(char *path)
{
    int len;
    
    char ext[3];

	len = strlen(path) / sizeof(char);
    //check whether the path include the characters "bmp" at end
    strncpy(ext, &path[len - 3], 3);
    if (!(ext[0] == 'b' && ext[1] == 'm' && ext[2] == 'p')) {
        printf("Error(CheckPath): the extension of the file is not bmp.\n");
        return -1;
    }
    return 0;
}


/**
 * 路径检查函数：是否为BMP文件，是否可读
 * 正确返回0，错误返回-1
 *
 * 使用方法
 *         ret=CheckFilePath(path);
 */
int CheckFilePath(char *path)
{
    FILE *fd;
    CheckIfBmp(path);

    //check whether the file can be read or not
    fd = fopen(path, "r");
    if (!fd)
    {
        printf("Error(CheckPath): can not open the file.\n");
        return -1;
    }
    fclose(fd);

    return 0;
}


/**
 * 从文件中读取位图函数
 * 正确返回0，错误返回-1
 *
 * 使用方法：
 *     bmp=(Bitmap*)malloc(sizeof(Bitmap));
 *     ret=ReadBitmap(path, bmp);
 */
int ReadBitmap(char* path, Bitmap* bmp)
{
    int ret;
    FILE *fd;

    //检查路径是否可读
    ret=CheckFilePath(path);
    if(ret==-1)
    {
        printf("Error(ReadBitmap): the path of the image is invalid.\n");
        return -1;
    }

    //打开文件
    fd=fopen(path,"rb");
    if(fd==0)
    {
        printf("Error(ReadBitmap): can not open the image.\n");
        return -1;
    }

    //读取文件信息头     14字节
    fread(&(bmp->bmfh.bfType),sizeof(WORD),1,fd);
    fread(&(bmp->bmfh.bfSize),sizeof(DWORD),1,fd);
    fread(&(bmp->bmfh.bfReserved1),sizeof(WORD),1,fd);
    fread(&(bmp->bmfh.bfReserved2),sizeof(WORD),1,fd);
    fread(&(bmp->bmfh.bfOffBits),sizeof(DWORD),1,fd);

    //读取位图信息头    40字节
    fread(&(bmp->bmih.biSize),sizeof(DWORD),1,fd);
    fread(&(bmp->bmih.biWidth),sizeof(DWORD),1,fd);
    fread(&(bmp->bmih.biHeight),sizeof(DWORD),1,fd);
    fread(&(bmp->bmih.biPlanes),sizeof(WORD),1,fd);
    fread(&(bmp->bmih.biBitCount),sizeof(WORD),1,fd);
    fread(&(bmp->bmih.biCompression),sizeof(DWORD),1,fd);
    fread(&(bmp->bmih.biSizeImage),sizeof(DWORD),1,fd);
    fread(&(bmp->bmih.biXPelsPerMeter),sizeof(DWORD),1,fd);
    fread(&(bmp->bmih.biYPelsPerMeter),sizeof(DWORD),1,fd);
    fread(&(bmp->bmih.biClrUsed),sizeof(DWORD),1,fd);
    fread(&(bmp->bmih.biClrImportant),sizeof(DWORD),1,fd);

    //创建位图结构
    ret=CreateBitmap(bmp, bmp->bmih.biWidth, bmp->bmih.biHeight, bmp->bmih.biBitCount);
    if(ret==-1)
    {
        printf("Error(CreateBitmap): can not CreateBitmap.\n");
        return -1;
    }

    //读取图像数据
    //由于4字节对齐格式
    fseek(fd,bmp->bmfh.bfOffBits,SEEK_SET);    //定位到图像数据区
    ret=fread(bmp->imageData,bmp->imageSize,1,fd);
    if(ret==0)
    {
        if(feof(fd))    //if the file pointer point to the end of the file
        {
        }
        if(ferror(fd))  //if error happened while read the pixel data
        {
            printf("Error(ReadBitmap): can not read the pixel data.\n");
            fclose(fd);
            return -1;
        }
    }

    //关闭文件
    fclose(fd);
    return 0;
}


/**
 * 打印文件信息头和位图信息头
 *
 * 使用方法：
 *     Bitmap *bmp=(Bitmap*)malloc(sizeof(Bitmap));
 *     ret=PrintFileInfo(bmp);
 */
void PrintFileInfo(Bitmap *bmp)  
{  
    printf("The contents in the file header of the BMP file:\n");  
    printf("bfOffBits: %ld\n",bmp->bmfh.bfOffBits);  
    printf("bfReserved1: %ld\n",bmp->bmfh.bfReserved1);  
    printf("bfReserved2: %ld\n",bmp->bmfh.bfReserved2);  
    printf("bfSize: %ld\n",bmp->bmfh.bfSize);  
    printf("bfType: %ld\n",bmp->bmfh.bfType);  


    printf("The content in the info header of the BMP file:\n");  
    printf("biBitCount: %ld\n",bmp->bmih.biBitCount);  
    printf("biClrImportant: %ld\n",bmp->bmih.biClrImportant);  
    printf("biClrUsed: %ld\n",bmp->bmih.biClrUsed);  
    printf("biCompression: %ld\n",bmp->bmih.biCompression);  
    printf("biHeight: %ld\n",bmp->bmih.biHeight);  
    printf("biPlanes: %ld\n",bmp->bmih.biPlanes);  
    printf("biSize: %ld\n",bmp->bmih.biSize);  
    printf("biSizeImage: %ld\n",bmp->bmih.biSizeImage);  
    printf("biWidth: %ld\n",bmp->bmih.biWidth);  
    printf("biXPelsPerMeter: %ld\n",bmp->bmih.biXPelsPerMeter);  
    printf("biYPelsPerMeter: %ld\n",bmp->bmih.biYPelsPerMeter);  
}  


int Printf_pixel(Bitmap* bmp, int width, int height, int bitCount)
{
	int i;
	int j;
	int k;
	int pixel;
	int pixelnum[256]={0};
	if(bitCount==8)
	{
		for(i=0;i<height;i++)
		{
			for(j=0;j<width;j++)
			{
				for(k = 0 ; k < 256; k++)
				{
					pixel = *(bmp->imageData+bmp->widthStep*(height-1-i)+j);
					if(pixel == k)
					pixelnum[k]++;
				
				}
			}
			
		}
		
	   for(k = 0 ; k < 256; k++)
			printf("灰度为%d的像素个数为%d\n",k,pixelnum[k]);
				

	}
	else if(bitCount==24)
	{
		for(i=0;i<height;i++)
		{
			for(j=0;j<width*3;j++)
			{
				printf("(");
				pixel=*(bmp->imageData+bmp->widthStep*(height-1-i)+j);
				printf("%d,",pixel);
				j++;
				pixel=*(bmp->imageData+bmp->widthStep*(height-1-i)+j);
				printf("%d,",pixel);
				j++;
				pixel=*(bmp->imageData+bmp->widthStep*(height-1-i)+j);
				printf("%d) ",pixel);
			}
			printf("\n");
		}
	}
	else
	{
		printf("Only supported: 8 or 24 bits.\n");
	}

    return 0;
}


void ShowImage(char * filepath)  
{  
    char cmd[200];  
    strcpy(cmd,"start ");  
    strcat(cmd,filepath);  
    printf("%s\n",cmd); 
	printf("\n");
    system(cmd);  
} 


/**
 * 保存位图到文件中去
 * 正确返回0，错误返回-1
 *
 * 使用方法：
 *     bmp=(Bitmap*)malloc(sizeof(Bitmap));
 *     ret=SaveBitmap(path, bmp);
 */
int SaveBitmap ( char* path, Bitmap* bmp )
{
    int ret;
    FILE *fd;

    //检查路径是否正确
    int len = strlen(path) / sizeof(char);
    char ext[3];
    //check whether the path include the characters "bmp" at end
    strncpy(ext, &path[len - 3], 3);
    if (!(ext[0] == 'b' && ext[1] == 'm' && ext[2] == 'p'))
    {
        printf("Error(SaveBitmap): the extension of the file is not bmp.\n");
        return -1;
    }

    //打开文件
    fd=fopen(path,"wb");
    if(fd==0)
    {
        printf("Error(SaveBitmap): can not open the image.\n");
        return -1;
    }

    //保存文件信息头     14字节
    fwrite(&(bmp->bmfh.bfType),sizeof(WORD),1,fd);
    fwrite(&(bmp->bmfh.bfSize),sizeof(DWORD),1,fd);
    fwrite(&(bmp->bmfh.bfReserved1),sizeof(WORD),1,fd);
    fwrite(&(bmp->bmfh.bfReserved2),sizeof(WORD),1,fd);
    fwrite(&(bmp->bmfh.bfOffBits),sizeof(DWORD),1,fd);

    //保存位图信息头    40字节
    fwrite(&(bmp->bmih.biSize),sizeof(DWORD),1,fd);
    fwrite(&(bmp->bmih.biWidth),sizeof(DWORD),1,fd);
    fwrite(&(bmp->bmih.biHeight),sizeof(DWORD),1,fd);
    fwrite(&(bmp->bmih.biPlanes),sizeof(WORD),1,fd);
    fwrite(&(bmp->bmih.biBitCount),sizeof(WORD),1,fd);
    fwrite(&(bmp->bmih.biCompression),sizeof(DWORD),1,fd);
    fwrite(&(bmp->bmih.biSizeImage),sizeof(DWORD),1,fd);
    fwrite(&(bmp->bmih.biXPelsPerMeter),sizeof(DWORD),1,fd);
    fwrite(&(bmp->bmih.biYPelsPerMeter),sizeof(DWORD),1,fd);
    fwrite(&(bmp->bmih.biClrUsed),sizeof(DWORD),1,fd);
    fwrite(&(bmp->bmih.biClrImportant),sizeof(DWORD),1,fd);

    //如果为8位，则 保存调色板
    RGBQUAD pal[256];
    int i;
    if(bmp->bitCount==8)
    {
        for(i=0;i<256;i++)
        {
            pal[i].rgbBlue=i;
            pal[i].rgbGreen=i;
            pal[i].rgbRed=i;
            pal[i].rgbReserved=0;
        }
        if(fwrite(pal,sizeof(RGBQUAD)*256,1,fd)!=1)
        {
            printf("Error(SaveBitmap): can not write Color Palette.\n");
            return -1;
        }
    }


    //保存图像数据
    ret=fwrite(bmp->imageData,bmp->imageSize,1,fd);
    if(ret!=1)
    {
        printf("Error(SaveBitmap): can not save the pixel data.\n");
        return -1;
    }

    //关闭文件
    fclose(fd);
	printf("%s file save sucessfully!!!\n",path);
	printf("open %s file\n",path);
	ShowImage(path);
    return 0;
}

