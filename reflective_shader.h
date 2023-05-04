#ifndef __REFLECTIVE_SHADER_H__
#define __REFLECTIVE_SHADER_H__

#include <algorithm>
#include "shader.h"

class Reflective_Shader : public Shader
{
public:
    const Shader* shader = nullptr;
    double reflectivity;
    
    Reflective_Shader(const Parse* parse,std::istream& in);
    virtual ~Reflective_Shader() = default;
    
    virtual vec3 Shade_Surface(const Render_World& render_world,const Ray& ray,
        const Hit& hit,const vec3& intersection_point,const vec3& normal,
        int recursion_depth) const override;

    static constexpr const char* parse_name = "reflective_shader";
};
#endif
