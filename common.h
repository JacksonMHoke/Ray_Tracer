#ifndef __COMMON__
#define __COMMON__
#include <vector>
#include "vec.h"

typedef unsigned int Pixel;

inline vec3 From_Pixel(Pixel color)
{
    return vec3(color>>24,(color>>16)&0xff,(color>>8)&0xff)/255.;
}

inline Pixel Pixel_Color(const vec3& color)
{
    unsigned int r=std::min(color[0],1.0)*255;
    unsigned int g=std::min(color[1],1.0)*255;
    unsigned int b=std::min(color[2],1.0)*255;
    return (r<<24)|(g<<16)|(b<<8)|0xff;
}

inline void set_pixel(Pixel* pixels, int width, int height, int x, int y,
    const vec3& color)
{
    pixels[x+width*y]=Pixel_Color(color);
}

struct Triangle
{
    vec4 A,B,C; // position of vertex
    vec3 Ca,Cb,Cc; // color of vertex
};

void Rasterize(Pixel* pixels, int width, int height, const std::vector<Triangle>& tris);
void Read_png(Pixel*& data,int& width,int& height,const char* filename);
void Dump_png(Pixel* data,int width,int height,const char* filename);


#endif
