#ifndef IMGENHANCE_H_INCLUDED  
#define IMGENHANCE_H_INCLUDED 


int HistEqualization(Bitmap* dstBmp, Bitmap* srcBmp);
int Linear_trans(Bitmap* dstBmp, Bitmap* srcBmp);
int power_convertion(Bitmap* dstBmp, Bitmap* srcBmp,double para_c,double para_g) ;
int Sobel(Bitmap* dstBmp, Bitmap* srcBmp) ;
int Laplacian(Bitmap* dstBmp, Bitmap* srcBmp) ;
int Smooth_55(Bitmap* dstBmp, Bitmap* srcBmp) ;
int Multiply(Bitmap* dstBmp, Bitmap* srcBmp1,Bitmap* srcBmp);
int Add_img(Bitmap* dstBmp, Bitmap* srcBmp1,Bitmap* srcBmp);
#endif // IMGENHANCE_H_INCLUDED