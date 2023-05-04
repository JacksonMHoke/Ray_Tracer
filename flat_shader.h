#ifndef __FLAT_SHADER_H__
#define __FLAT_SHADER_H__

#include "shader.h"
#include "fixed_color.h"

class Flat_Shader : public Shader
{
public:
    const Color* color = nullptr;

    Flat_Shader(const Parse* parse,std::istream& in);
    virtual ~Flat_Shader() = default;
    
    virtual vec3 Shade_Surface(const Render_World& render_world,const Ray& ray,
        const Hit& hit,const vec3& intersection_point,const vec3& normal,
        int recursion_depth) const override;
    
    static constexpr const char* parse_name = "flat_shader";
};
#endif
