#ifndef __DUMP_PNG_H
#define __DUMP_PNG_H

#include "misc.h"

void Dump_png(Pixel* data,int width,int height,const char* filename);
void Read_png(Pixel*& data,int& width,int& height,const char* filename);

#endif
