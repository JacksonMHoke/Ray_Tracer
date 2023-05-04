#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "object.h"

class Parse;

class Sphere : public Object
{
    vec3 center;
    double radius;

public:
    Sphere(const Parse* parse,std::istream& in);
    virtual ~Sphere() = default;

    virtual Hit Intersection(const Ray& ray, int part) const override;
    virtual vec3 Normal(const Ray& ray, const Hit& hit) const override;
    virtual std::pair<Box,bool> Bounding_Box(int part) const override;

    static constexpr const char* parse_name = "sphere";
};
#endif
