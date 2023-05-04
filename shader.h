#ifndef __SHADER_H__
#define __SHADER_H__

#include "vec.h"
class Render_World;
class Ray;
class Parse;
struct Hit;

class Shader
{
public:
    std::string name;

    Shader() = default;
    virtual ~Shader() = default;

    virtual vec3 Shade_Surface(const Render_World& render_world,const Ray& ray,
        const Hit& hit,const vec3& intersection_point,const vec3& normal,
        int recursion_depth) const=0;
};
#endif
