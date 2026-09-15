/**
 * 最简单的视频YUVRGB数据处理示例
 * Simplest YUVData Test
 *
 *
 * 本项目包含如下几种视音频测试示例：
 *  (1)像素数据处理程序。包含RGB和YUV像素格式处理的函数。
 *
 * 实验一：色彩模型转换与YUV子采样
 *  1. RGB24 -> YUV444P / YUV422P / YUV420P
 *  2. YUV -> RGB24 -> BMP 预览
 *  3. 计算 PSNR
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include"yuv_rgb_raw.h"

//样例文件所在目录，按实际运行目录调整
//在 no1_yuv/yuv 下运行（Visual Studio 默认工作目录）时用 "../"
#define SAMPLE_DIR "../"
#define IMG_W 256
#define IMG_H 256

//打印文件大小，用于比较不同子采样的文件体积
static void show_size(const char *path){
	FILE *fp = fopen(path, "rb");
	long size;

	if (fp == NULL){
		printf("  %-28s 打开失败\n", path);
		return;
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fclose(fp);
	printf("  %-28s %ld 字节\n", path, size);
}

int main(int argc, char* argv[]){

	printf("== 1. RGB24 转 YUV（4:4:4 / 4:2:2 / 4:2:0）==\n");
	show_size(SAMPLE_DIR "lena_256x256_rgb24.rgb");

	simplest_rgb24_to_yuv444(SAMPLE_DIR "lena_256x256_rgb24.rgb", IMG_W, IMG_H, 1, SAMPLE_DIR "output_lena_444.yuv");
	show_size(SAMPLE_DIR "output_lena_444.yuv");

	simplest_rgb24_to_yuv422(SAMPLE_DIR "lena_256x256_rgb24.rgb", IMG_W, IMG_H, 1, SAMPLE_DIR "output_lena_422.yuv");
	show_size(SAMPLE_DIR "output_lena_422.yuv");

	simplest_rgb24_to_yuv420(SAMPLE_DIR "lena_256x256_rgb24.rgb", IMG_W, IMG_H, 1, SAMPLE_DIR "output_lena.yuv");
	show_size(SAMPLE_DIR "output_lena.yuv");

	printf("\n== 2. YUV 转 RGB24 并保存为 BMP 预览 ==\n");
	simplest_yuv_to_bmp(SAMPLE_DIR "output_lena_444.yuv", IMG_W, IMG_H, YUV444P, SAMPLE_DIR "output_lena_444.bmp");
	show_size(SAMPLE_DIR "output_lena_444.bmp");

	simplest_yuv_to_bmp(SAMPLE_DIR "output_lena_422.yuv", IMG_W, IMG_H, YUV422P, SAMPLE_DIR "output_lena_422.bmp");
	show_size(SAMPLE_DIR "output_lena_422.bmp");

	simplest_yuv_to_bmp(SAMPLE_DIR "output_lena.yuv", IMG_W, IMG_H, YUV420P, SAMPLE_DIR "output_lena.bmp");
	show_size(SAMPLE_DIR "output_lena.bmp");

	printf("\n== 3. PSNR（lena 原图 与 失真图）==\n");
	printf("  PSNR(Y) = ");
	simplest_yuv420_psnr(SAMPLE_DIR "lena_256x256_yuv420p.yuv", SAMPLE_DIR "lena_distort_256x256_yuv420p.yuv", IMG_W, IMG_H, 1);

	return 0;
}