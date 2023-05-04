#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "color.h"
#include "vec.h"
#include "misc.h"

class Texture : public Color
{
    Pixel* data; // Texture data; row-major order
    int width,height;
    bool use_bilinear_interpolation;
public:
    Texture(const Parse* parse,std::istream& in);
    virtual ~Texture();

    virtual vec3 Get_Color(const vec2& uv) const;

    static constexpr const char* parse_name = "texture";
};


#endif
