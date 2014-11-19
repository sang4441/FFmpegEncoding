#ifndef __BASICPLAYER_H__
#define __BASICPLAYER_H__

int openMovie(const char filePath[]);
int decodeFrame();
void copyPixels(uint8_t *pixels);
int getWidth();
int getHeight();
void closeMovie();

#endif
