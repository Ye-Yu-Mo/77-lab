#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include"yuv_rgb_raw.h"

unsigned char clip_value(unsigned char x, unsigned char min_val, unsigned char  max_val) {
	if (x > max_val) {
		return max_val;
	}
	else if (x < min_val) {
		return min_val;
	}
	else {
		return x;
	}
}

//RGB to YUV420
//每个 2x2 像素块共用一组色度，U、V 分别写入自己的平面
bool RGB24_TO_YUV420(unsigned char *RgbBuf,int w,int h,unsigned char *yuvBuf)
{
	unsigned char*ptrY, *ptrU, *ptrV, *ptrRGB;
	memset(yuvBuf,0,w*h*3/2);
	ptrY = yuvBuf;
	ptrU = yuvBuf + w*h;
	ptrV = ptrU + (w*h*1/4);
	unsigned char y, u, v, r, g, b;
	for (int j = 0; j<h;j++){
		ptrRGB = RgbBuf + w*j*3 ;
		for (int i = 0;i<w;i++){
			r = *(ptrRGB++);
			g = *(ptrRGB++);
			b = *(ptrRGB++);
			y = (unsigned char)( ( 66 * r + 129 * g +  25 * b + 128) >> 8) + 16  ;
			u = (unsigned char)( ( -38 * r -  74 * g + 112 * b + 128) >> 8) + 128 ;
			v = (unsigned char)( ( 112 * r -  94 * g -  18 * b + 128) >> 8) + 128 ;
			*(ptrY++) = clip_value(y,0,255);
			if (j%2==0 && i%2==0){
				*(ptrU++) = clip_value(u,0,255);
				*(ptrV++) = clip_value(v,0,255);
			}
		}
	}
	return true;
}

//RGB to YUV444
//每个像素都有独立的 Y、U、V，不做子采样
bool RGB24_TO_YUV444(unsigned char *RgbBuf,int w,int h,unsigned char *yuvBuf)
{
	unsigned char*ptrY, *ptrU, *ptrV, *ptrRGB;
	memset(yuvBuf,0,w*h*3);
	ptrY = yuvBuf;
	ptrU = yuvBuf + w*h;
	ptrV = ptrU + w*h;
	unsigned char y, u, v, r, g, b;
	for (int j = 0; j<h;j++){
		ptrRGB = RgbBuf + w*j*3 ;
		for (int i = 0;i<w;i++){
			r = *(ptrRGB++);
			g = *(ptrRGB++);
			b = *(ptrRGB++);
			y = (unsigned char)( ( 66 * r + 129 * g +  25 * b + 128) >> 8) + 16  ;
			u = (unsigned char)( ( -38 * r -  74 * g + 112 * b + 128) >> 8) + 128 ;
			v = (unsigned char)( ( 112 * r -  94 * g -  18 * b + 128) >> 8) + 128 ;
			*(ptrY++) = clip_value(y,0,255);
			*(ptrU++) = clip_value(u,0,255);
			*(ptrV++) = clip_value(v,0,255);
		}
	}
	return true;
}

//RGB to YUV422
bool RGB24_TO_YUV422(unsigned char *RgbBuf, int w, int h, unsigned char *yuvBuf)
{
	unsigned char*ptrY, *ptrU, *ptrV, *ptrRGB;
	memset(yuvBuf, 0, w*h * 2);
	ptrY = yuvBuf;
	ptrU = yuvBuf + w * h;
	ptrV = ptrU + (w*h * 1 / 2);
	unsigned char y, u, v, r, g, b;
	for (int j = 0; j < h; j++) {
		ptrRGB = RgbBuf + w * j * 3;
		for (int i = 0; i < w; i++) {
			r = *(ptrRGB++);
			g = *(ptrRGB++);
			b = *(ptrRGB++);
			y = (unsigned char)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
			u = (unsigned char)((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
			v = (unsigned char)((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;
			*(ptrY++) = clip_value(y, 0, 255);
			if (i % 2 == 0) {
				*(ptrU++) = clip_value(u, 0, 255);
				*(ptrV++) = clip_value(v, 0, 255);
			}
		}
	}
	return true;
}

/**
 * Convert RGB24 file to YUV422 file
 * @param url_in  Location of Input RGB file.
 * @param w       Width of Input RGB file.
 * @param h       Height of Input RGB file.
 * @param num     Number of frames to process.
 * @param url_out Location of Output YUV file.
 */
int simplest_rgb24_to_yuv422(char *url_in, int w, int h, int num, char *url_out) {
	FILE *fp = fopen(url_in, "rb+");
	FILE *fp1 = fopen(url_out, "wb+");

	unsigned char *pic_rgb24 = (unsigned char *)malloc(w*h * 3);
	unsigned char *pic_yuv422 = (unsigned char *)malloc(w*h * 2);

	if (fp==NULL || fp1==NULL || pic_rgb24==NULL || pic_yuv422==NULL){
		printf("Error: cannot open %s\n",url_in);
		return -1;
	}

	for (int i = 0; i < num; i++) {
		if (fread(pic_rgb24, 1, w*h * 3, fp) != (size_t)(w*h * 3)){
			printf("Error: %s 数据不足\n",url_in);
			break;
		}
		RGB24_TO_YUV422(pic_rgb24, w, h, pic_yuv422);
		fwrite(pic_yuv422, 1, w*h * 2, fp1);
	}

	free(pic_rgb24);
	free(pic_yuv422);
	fclose(fp);
	fclose(fp1);

	return 0;
}

/**
 * Convert RGB24 file to YUV444 file
 * @param url_in  Location of Input RGB file.
 * @param w       Width of Input RGB file.
 * @param h       Height of Input RGB file.
 * @param num     Number of frames to process.
 * @param url_out Location of Output YUV file.
 */
int simplest_rgb24_to_yuv444(char *url_in, int w, int h, int num, char *url_out) {
	FILE *fp = fopen(url_in, "rb+");
	FILE *fp1 = fopen(url_out, "wb+");

	unsigned char *pic_rgb24 = (unsigned char *)malloc(w*h * 3);
	unsigned char *pic_yuv444 = (unsigned char *)malloc(w*h * 3);

	if (fp==NULL || fp1==NULL || pic_rgb24==NULL || pic_yuv444==NULL){
		printf("Error: cannot open %s\n",url_in);
		return -1;
	}

	for (int i = 0; i < num; i++) {
		if (fread(pic_rgb24, 1, w*h * 3, fp) != (size_t)(w*h * 3)){
			printf("Error: %s 数据不足\n",url_in);
			break;
		}
		RGB24_TO_YUV444(pic_rgb24, w, h, pic_yuv444);
		fwrite(pic_yuv444, 1, w*h * 3, fp1);
	}

	free(pic_rgb24);
	free(pic_yuv444);
	fclose(fp);
	fclose(fp1);

	return 0;
}

////////////////////////////////////////////////////////////////////////////

static long int crv_tab[256];   
static long int cbu_tab[256];   
static long int cgu_tab[256];   
static long int cgv_tab[256];   
static long int tab_76309[256]; 
static unsigned char clp[1024];   //for clip in CCIR601   

void init_yuv420p_table() 
{   
    long int crv,cbu,cgu,cgv;   
    int i,ind;      
   
    crv = 104597; cbu = 132201;  /* fra matrise i global.h */   
    cgu = 25675;  cgv = 53279;   
   
    for (i = 0; i < 256; i++)    
    {   
        crv_tab[i] = (i-128) * crv;   
        cbu_tab[i] = (i-128) * cbu;   
        cgu_tab[i] = (i-128) * cgu;   
        cgv_tab[i] = (i-128) * cgv;   
        tab_76309[i] = 76309*(i-16);   
    }   
   
    for (i = 0; i < 384; i++)   
        clp[i] = 0;   
    ind = 384;   
    for (i = 0;i < 256; i++)   
        clp[ind++] = i;   
    ind = 640;   
    for (i = 0;i < 384; i++)   
        clp[ind++] = 255;   
}

/**
内存分布
                    w
            +--------------------+
            |Y0Y1Y2Y3...         |
            |...                 |   h
            |...                 |
            |                    |
            +--------------------+
            |U0U1      |
            |...       |   h/2
            |...       |
            |          |
            +----------+
            |V0V1      |
            |...       |  h/2
            |...       |
            |          |
            +----------+
                w/2
 */

//YUV planar 转 RGB24，色度按格式做最近邻扩展
//format: YUV420P / YUV422P / YUV444P
void yuvp_to_rgb24(unsigned char* yuvbuf, unsigned char* rgbbuf, int width, int height, int format)
{
    int u, v;
    int i, j, c1, c2, c3, c4;
    unsigned char *src_u, *src_v;
    unsigned char *d;
    static int init_yuv420p = 0;

    int uv_w = width, uv_h = height;   //色度平面尺寸
    if (format == YUV422P) {
        uv_w = width / 2;
    }
    else if (format == YUV420P) {
        uv_w = width / 2;
        uv_h = height / 2;
    }

    src_u = yuvbuf + width * height;          // u
    src_v = src_u + uv_w * uv_h;              // v

    if (init_yuv420p == 0)
    {
        init_yuv420p_table();
        init_yuv420p = 1;
    }

    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            int uv_pos;
            if (format == YUV444P) {
                uv_pos = j * uv_w + i;
            }
            else if (format == YUV422P) {
                uv_pos = j * uv_w + i / 2;
            }
            else {
                uv_pos = (j / 2) * uv_w + i / 2;
            }

            u = src_u[uv_pos];
            v = src_v[uv_pos];

            c1 = crv_tab[v];
            c2 = cgu_tab[u];
            c3 = cgv_tab[v];
            c4 = cbu_tab[u];

            d = rgbbuf + (j * width + i) * 3;
            int y1 = tab_76309[yuvbuf[j * width + i]];
            *d++ = clp[384 + ((y1 + c1) >> 16)];
            *d++ = clp[384 + ((y1 - c2 - c3) >> 16)];
            *d   = clp[384 + ((y1 + c4) >> 16)];
        }
    }
}

void yuv420p_to_rgb24(unsigned char* yuvbuf,unsigned char* rgbbuf, int width,int height)
{
    yuvp_to_rgb24(yuvbuf, rgbbuf, width, height, YUV420P);
}

/**
 * Convert RGB24 file to YUV420P file
 * @param url_in  Location of Input RGB file.
 * @param w       Width of Input RGB file.
 * @param h       Height of Input RGB file.
 * @param num     Number of frames to process.
 * @param url_out Location of Output YUV file.
 */

int simplest_rgb24_to_yuv420(char *url_in, int w, int h,int num,char *url_out){
	FILE *fp=fopen(url_in,"rb+");
	FILE *fp1=fopen(url_out,"wb+");

	unsigned char *pic_rgb24=(unsigned char *)malloc(w*h*3);
	unsigned char *pic_yuv420=(unsigned char *)malloc(w*h*3/2);

	if(fp==NULL||fp1==NULL||pic_rgb24==NULL||pic_yuv420==NULL){
		printf("Error: cannot open %s\n",url_in);
		return -1;
	}

	for(int i=0;i<num;i++){
		if(fread(pic_rgb24,1,w*h*3,fp)!=(size_t)(w*h*3)){
			printf("Error: %s 数据不足\n",url_in);
			break;
		}
		RGB24_TO_YUV420(pic_rgb24,w,h,pic_yuv420);
		fwrite(pic_yuv420,1,w*h*3/2,fp1);
	}

	free(pic_rgb24);
	free(pic_yuv420);
	fclose(fp);
	fclose(fp1);

	return 0;
}

/**
 * Convert RGB24 file to BMP file
 * @param rgb24path    Location of input RGB file.
 * @param width        Width of input RGB file.
 * @param height       Height of input RGB file.
 * @param url_out      Location of Output BMP file.
 */
int simplest_rgb24_to_bmp(unsigned char *rgb24buf,int width,int height,const char *bmppath){
	typedef struct
	{
		int imageSize;
		int blank;
		int startPosition;
	}BmpHead;

	typedef struct
	{
		int  Length;
		int  width;
		int  height;
		unsigned short  colorPlane;
		unsigned short  bitColor;
		int  zipFormat;
		int  realSize;
		int  xPels;
		int  yPels;
		int  colorUse;
		int  colorImportant;
	}InfoHead;

	int i=0,j=0;
	BmpHead m_BMPHeader={0};
	InfoHead  m_BMPInfoHeader={0};
	char bfType[2]={'B','M'};
	int header_size=sizeof(bfType)+sizeof(BmpHead)+sizeof(InfoHead);
    FILE *fp_bmp=NULL;
	/*
    unsigned char *rgb24_buffer=NULL;
	FILE *fp_rgb24=NULL,*fp_bmp=NULL;

	if((fp_rgb24=fopen(rgb24path,"rb"))==NULL){
		printf("Error: Cannot open input RGB24 file.\n");
		return -1;
	}
    */
	if((fp_bmp=fopen(bmppath,"wb"))==NULL){
		printf("Error: Cannot open output BMP file.\n");
		return -1;
	}
    /*
	rgb24_buffer=(unsigned char *)malloc(width*height*3);
	fread(rgb24_buffer,1,width*height*3,fp_rgb24);
    */

	m_BMPHeader.imageSize=3*width*height+header_size;
	m_BMPHeader.startPosition=header_size;

	m_BMPInfoHeader.Length=sizeof(InfoHead); 
	m_BMPInfoHeader.width=width;
	//BMP storage pixel data in opposite direction of Y-axis (from bottom to top).
	m_BMPInfoHeader.height=-height;
	m_BMPInfoHeader.colorPlane=1;
	m_BMPInfoHeader.bitColor=24;
	m_BMPInfoHeader.realSize=3*width*height;

	fwrite(bfType,1,sizeof(bfType),fp_bmp);
	fwrite(&m_BMPHeader,1,sizeof(m_BMPHeader),fp_bmp);
	fwrite(&m_BMPInfoHeader,1,sizeof(m_BMPInfoHeader),fp_bmp);

	//BMP save R1|G1|B1,R2|G2|B2 as B1|G1|R1,B2|G2|R2
	//It saves pixel data in Little Endian
	//So we change 'R' and 'B'
	for(j =0;j<height;j++){
		for(i=0;i<width;i++){
			char temp=rgb24buf[(j*width+i)*3+2];
			rgb24buf[(j*width+i)*3+2]=rgb24buf[(j*width+i)*3+0];
			rgb24buf[(j*width+i)*3+0]=temp;
		}
	}
	fwrite(rgb24buf,3*width*height,1,fp_bmp);
	fclose(fp_bmp);
	//free(rgb24_buffer);
	printf("Finish generate %s!\n",bmppath);
	return 0;
}
/**
 * Convert YUV420P file to  RGB24 file
 * @param url_in  Location of Input YUV file.
 * @param w       Width of Input RGB file.
 * @param h       Height of Input RGB file.
 * @param num     Number of frames to process.
 * @param url_out Location of Output RGB file.
 */
int simplest_yuv_to_bmp(char *url_in, int w, int h, int format, char *url_out){
	FILE *fp=fopen(url_in,"rb+");

	unsigned char *pic_rgb24=(unsigned char *)malloc(w*h*3);
	unsigned char *pic_yuv=(unsigned char *)malloc(w*h*3);

	if (fp==NULL || pic_rgb24==NULL || pic_yuv==NULL){
		printf("Error: cannot open %s\n",url_in);
		return -1;
	}

	//按格式读取原始数据：420->1.5 字节/像素，422->2，444->3
	int frame_bytes = format==YUV420P ? w*h*3/2 : (format==YUV422P ? w*h*2 : w*h*3);
	if (fread(pic_yuv,1,frame_bytes,fp) != (size_t)frame_bytes){
		printf("Error: %s 数据不足，需要 %d 字节\n",url_in,frame_bytes);
		free(pic_rgb24);
		free(pic_yuv);
		fclose(fp);
		return -1;
	}

	yuvp_to_rgb24(pic_yuv,pic_rgb24,w,h,format);
	simplest_rgb24_to_bmp(pic_rgb24,w,h, url_out);

	free(pic_rgb24);
	free(pic_yuv);
	fclose(fp);

	return 0;
}

int simplest_yuv420_to_bmp(char *url_in, int w, int h,char *url_out){
	return simplest_yuv_to_bmp(url_in, w, h, YUV420P, url_out);
}

/**
 * Calculate PSNR between 2 YUV420P file
 * @param url1     Location of first Input YUV file.
 * @param url2     Location of another Input YUV file.
 * @param w        Width of Input YUV file.
 * @param h        Height of Input YUV file.
 * @param num      Number of frames to process.
 */

int simplest_yuv420_psnr(char *url1,char *url2,int w,int h,int num){
	FILE *fp1=fopen(url1,"rb+");
	FILE *fp2=fopen(url2,"rb+");
	unsigned char *pic1=(unsigned char *)malloc(w*h);
	unsigned char *pic2=(unsigned char *)malloc(w*h);

	for(int i=0;i<num;i++){
		fread(pic1,1,w*h,fp1);
		fread(pic2,1,w*h,fp2);

		double mse_sum=0,mse=0,psnr=0;
		for(int j=0;j<w*h;j++){
			mse_sum+=pow((double)(pic1[j]-pic2[j]),2);
		}
		mse=mse_sum/(w*h);
		psnr=10*log10(255.0*255.0/mse);
		printf("%5.3f\n",psnr);

		fseek(fp1,w*h/2,SEEK_CUR);
		fseek(fp2,w*h/2,SEEK_CUR);

	}

	free(pic1);
	free(pic2);
	fclose(fp1);
	fclose(fp2);
	return 0;
}