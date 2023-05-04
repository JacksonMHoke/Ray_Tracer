#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "box.h"
#include "hit.h"
#include "vec.h"
#include "misc.h"
#include <iosfwd>
#include <vector>

// t has to be bigger than small_t to register an intersection with a ray.  You
// may need to tweak this value.
// http://stackoverflow.com/questions/17688360/ray-tracing-shadow-bug
static const double small_t = 1e-4;

class Ray;

class Object
{
public:
    // Useful for debugging
    std::string name;

    int num_parts=1;

    Object() = default;
    virtual ~Object() = default;

    // Check for an intersection in the range [small_t,inf).  If there are
    // multiple intersections, return the closest one.  If there are no
    // intersections, return a Hit structure with dist<0.  If possible
    // also compute hit.uv.  If part>=0, intersect only against part of the
    // primitive.  This is only used for meshes, where part is the triangle
    // index.  If part<0, intersect against all triangles.  For other
    // primitives, part is ignored.
    virtual Hit Intersection(const Ray& ray, int part) const=0;

    // Return the normal at the intersection location.  Note that the
    // intersection location is ray.Point(hit.dist).
    virtual vec3 Normal(const Ray& ray, const Hit& hit) const=0;

    virtual std::pair<Box,bool> Bounding_Box(int part) const=0;
};

#endif
