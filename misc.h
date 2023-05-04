// Student Name: Jackson Hoke
// Student ID: 862161195

#ifndef __MISC_H__
#define __MISC_H__

#include "vec.h"
#include <iostream>

// Prints out a TODO message at most once.
#define TODO {static std::ostream& todo=std::cout<<"TODO: "<<__FUNCTION__<<" in "<<__FILE__<<std::endl;(void)todo;}

typedef unsigned int Pixel;

inline Pixel Pixel_Color(const vec3& color)
{
    unsigned int r=std::min(color[0],1.0)*255;
    unsigned int g=std::min(color[1],1.0)*255;
    unsigned int b=std::min(color[2],1.0)*255;
    return (r<<24)|(g<<16)|(b<<8)|0xff;
}

inline vec3 From_Pixel(Pixel color)
{
    return vec3(color>>24,(color>>16)&0xff,(color>>8)&0xff)/255.;
}

// Useful for creating indentation in pixel traces
struct Debug_Scope
{
    static bool enable;
    static int level;

    Debug_Scope(){level++;}
    ~Debug_Scope(){level--;}
};

// This routine is useful for generating pixel traces.  It only prints when the
// desired pixel is being traced.
template<class... Args>
static void Pixel_Print(Args&&... args)
{
    if(!Debug_Scope::enable) return;
    for(int i=0;i<Debug_Scope::level;i++) std::cout<<"  ";
    (std::cout<<...<<std::forward<Args>(args))<<std::endl;
}

/*
Example usage of these routines:

void foo(const vec3& pt, const Ray& ray)
{
    Debug_Scope scope; 
    Pixel_Print("calling foo; pt: ",pt,"; ray: ",ray);

    blah;
    blah;
    blah;
}
*/



inline int wrap(int i,int n)
{
    int k=i%n;
    if(k<0) k+=n;
    return k;
}

#endif

