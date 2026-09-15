/**
 * 最简单的视频YUVRGB数据处理示例
 * Simplest YUVData Test
 *
 *
 * 本项目包含如下几种视音频测试示例：
 *  (1)像素数据处理程序。包含RGB和YUV像素格式处理的函数。
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include"yuv_rgb_raw.h"

int main(int argc, char* argv[]){

	//Test

	

	//simplest_yuv420_to_bmp("../lena_256x256_yuv420p.yuv",256,256,"../output_lena.bmp");

	//simplest_rgb24_to_yuv420("../lena_256x256_rgb24.rgb",256,256,1,"../output_lena.yuv");

	//simplest_rgb24_to_yuv422("../lena_256x256_rgb24.rgb", 256, 256, 1, "../output_lena_422.yuv");

	simplest_yuv420_psnr("../lena_256x256_yuv420p.yuv", "../lena_distort_256x256_yuv420p.yuv", 256, 256, 1);

	return 0;
}