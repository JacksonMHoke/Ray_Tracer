#ifndef __PHONG_SHADER_H__
#define __PHONG_SHADER_H__

#include "shader.h"

class Phong_Shader : public Shader
{
public:
    const Color* color_ambient = nullptr;
    const Color* color_diffuse = nullptr;
    const Color* color_specular = nullptr;
    double specular_power = 0;

    Phong_Shader(const Parse* parse,std::istream& in);
    virtual ~Phong_Shader() = default;

    virtual vec3 Shade_Surface(const Render_World& render_world,const Ray& ray,
        const Hit& hit,const vec3& intersection_point,const vec3& normal,
        int recursion_depth) const override;

    static constexpr const char* parse_name = "phong_shader";
};
#endif
