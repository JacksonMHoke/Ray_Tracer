#ifndef __MESH_H__
#define __MESH_H__

#include "object.h"

// Consider a hit to be inside a triange if all barycentric weights
// satisfy weight>=-weight_tol
static const double weight_tol = 1e-4;

class Parse;

class Mesh : public Object
{
    std::vector<vec3> vertices;
    std::vector<ivec3> triangles;
    std::vector<vec2> uvs; // indexed texture coordinates
    std::vector<ivec3> triangle_texture_index; // triangle index -> texture coordinate indices


public:
    Mesh(const Parse* parse,std::istream& in);
    virtual ~Mesh() = default;

    virtual Hit Intersection(const Ray& ray, int part) const override;
    virtual vec3 Normal(const Ray& ray, const Hit& hit) const override;
    virtual std::pair<Box,bool> Bounding_Box(int part) const override;

    static constexpr const char* parse_name = "mesh";

private:
    Hit Intersect_Triangle(const Ray& ray, int tri) const;
    void Read_Obj(const char* file);
};
#endif
