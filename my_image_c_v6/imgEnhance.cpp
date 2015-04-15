#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include "BMP.h"
#include "imgEnhance.h"



/**
 * 直方均衡化
 *
 * 使用方法：
 *     Bitmap *srcbmp=(Bitmap*)malloc(sizeof(Bitmap));
 *     Bitmap *dstbmp=(Bitmap*)malloc(sizeof(Bitmap));
 *     ret=HistEqualization(dstbmp,srcbmp);
 */
int HistEqualization(Bitmap* dstBmp, Bitmap* srcBmp)
{
	int ret;
    int i,j;
	int sum = 0;
    BYTE pixel;
	int pixelnum[256];
	double rate;
	BYTE *lpSrc;
	BYTE *lpDst;
	
	

	for(i = 0 ; i < 256; i++)
		pixelnum[i] = 0;

    //检查图像格式是否合法
    if(srcBmp->bitCount!=8)
    {
        printf("Error(): the source image must be in gray scale.\n");
        return -1;
    }
	
    //为dst图像分配数据空间
    ret=CreateBitmap(dstBmp,srcBmp->width,srcBmp->height,srcBmp->bitCount);
    if(ret==-1)
    {
        printf("Error(HistEqualization): can't create target image.\n");
        return -1;
    }

    //计算灰度数据
    for(i=0;i<srcBmp->height;i++)
    {
		lpSrc = srcBmp->imageData+srcBmp->widthStep*(srcBmp->height-1-i );
        for(j=0;j<srcBmp->width;j++)
        {
			pixel = *lpSrc; 
			pixelnum[pixel]++;
			lpSrc ++;
        }
    }
	
	/*for(i = 0 ; i < 256; i++)
			printf("灰度为%d的像素个数为%d\n",i,pixelnum[i]);*/

	printf("Start Histogram Equalization：\n");
	for(i = 0; i < 256; i++)
	{
		sum += pixelnum[i];
		rate = (double) sum / (srcBmp->width * srcBmp->height);
		pixelnum[i] = (BYTE) 255 * rate;
	}
	
	/*for(i = 0 ; i < 256; i++)
			printf("灰度为%d的像素个数为%d\n",i,pixelnum[i]);*/

	 for(i=0;i<srcBmp->height;i++)
    {
		lpSrc = srcBmp->imageData+srcBmp->widthStep*(srcBmp->height-1-i );
		lpDst = dstBmp->imageData+dstBmp->widthStep*(dstBmp->height-1-i);
        for(j=0;j<srcBmp->width;j++)
        {
			pixel = *lpSrc;
			*lpDst = pixelnum[pixel];
			lpSrc ++;
			lpDst ++;
        }
    }
	
	printf("Finish Histogram Equalization!!!\n");
	
    return 0;
}

/**
 * 分段线性变换
 *
 * 使用方法：
 *     Bitmap *srcbmp=(Bitmap*)malloc(sizeof(Bitmap));
 *     Bitmap *dstbmp=(Bitmap*)malloc(sizeof(Bitmap));
 *     ret=Linear_trans(dstbmp,srcbmp);
 */
int Linear_trans(Bitmap* dstBmp, Bitmap* srcBmp)
{
	int i, j;
	int ret;
	BYTE pixel, max_pixel, min_pixel;
	int pixelnum[256];

	for(i = 0 ; i < 256; i++)
	pixelnum[i] = 0;

	max_pixel = 0;
	min_pixel = 0;
    //检查图像格式是否合法
    if(srcBmp->bitCount!=8)
    {
        printf("Error(): the source image must be in gray scale.\n");
        return -1;
    }
	
    //为dst图像分配数据空间
    ret=CreateBitmap(dstBmp,srcBmp->width,srcBmp->height,srcBmp->bitCount);
    if(ret==-1)
    {
        printf("Error(Linear_trans): can't create target image.\n");
        return -1;
    }

	for(i=0;i<srcBmp->height;i++)
    {
        for(j=0;j<srcBmp->width;j++)
        {
			pixel = *(srcBmp->imageData+srcBmp->widthStep*(srcBmp->height-1-i)+j);
			if (pixel >= max_pixel)  max_pixel = pixel;
			if (pixel <= min_pixel)  min_pixel = pixel;
        }
    }

	for(i=0;i<srcBmp->height;i++)
    {
        for(j=0;j<srcBmp->width;j++)
        {
			pixel = *(srcBmp->imageData+srcBmp->widthStep*(srcBmp->height-1-i)+j);
			if (pixel >= max_pixel)  *(dstBmp->imageData+dstBmp->widthStep*(dstBmp->height-1-i)+j) = 255;
			else if (pixel <= min_pixel)  *(dstBmp->imageData+dstBmp->widthStep*(dstBmp->height-1-i)+j) = 0;
			else *(dstBmp->imageData+dstBmp->widthStep*(dstBmp->height-1-i)+j) = (BYTE) 255/(max_pixel - min_pixel) * pixel - 5;
        }
    }

	return 0;
}


/**
 * 灰度幂次变化 
 *
 * 使用方法：
 *     Bitmap *srcbmp=(Bitmap*)malloc(sizeof(Bitmap));
 *     Bitmap *dstbmp=(Bitmap*)malloc(sizeof(Bitmap));
 *     ret=Linear_trans(dstbmp,srcbmp,para_c,para_g);
 */
 
int power_convertion(Bitmap* dstBmp, Bitmap* srcBmp,double para_c,double para_g)   
{   
    int i,j;  
	int ret;
    BYTE pixel;

	 //为dst图像分配数据空间
    ret=CreateBitmap(dstBmp,srcBmp->width,srcBmp->height,srcBmp->bitCount);
    if(ret==-1)
    {
        printf("Error(power_convertion): can't create target image.\n");
        return -1;
    }

    for(i=0;i<srcBmp->height;i++)   
    {   
		for(j=0;j<srcBmp->width;j++)   
        {   
			pixel = *(srcBmp->imageData+srcBmp->widthStep*(srcBmp->height-1-i)+j);
            pixel = (BYTE) ( para_c * pow( pixel/255.0, para_g ) * 255.0 ) ;//归一化后幂次变换     
			 *(dstBmp->imageData+dstBmp->widthStep*(dstBmp->height-1-i)+j) = pixel ;
        }   
    }  

	return 0;
   
}   


// Sobel 算子

int Sobel(Bitmap* dstBmp, Bitmap* srcBmp) 
{
	
	int i,j,k,m;  
	int ret;
    double pixel1 = 0; 
	int pixel2 = 0; 
	int pixel3 = 0; 
	int num = 0;
	double pixel_tmp = 0;
	int pixel_min = 0;
	int pixel_max = 0;
	int pixel_array[300][300] = {0};
	BYTE *lpSrc;
	BYTE *lpDst;

	//水平的模板
	int N = 3;
	int SblMask1[3][3] = {  
                -1,-2,-1,  
                 0, 0, 0,  
                 1, 2, 1  
    }; 

	 //垂直的模板

	int SblMask2[3][3] = {  
                -1, 0, 1,  
                -2, 0, 2,  
                -1, 0, 1  
    }; 

	 //为dst图像分配数据空间
    ret=CreateBitmap(dstBmp,srcBmp->width,srcBmp->height,srcBmp->bitCount);
    if(ret==-1)
    {
        printf("Error(Sobel): can't create target image.\n");
        return -1;
    }

	 for(i = 0; i < srcBmp->height; i++)   
    {   
		lpSrc = srcBmp->imageData+srcBmp->widthStep*(srcBmp->height-1-i );
		lpDst = dstBmp->imageData+dstBmp->widthStep*(dstBmp->height-1-i);

		for(j = 0; j < srcBmp->width ; j++)   
        {   
			if ( i >= (N -1)/2 && i < srcBmp->height - (N -1)/2 && j >= (N -1)/2 && j < srcBmp->width - (N -1)/2)
			{
				pixel1 = 0;
			    pixel2 = 0;

				for(m = -(N -1)/2; m <= (N -1)/2 ; m++)
					for(k = -(N -1)/2; k <= (N -1)/2; k++)
					{ 
					
						pixel1 += (*(lpSrc + k +  m * srcBmp->widthStep))* SblMask1[(N -1)/2 - m][k+(N -1)/2] ;
						pixel2 += (*(lpSrc + k +  m * srcBmp->widthStep))* SblMask2[(N -1)/2 - m][k+(N -1)/2] ;
						
					
					}
					
			pixel3 = abs( pixel1 ) + abs( pixel2 );
			
			if (pixel3 > 255)  
				*lpDst = 255;
			else               
				*lpDst = pixel3;
			
			lpSrc ++;
			lpDst ++;
			}
			
			
        }  
    }  


	return 0;
}



int Laplacian(Bitmap* dstBmp, Bitmap* srcBmp) 
{
	
	int i,j;  
	int ret;
    int pixel = 0;
	int pixel1 = 0;
	double pixel_tmp = 0;
	int pixel_min = 0;
	int pixel_max = 0;
	int N = 3;
	BYTE *lpSrc;
	BYTE *lpDst;
	



	int LaplMask[3][3] = {  
                0,1,0,  
                 1, -4, 1,  
                 0, 1, 0  
    }; 



	 //为dst图像分配数据空间
    ret=CreateBitmap(dstBmp,srcBmp->width,srcBmp->height,srcBmp->bitCount);
    if(ret==-1)
    {
        printf("Error(Laplacian): can't create target image.\n");
        return -1;
    }

	 for(i = 0; i < srcBmp->height ; i++)   
    {   
		lpSrc = srcBmp->imageData+srcBmp->widthStep*(srcBmp->height-1-i );
		lpDst = dstBmp->imageData+dstBmp->widthStep*(dstBmp->height-1-i);
		for(j = 0; j < srcBmp->width ; j++)   
        {   
			if ( i >= (N -1)/2 && i < srcBmp->height - (N -1)/2 && j >= (N -1)/2 && j < srcBmp->width - (N -1)/2)
			{
				
				pixel = *(lpSrc - 1) + *(lpSrc + 1 ) + *( lpSrc - srcBmp->widthStep ) + *( lpSrc + srcBmp->widthStep ) - 4 * (*( lpSrc ));
				pixel1 = (*lpSrc) - pixel ;
                
				if(pixel1 > 255) *lpDst = 255;    //出现有黑影 说明有负值或超过255
				else if(pixel1 < 0 )  *lpDst = 0;
				else *lpDst = pixel1;
			
				
			}

			
			lpSrc ++;
			lpDst ++;
       }  
    }  

/*	printf("%d ",pixel_max);
	printf("%d ",pixel_min);

	pixel_max = pixel_max - pixel_min;
	pixel_tmp = 255 / pixel_max;
	  for(i = 0; i < dstBmp->height ; i++)   
    {   
		lpSrc = srcBmp->imageData+srcBmp->widthStep*(srcBmp->height-1-i );
		lpDst = dstBmp->imageData+dstBmp->widthStep*(dstBmp->height-1-i);
		for(j = 0; j < dstBmp->width ; j++)   
        {
			*lpDst = (BYTE) (*lpDst - pixel_min) * pixel_tmp  ;
			lpDst ++;	
			lpSrc ++;
       }  
    } 
  
	*/
	return 0;
}


int Smooth_55(Bitmap* dstBmp, Bitmap* srcBmp) 
{
	
	int i,j,m,k;  
	int ret;
    unsigned long sum = 0;
	int N = 5;
	BYTE *lpSrc;
	BYTE *lpDst;

	 //为dst图像分配数据空间
    ret=CreateBitmap(dstBmp,srcBmp->width,srcBmp->height,srcBmp->bitCount);
    if(ret==-1)
    {
        printf("Error(Smooth_55): can't create target image.\n");
        return -1;
    }


	 for(i = 0; i < srcBmp->height ; i++)   
    {   
		lpSrc = srcBmp->imageData+srcBmp->widthStep*(srcBmp->height-1-i );
		lpDst = dstBmp->imageData+dstBmp->widthStep*(dstBmp->height-1-i);
		for(j = 0; j < srcBmp->width ; j++)   
        {   
			if ( i >= (N -1)/2 && i < srcBmp->height - (N -1)/2 && j >= (N -1)/2 && j < srcBmp->width - (N -1)/2)
			{
				for(m = -(N -1)/2; m <= (N -1)/2 ; m++)
					for(k = -(N -1)/2; k <= (N -1)/2; k++)
					{ 
					//	if((i+k)>=0 && (j+m)>=0 && (i+k)< srcBmp->height && (j+m)<srcBmp->width)
						sum += *(lpSrc + k +  m * srcBmp->widthStep);
					 //printf("%d ",sum);
					}	
			sum = sum / (N * N);
			if(sum < 0) sum = 0;
			if(sum > 255) sum = 255;
			 *lpDst = sum;
			 sum = 0;
			lpSrc ++;
			lpDst ++;
			}
			
       }  

		
    }  
	return 0;
}

int Multiply(Bitmap* dstBmp, Bitmap* srcBmp1,Bitmap* srcBmp)
{
	
	int i,j;  
	int ret;
    int pixel = 0;
	int pixel_min = 0;
	int pixel_max = 0;
    double pixel_tmp = 0;
	BYTE *lpSrc;
	BYTE *lpSrc1;
	BYTE *lpDst;

	 //为dst图像分配数据空间
    ret=CreateBitmap(dstBmp,srcBmp->width,srcBmp->height,srcBmp->bitCount);
    if(ret==-1)
    {
        printf("Error(Multiply): can't create target image.\n");
        return -1;
    }


	 for(i = 0; i < srcBmp->height ; i++)   
    {   
		lpSrc = srcBmp->imageData+srcBmp->widthStep*(srcBmp->height-1-i );
		lpSrc1 = srcBmp1->imageData+srcBmp1->widthStep*(srcBmp1->height-1-i );
		
		for(j = 0; j < srcBmp->width ; j++)   
        {   
			//pixel = ((double) ( *lpSrc ) / 255 )* ( (double)( *lpDst )  / 255 );
			pixel = (*lpSrc) * (*lpSrc1)  ;
			
			/*printf("%d " ,pixel);*/
			if(pixel > pixel_max)  pixel_max = pixel;
			if(pixel < pixel_min)  pixel_min = pixel;
			
			lpSrc ++;
			lpSrc1 ++;
		
		 }  

		
    }  

	printf("%d ",pixel_max);
	printf("%d ",pixel_min);

	pixel_max = pixel_max - pixel_min;
	pixel_tmp = (double)255 / pixel_max;
	printf("%f ",pixel_tmp);
	  for(i = 0; i < dstBmp->height ; i++)   
    {   
		lpSrc = srcBmp->imageData+srcBmp->widthStep*(srcBmp->height-1-i );
		lpSrc1 = srcBmp1->imageData+srcBmp1->widthStep*(srcBmp1->height-1-i );
		lpDst = dstBmp->imageData+dstBmp->widthStep*(dstBmp->height-1-i);
		for(j = 0; j < dstBmp->width ; j++)   
        {
			pixel = (*lpSrc) * (*lpSrc1) ;
		   *lpDst =  (pixel - pixel_min) * pixel_tmp  ;
			lpDst ++;
			lpSrc1 ++;
			lpSrc ++;
       }  
    }
	return 0;	
}

int Add_img(Bitmap* dstBmp, Bitmap* srcBmp1,Bitmap* srcBmp)
{
	
	int i,j;  
	int ret;
    int pixel = 0;
	BYTE *lpSrc;
	BYTE *lpSrc1;
	BYTE *lpDst;

	 //为dst图像分配数据空间
    ret=CreateBitmap(dstBmp,srcBmp->width,srcBmp->height,srcBmp->bitCount);
    if(ret==-1)
    {
        printf("Error(Multiply): can't create target image.\n");
        return -1;
    }


	 for(i = 0; i < srcBmp->height ; i++)   
    {   
		lpSrc = srcBmp->imageData+srcBmp->widthStep*(srcBmp->height-1-i );
		lpSrc1 = srcBmp1->imageData+srcBmp1->widthStep*(srcBmp1->height-1-i );
		lpDst = dstBmp->imageData+dstBmp->widthStep*(dstBmp->height-1-i);
		for(j = 0; j < srcBmp->width ; j++)   
        {   
		
			pixel = (*lpSrc) + (*lpSrc1);
			if(pixel > 255)  *lpDst = 255;
			else *lpDst = pixel;
			
			lpSrc ++;
			lpSrc1 ++;
			lpDst  ++;
		
		 }  

		
    }  

	return 0;	
}