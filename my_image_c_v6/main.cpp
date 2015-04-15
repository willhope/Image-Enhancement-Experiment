#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <process.h>
#include "BMP.h"
#include "imgEnhance.h"

Bitmap* bmp;
Bitmap* hand_bmp;
Bitmap* hand_linear_bmp;
Bitmap* spine_bmp;
Bitmap* moon_bmp;
Bitmap* bone_bmp1;
Bitmap* bone_bmp2;
Bitmap* bone_bmp3;
Bitmap* bone_bmp4;
Bitmap* bone_bmp5;
Bitmap* bone_bmp6;
Bitmap* bone_bmp7;

// 打印选择项目的调用函数
void PrintMenu()
{
			printf(" ---------------------------Choose Your Operation-----------------------------\n");
			printf("| 1-----Histogram Equalization (ex. hand.bmp->hand_list.bmp)                  |\n");
			printf("| 2-----Linear Transformation  (ex. stone.bmp->stone_linear.bmp)              |\n");
			printf("| 3-----power_convertion       (ex. spine.bmp->spine_power.bmp)               |\n");
			printf("| 4-----Laplacian filter Enhancement  (ex. moon.bmp->moon_laplacian.bmp)      |\n");
			printf("| 5-----Bone Combining Spatial Enhancement (ex. bone.bmp->...->bone_final.bmp)|\n");
			printf("| other-----Exit the program                                                  |\n");
			printf(" -----------------------------------------------------------------------------\n");
}




int main()
{
	char filepath[30] = {0} ;         // 要打开的bmp文件路径存储数组
	char filesavepath[30] = {0};
    int ret = 0;    
	int select;
    int q = 0;
	double pow_c = 0;
	double pow_g = 0;
	int threshold = 0;
	system("echo off");
	system("color 2");         // 调整字体颜色
	printf("-----------Image process-----------\n");
	/*printf("Input the path of the BMP file:\n");
	gets(filepath);
	ret=CheckFilePath(filepath);  */ // 测试路径下是否有此文

	bmp = (Bitmap*)malloc(sizeof(Bitmap));
	

	do
	{
		PrintMenu();           // 如果路径合法，则打印相关程序功能选项
		scanf("%u",&select);   // 测试用户选择
		switch(select)
		{
		case 1:
			{
				printf("Histogram Equalization----------\n");
				//strcpy(filepath, "f:/hand.bmp");
				printf("Input the path of the BMP file:\n");
				scanf("%s",filepath);
				ret=CheckFilePath(filepath);
				strcpy(filesavepath, "f:/hand_hist.bmp");
				hand_bmp = (Bitmap*)malloc(sizeof(Bitmap));
				ReadBitmap(filepath,bmp);
				PrintFileInfo(bmp);
				
				/*printf("Input the save path of the BMP file:\n");
				scanf("%s",&filesavepath);*/
				CheckIfBmp(filesavepath);
				HistEqualization(hand_bmp,bmp);
				ShowImage(filepath);
				SaveBitmap(filesavepath,hand_bmp);
				break;
			}
		case 2:
			{
				printf("Piecewise-Linear transform----------\n");
				//strcpy(filepath, "f:/hand.bmp");
				printf("Input the path of the BMP file:\n");
				scanf("%s",filepath);
				ret=CheckFilePath(filepath);
				strcpy(filesavepath, "f:/stone_linear.bmp");
				hand_linear_bmp = (Bitmap*)malloc(sizeof(Bitmap));
				ReadBitmap(filepath,bmp);
				PrintFileInfo(bmp);
				/*printf("Input the save path of the BMP file:\n");
				scanf("%s",&filesavepath);*/
				CheckIfBmp(filesavepath);
				Linear_trans(hand_linear_bmp,bmp);
				ShowImage(filepath);
				SaveBitmap(filesavepath,hand_linear_bmp);
				break;
			}
		case 3:
			{
				printf("power_convertion s = c * r ^ g----------(suggest: c = 1 and g = 0.4)\n");
				printf("Input the path of the BMP file:\n");
				scanf("%s",filepath);
				ret=CheckFilePath(filepath);
				printf("Input c:\n");
				scanf("%lf",&pow_c);
				printf("Input g:\n");
				scanf("%lf",&pow_g);
				
				//strcpy(filepath, "f:/spine.bmp");
				strcpy(filesavepath, "f:/spine_power.bmp");
				spine_bmp = (Bitmap*)malloc(sizeof(Bitmap));
				ReadBitmap(filepath,bmp);
				PrintFileInfo(bmp);
				

				CheckIfBmp(filesavepath);
				power_convertion(spine_bmp,bmp, pow_c ,pow_g);
				ShowImage(filepath);
				SaveBitmap(filesavepath,spine_bmp);
				break;
			}
		case 4:
			{
				printf("Laplacian Image Enhancement \n");
				printf("Input the path of the BMP file:\n");
				scanf("%s",filepath);
				ret=CheckFilePath(filepath);
				//strcpy(filepath, "f:/spine.bmp");
				strcpy(filesavepath, "f:/moon_laplacian.bmp");
				moon_bmp = (Bitmap*)malloc(sizeof(Bitmap));
				ReadBitmap(filepath,bmp);
				PrintFileInfo(bmp);
				

				CheckIfBmp(filesavepath);
				Laplacian(moon_bmp,bmp);
				ShowImage(filepath);
				SaveBitmap(filesavepath,moon_bmp);
				break;
			}
		case 5:
			{
				/*printf("Input the save path of the BMP file:\n");
				scanf("%s",&filesavepath);*/
				printf("Bone Combining Spatial Enhancement----------\n");
				//strcpy(filepath, "f:/bone.bmp");
				printf("Input the path of the BMP file:\n");
				scanf("%s",filepath);
				ret=CheckFilePath(filepath);
				strcpy(filesavepath, "f:/bone_Lapla.bmp");
				bone_bmp1 = (Bitmap*)malloc(sizeof(Bitmap));
				bone_bmp2 = (Bitmap*)malloc(sizeof(Bitmap));
				bone_bmp3 = (Bitmap*)malloc(sizeof(Bitmap));
				bone_bmp4 = (Bitmap*)malloc(sizeof(Bitmap));
				bone_bmp5 = (Bitmap*)malloc(sizeof(Bitmap));
				bone_bmp6 = (Bitmap*)malloc(sizeof(Bitmap));
				bone_bmp7 = (Bitmap*)malloc(sizeof(Bitmap));

				ReadBitmap(filepath,bmp);
				PrintFileInfo(bmp);
				ShowImage(filepath);
				CheckIfBmp(filesavepath);
				
				

				Laplacian(bone_bmp1,bmp);
				strcpy(filesavepath, "f:/bone_lapla.bmp");
				SaveBitmap(filesavepath,bone_bmp1);

				Smooth_55(bone_bmp2,bone_bmp1);
				strcpy(filesavepath, "f:/bone_lapla_smooth.bmp");
				SaveBitmap(filesavepath,bone_bmp2);

				
				strcpy(filesavepath, "f:/bone_sobel_smooth.bmp");
				CheckIfBmp(filesavepath);
				Sobel(bone_bmp4,bmp);
				Smooth_55(bone_bmp3,bone_bmp4);
				SaveBitmap(filesavepath,bone_bmp3);

				strcpy(filesavepath, "f:/bone_Multiply.bmp");
				Multiply(bone_bmp5,bone_bmp3,bone_bmp1);
				SaveBitmap(filesavepath,bone_bmp5);
				
				strcpy(filesavepath, "f:/bone_add.bmp");
				Add_img(bone_bmp6,bone_bmp5,bmp);
				SaveBitmap(filesavepath,bone_bmp6);
				
				strcpy(filesavepath, "f:/bone_fina.bmp");
				power_convertion(bone_bmp7,bone_bmp6, 1 ,0.5);
				SaveBitmap(filesavepath,bone_bmp7);

				break;
				
			}
	/*	case 6:
			{
				break;
			}
		case 7:
			{
				break;	
			}*/
		default:
			q=1;
			break;
		}
		select=0;
	} 
	while (q==0);
	return 0;
}


