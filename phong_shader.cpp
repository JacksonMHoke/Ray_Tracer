#include "light.h"
#include "parse.h"
#include "object.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"

Phong_Shader::Phong_Shader(const Parse* parse,std::istream& in)
{
    in>>name;
    color_ambient=parse->Get_Color(in);
    color_diffuse=parse->Get_Color(in);
    color_specular=parse->Get_Color(in);
    in>>specular_power;
}

vec3 Phong_Shader::
Shade_Surface(const Render_World& render_world,const Ray& ray,const Hit& hit,
    const vec3& intersection_point,const vec3& normal,int recursion_depth) const
{
    Debug_Scope scope;
    vec3 color;
    
    for (const auto source : render_world.lights) {
        vec3 diffuse;
        vec3 spec;
        vec3 ambient;
        vec3 l = source->position - intersection_point;

        vec3 lhat = l.normalized();
        vec3 nhat = normal.normalized();

        vec3 r = (2*dot(nhat, lhat)*nhat)-lhat;
        vec3 rhat = r.normalized();

        vec3 v = render_world.camera.position - intersection_point;
        vec3 vhat = v.normalized();

        Ray light;
        light.endpoint = intersection_point;
        light.direction = lhat;
        Pixel_Print("Seeing if light: ", source->name, " is interrupted by another object");
        std::pair<Shaded_Object, Hit> inter = render_world.Closest_Intersection(light);
        if (render_world.enable_shadows && inter.second.dist>-1 && inter.second.dist<=l.magnitude()) {
            Pixel_Print("Light: ", source->name, " is interrupted by another object");
            continue;
        }
        Pixel_Print("Light: ", source->name, " is not interrupted by another object");
        //diffusion
        auto dotprod = dot(lhat, nhat);
        //only include diffuse/spec terms if diffuse dot product is positive
        if (dotprod>0) {
            diffuse = source->Emitted_Light(l)*dotprod*color_diffuse->Get_Color(hit.uv);
            dotprod = dot(rhat, vhat);
            if (dotprod>0) spec = source->Emitted_Light(l)*pow(dotprod, specular_power)*color_specular->Get_Color(hit.uv);
        }
        Pixel_Print("Shading for light: ", source->name, " diffuse: ", diffuse, " spec: ", spec);
        color += diffuse+spec;
    }
    if (render_world.ambient_color && render_world.ambient_intensity) color += render_world.ambient_color->Get_Color(hit.uv)*render_world.ambient_intensity*color_ambient->Get_Color(hit.uv);
    else color += color_ambient->Get_Color(hit.uv)*render_world.ambient_intensity;

    Pixel_Print("Phong Shading final color: ", color);
    return color;
}
