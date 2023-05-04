// Student Name: Jackson Hoke
// Student ID: 862161195

#include "plane.h"
#include "hit.h"
#include "ray.h"
#include <cfloat>
#include <limits>

Plane::Plane(const Parse* parse,std::istream& in)
{
    in>>name>>x>>normal;
    normal=normal.normalized();
}

// Intersect with the plane.  The plane's normal points outside.
Hit Plane::Intersection(const Ray& ray, int part) const
{
    Hit rt;
    double denominator = dot(normal, ray.direction);
    if (abs(denominator) <= small_t) {
        return rt;
    }
    double t = dot(x-ray.endpoint, normal)/denominator;
    if (t>small_t) rt.dist=t;
    return rt;
}

vec3 Plane::Normal(const Ray& ray, const Hit& hit) const
{
    return normal;
}

std::pair<Box,bool> Plane::Bounding_Box(int part) const
{
    Box b;
    b.Make_Full();
    return {b,true};
}
