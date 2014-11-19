#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avcodec.h"
#include "avformat.h"
#include "swscale.h"
#include <jni.h>
#include <android/log.h>


typedef struct{
	char M,N;
	int x;
	int y;
	int max;
	uint8_t *data;
}PPMImage;

PPMImage *getPPM(char *flname)
{
	FILE *fp;
	int i, j, size;
	PPMImage *img = NULL;
	char temp[255];

	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	if(flname == NULL){
		printf("no find filename.\n");
		__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
		return NULL;
	}

	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	fp = fopen(flname,"rb");
	if(fp==NULL){
		printf("no find PPM file.\n");
		__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
		return NULL;
	}

	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	img = (PPMImage *) malloc(sizeof(PPMImage));

	fscanf(fp,"%c%c\n", &img->M, &img->N);
	if(img->M != 'P' || img->N != '6'){
		printf("It's not PPM Image Format.\n");
		__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
		__android_log_print(ANDROID_LOG_INFO, "LOG", "%c%c", img->M, img->N);
		free(img);
		return NULL;
	}

	fgets(temp,255,fp);
	fscanf(fp,"%d%d\n", &img->x, &img->y);
	fscanf(fp,"%d\n", &img->max);
	if(img->max != 255){
		printf("wrong image format.\n");
		__android_log_print(ANDROID_LOG_INFO, "LOG", "%s,%d", __FUNCTION__, __LINE__);
		__android_log_print(ANDROID_LOG_INFO, "LOG", "%d,%d", img->x ,img->y);
		__android_log_print(ANDROID_LOG_INFO, "LOG", "%d", img->max);
		free(img);
		return NULL;
	}
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	size = (img->y * img->x * 3)*sizeof(uint8_t);
	img->data = (uint8_t *)malloc(size);
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	fread(&img->data, size, 1, fp);
	fclose(fp);
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	return img;
}

jint Java_com_example_ffmpegbasic_MoviePlayView_encoderVideo(JNIEnv* env, jobject javaThis)
{
	char			*flname, *err, *info;
	AVCodec			*codec;
	AVCodecContext	*c = NULL;
	int				i,out_size,size,x,y,z,outbuf_size;
	int				frameCount = 99;
	FILE			*f;
	AVFrame			*picture, *yuvFrame;
	uint8_t			*outbuf, *picture_buf;
	PPMImage		*img;
	const char		*destfilename = "/mnt/sdcard/new.mp4";
	int				numBytes;
	uint8_t			*buffer;

	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	av_register_all();

	avcodec_init();
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	avcodec_register_all();
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	codec = avcodec_find_encoder(CODEC_ID_H263);
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	if(!codec){
//		sprintf(err, "codec not found.\n");
//		log_message(err);
		__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	}
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	c = avcodec_alloc_context();
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	picture = avcodec_alloc_frame();
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	yuvFrame = avcodec_alloc_frame();
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	img = getPPM("/mnt/sdcard/ppm/stop01.ppm");
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	c->bit_rate = 400000;
	c->width = img->x;
	c->height = img->y;
	free(img);
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	c->time_base = (AVRational){1,25};
	c->gop_size = 10;
	c->pix_fmt = PIX_FMT_YUV420P;

	if(avcodec_open(c,codec)<0){
		printf(err, "could not open %s", destfilename);
		__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	}
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
	outbuf_size = 100000;
	outbuf = malloc(outbuf_size);
	size = c->width * c->height;
	picture_buf = malloc(size*3);
	picture->data[0] = picture_buf;
	picture->data[1] = picture->data[0] + size;
	picture->data[2] = picture->data[1] + size/4;
	picture->linesize[0] = c->width;
	picture->linesize[1] = c->width/2;
	picture->linesize[2] = c->width/3;

	numBytes = avpicture_get_size(PIX_FMT_YUV420P, c->width, c->height);
	buffer = malloc(numBytes);

	avpicture_fill((AVPicture *)yuvFrame, buffer, PIX_FMT_YUV420P, c->width, c->height);

	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);

	for(z=1; z<frameCount; z++){
		sprintf(flname, "/mnt/sdcard/ppm/stop0%d.ppm",z);
		img = getPPM(flname);
		picture->data[0] = img->data;
		rgb2yuv(picture, yuvFrame, c);
		out_size = avcodec_encode_video(c, outbuf, outbuf_size, yuvFrame);
		printf(info, "encoding frame %3d (size=%5d)\n", z, out_size);
		fwrite(outbuf, 1, out_size, f);
		free(img);
	}
	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);

	for(; out_size; i++){
		out_size = avcodec_encode_video(c, outbuf, outbuf_size, NULL);
		printf(info, "write frame %3d (size=%5d)\n",i, out_size);
		fwrite(outbuf, 1, out_size, f);
	}

	outbuf[0] = 0x00;
	outbuf[1] = 0x00;
	outbuf[2] = 0x01;
	outbuf[3] = 0xb7;
	fwrite(outbuf, 1, 4, f);
	fclose(f);
	free(picture_buf);
	free(outbuf);

	avcodec_close(c);

	av_free(c);
	av_free(picture);
	av_free(yuvFrame);

}

int rgb2yuv(AVFrame *frameRGB, AVFrame *frameYUV, AVCodecContext *c)
{
	char *err;
	static struct SwsContext *img_convert_ctx;

	if(img_convert_ctx = NULL){
		int w = c->width;
		int h = c->height;

		img_convert_ctx = sws_getContext(w, h, PIX_FMT_RGB24, w, h, c->pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);

		if(img_convert_ctx == NULL){
			sprintf(err, "Cannot initialize the conversion context!\n");
			__android_log_print(ANDROID_LOG_INFO, "LOG", "%s%d", __FUNCTION__, __LINE__);
//			log_message(err);
			return -1;
		}
	}

	int ret = sws_scale(img_convert_ctx, frameRGB->data, frameRGB->linesize, 0,
			c->height, frameYUV->data, frameYUV->linesize);

	return;
}
