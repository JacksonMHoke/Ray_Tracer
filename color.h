#ifndef __COLOR_H__
#define __COLOR_H__

struct Hit;

class Color
{
public:
    std::string name;

    virtual ~Color()=default;
    virtual vec3 Get_Color(const vec2& uv) const=0;
};


#endif
