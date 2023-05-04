// Student Name: Jackson Hoke
// Student ID: 862161195

#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

extern bool enable_acceleration;

Render_World::~Render_World()
{
    for(auto a:all_objects) delete a;
    for(auto a:all_shaders) delete a;
    for(auto a:all_colors) delete a;
    for(auto a:lights) delete a;
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
std::pair<Shaded_Object,Hit> Render_World::Closest_Intersection(const Ray& ray) const
{
    double min_t = std::numeric_limits<double>::max();
    Shaded_Object obj;
    Hit closestHit;
    Debug_Scope scope;
    for (auto o : objects) {
        Hit hit = o.object->Intersection(ray, -1);
        if (hit.dist<small_t) Pixel_Print("no intersection with ", o.object->name, ", hit distance: ", hit.dist);
        else Pixel_Print("intersect test with ", o.object->name, "; hit: ", hit);
        if (hit.dist>small_t && hit.dist<min_t) {
            min_t = hit.dist;
            obj = o;
            closestHit = hit;
        }
    }
    if (obj.object!=nullptr) Pixel_Print("closest intersection; obj: ", obj.object->name, "; hit: ", closestHit);
    else Pixel_Print("no intersection found");
    return {obj, closestHit};
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    // set up the initial view ray here
    vec3 end_point=camera.position;
    vec3 pos=camera.World_Position(pixel_index);
    vec3 dir=(pos-end_point).normalized();

    Ray ray;
    ray.endpoint=end_point;
    ray.direction=dir;
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth) const
{
    if (recursion_depth>recursion_depth_limit) {
        Pixel_Print("Recursion depth maxed out, returning black");
        return vec3();
    }
    vec3 color;
    // determine the color here
    Debug_Scope scope;
    Pixel_Print("cast ray (end: ", ray.endpoint, "); dir: ", ray.direction, ")");
    std::pair<Shaded_Object, Hit> closestIntersection = Closest_Intersection(ray);
    Hit hit = closestIntersection.second;
    const Object* obj = closestIntersection.first.object;
    const Shader* shad = closestIntersection.first.shader;
    if (hit.dist<0) {
        if (background_shader==nullptr) {
            Pixel_Print("no background shader, using color: (0, 0, 0)");
            color.make_zero();
            return color;
        } else {
            //use background shader
            vec3 tempVec;
            Pixel_Print("call background shader with location ", tempVec, "; normal: ", tempVec);
            color = background_shader->Shade_Surface(*this, ray, hit, tempVec, tempVec, recursion_depth);
            return color;
        }
    }
    vec3 normal = obj->Normal(ray, hit);
    vec3 inter_point = ray.Point(hit.dist);
    Pixel_Print("call Shade_Surface with location ", inter_point, "; normal: ", normal);
    color = shad->Shade_Surface(*this, ray, hit, inter_point, normal, recursion_depth);
    return color;
}
