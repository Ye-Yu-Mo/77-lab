#ifndef YUV_RAW
#define YUV_RAW

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

//YUV 子采样格式（planar，Y-U-V 连续存放）
#define YUV420P 420
#define YUV422P 422
#define YUV444P 444

/**
 * Convert YUV420P file to  RGB24 file
 * @param url_in  Location of Input YUV file.
 * @param w       Width of Input RGB file.
 * @param h       Height of Input RGB file.
 * @param num     Number of frames to process.
 * @param url_out Location of Output RGB file.
 */

int simplest_yuv420_to_bmp(char *url_in, int w, int h, char *url_out);

/**
 * Convert YUV planar file to BMP file
 * @param url_in  Location of Input YUV file.
 * @param w       Width of Input YUV file.
 * @param h       Height of Input YUV file.
 * @param format  YUV420P / YUV422P / YUV444P
 * @param url_out Location of Output BMP file.
 */
int simplest_yuv_to_bmp(char *url_in, int w, int h, int format, char *url_out);

/**
 * Convert RGB24 file to YUV420P file
 * @param url_in  Location of Input RGB file.
 * @param w       Width of Input RGB file.
 * @param h       Height of Input RGB file.
 * @param num     Number of frames to process.
 * @param url_out Location of Output YUV file.
 */
int simplest_rgb24_to_yuv420(char *url_in, int w, int h, int num, char *url_out);


int simplest_rgb24_to_yuv422(char *url_in, int w, int h, int num, char *url_out);

/**
 * Convert RGB24 file to YUV444 file
 * @param url_in  Location of Input RGB file.
 * @param w       Width of Input RGB file.
 * @param h       Height of Input RGB file.
 * @param num     Number of frames to process.
 * @param url_out Location of Output YUV file.
 */
int simplest_rgb24_to_yuv444(char *url_in, int w, int h, int num, char *url_out);
/**
 * Calculate PSNR between 2 YUV420P file
 * @param url1     Location of first Input YUV file.
 * @param url2     Location of another Input YUV file.
 * @param w        Width of Input YUV file.
 * @param h        Height of Input YUV file.
 * @param num      Number of frames to process.
 */
int simplest_yuv420_psnr(char *url1, char *url2, int w, int h, int num);

#endif // !YUV_RAW
#pragma once
