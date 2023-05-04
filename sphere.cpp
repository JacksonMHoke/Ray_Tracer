// Student Name: Jackson Hoke
// Student ID: 862161195

#include "sphere.h"
#include "ray.h"

Sphere::Sphere(const Parse* parse, std::istream& in)
{
    in>>name>>center>>radius;
}

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    double a = 1;
    double b = 2 * dot(ray.direction, ray.endpoint-center);
    double c = dot(ray.endpoint-center, ray.endpoint-center) - radius*radius;
    double discriminant = b*b-4*a*c;

    Hit rt;
    if (discriminant<0) return rt;
    double t1 = (-1*b+sqrt(discriminant))/2;
    double t2 = (-1*b-sqrt(discriminant))/2;
    if (t1 < small_t && t2 < small_t) return rt;
    if (t1 > t2) std::swap(t1, t2);
    if (t1 < small_t) rt.dist=t2;
    else rt.dist=t1;
    return rt;
}

vec3 Sphere::Normal(const Ray& ray, const Hit& hit) const
{
    vec3 normal;
    vec3 intersectPoint = ray.Point(hit.dist);
    normal = (intersectPoint-center).normalized();
    return normal;
}

std::pair<Box,bool> Sphere::Bounding_Box(int part) const
{
    return {{center-radius,center+radius},false};
}
