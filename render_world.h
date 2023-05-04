#ifndef __RENDER_WORLD_H__
#define __RENDER_WORLD_H__

#include <vector>
#include <utility>
#include "camera.h"
#include "object.h"
// #include "acceleration.h"

class Light;
class Shader;
class Ray;
class Color;

struct Shaded_Object
{
    const Object* object = nullptr;
    const Shader* shader = nullptr;
};

class Render_World
{
public:
    Camera camera;

    // This is the background shader that you should use in case no other
    // objects are intersected.  If this pointer is null, then use black as the
    // color instead.
    const Shader* background_shader = nullptr;

    // Use these to get access to objects and lights in the scene.
    std::vector<Shaded_Object> objects;
    std::vector<const Light*> lights;

    // Store pointers to these for deallocation.  You should not use these
    // directly.  Use the objects array above instead.
    std::vector<Object*> all_objects;
    std::vector<Shader*> all_shaders;
    std::vector<Color*> all_colors;
    
    const Color* ambient_color = nullptr;
    double ambient_intensity = 0;

    bool enable_shadows = true;
    int recursion_depth_limit = 3;

//     Acceleration acceleration;

    Render_World() = default;
    ~Render_World();

    void Render_Pixel(const ivec2& pixel_index);
    void Render();

    vec3 Cast_Ray(const Ray& ray,int recursion_depth) const;
    std::pair<Shaded_Object,Hit> Closest_Intersection(const Ray& ray) const;
};
#endif
