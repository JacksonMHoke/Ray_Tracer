#include "transparent_shader.h"
#include "parse.h"
#include "ray.h"
#include "render_world.h"
#include "cmath"

Transparent_Shader::
Transparent_Shader(const Parse* parse,std::istream& in)
{
    in>>name>>index_of_refraction>>opacity;
    shader=parse->Get_Shader(in);
    assert(index_of_refraction>=1.0);
}

// Use opacity to determine the contribution of this->shader and the Schlick
// approximation to compute the reflectivity.  This routine shades transparent
// objects such as glass.  Note that the incoming and outgoing indices of
// refraction depend on whether the ray is entering the object or leaving it.
// You may assume that the object is surrounded by air with index of refraction
// 1.
vec3 Transparent_Shader::
Shade_Surface(const Render_World& render_world,const Ray& ray,const Hit& hit,
    const vec3& intersection_point,const vec3& normal,int recursion_depth) const
{
    Debug_Scope scope;

    vec3 dir = ray.direction, nhat=normal.normalized(); //direction of incoming ray, normal unit vector
    vec3 co, cr, ct; //color object, color reflected, color transmitted
    double n1=index_of_refraction,n2=1;
    double R; //Schlick's approximation

    //if exiting
    if (dot(dir, normal)>0) {
        std::swap(n1, n2); //swap refraction indexes
        nhat*=-1; //normal needs to be inverted for calculations to be treated the same as entering
    }

    //calculating object color
    Pixel_Print("call Shade_Surface for Object in Transparent Shader with location ", intersection_point, "; normal: ", normal);
    co = shader->Shade_Surface(render_world, ray, hit, intersection_point, normal, recursion_depth+1);

    //calculating reflected ray color
    vec3 vhat = ray.direction*-1; //v vector pointing towards ray
    vec3 r = (2*dot(nhat, vhat)*nhat)-vhat; //reflected vector
    Ray reflectedray(intersection_point, r);

    Pixel_Print("Casting reflected ray from Transparent Shader");
    cr = render_world.Cast_Ray(reflectedray, recursion_depth+1);
    Pixel_Print("Reflected ray color returned : ", cr);

    //calculating transmitted ray color
    double costheta1 = dot(nhat,vhat);
    double sintheta1 = sqrt(1-pow(costheta1,2));
    vec3 b = ((vhat-costheta1*nhat)/sintheta1).normalized(); //b is perp to normal in same plane that transmitted ray is in

    double sintheta2 = (n2/n1)*sqrt(1-pow(costheta1,2));
    double costheta2 = sqrt(1-pow(sintheta2,2));

    Pixel_Print(costheta1, " ", sintheta1, " ", costheta2, " ", sintheta2);

    //calculating R using Schlicks approx
    R = pow((n2-n1)/(n1+n2),2);
    R = R+(1-R)*pow(1-costheta2,5);

    //if not total internal reflection
    Ray t_ray;
    if (costheta2<=1) {
        vec3 t = -1*costheta2*nhat-sintheta2*b;
        t_ray.endpoint=intersection_point;
        t_ray.direction=t.normalized();
        Pixel_Print("Casting transmitted ray from Transparent Shader");
        ct = render_world.Cast_Ray(t_ray, recursion_depth+1);
    } else {
        ct = vec3();
        Pixel_Print("Total internal reflection occured");
        return cr;
    }
    Pixel_Print("transmitted ray: (end: ", t_ray.endpoint, "; dir: ", t_ray.direction, "); transmitted color: ", ct, "; Schlick reflectivity: ", R, "; combined color: ", opacity*co+(1-opacity)*R*cr+(1-opacity)*(1-R)*ct);
    return opacity*co+(1-opacity)*R*cr+(1-opacity)*(1-R)*ct;
}
