#include "reflective_shader.h"
#include "parse.h"
#include "ray.h"
#include "render_world.h"

Reflective_Shader::Reflective_Shader(const Parse* parse,std::istream& in)
{
    in>>name;
    shader=parse->Get_Shader(in);
    in>>reflectivity;
    reflectivity=std::max(0.0,std::min(1.0,reflectivity));
}

vec3 Reflective_Shader::
Shade_Surface(const Render_World& render_world,const Ray& ray,const Hit& hit,
    const vec3& intersection_point,const vec3& normal,int recursion_depth) const
{
    Debug_Scope scope;
    vec3 co = shader->Shade_Surface(render_world, ray, hit, intersection_point, normal, recursion_depth+1);

    vec3 nhat = normal.normalized();
    vec3 vhat = ray.direction;
    vec3 r = (2*dot(nhat, vhat)*nhat)-vhat;

    Ray newray(intersection_point, r*-1);
    vec3 cr = render_world.Cast_Ray(newray, recursion_depth+1);
    Pixel_Print("reflected ray: end: ", newray.endpoint, "; dir: ", newray.direction, "; reflected color: ", cr, "; object color: ", co, "; final color: ", (1-reflectivity)*co+reflectivity*cr, "; current recursion depth: ", recursion_depth);
    return (1-reflectivity)*co+reflectivity*cr;
}
