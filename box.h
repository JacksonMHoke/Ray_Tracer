#ifndef __BOX_H__
#define __BOX_H__

#include "ray.h"
#include "misc.h"
#include <limits>

class Box
{
public:
    // lowermost and uppermost corners of bounding box
    vec3 lo,hi;

    // Return whether the ray intersects this box.
    std::pair<bool,double> Intersection(const Ray& ray) const;

    // Compute the smallest box that contains both *this and bb.
    Box Union(const Box& bb) const;

    // Compute the smallest box that is contained by both *this and bb.
    Box Intersection(const Box& bb) const;

    // Enlarge this box (if necessary) so that pt also lies inside it.
    void Include_Point(const vec3& pt);

    // Create a box to which points can be correctly added using Include_Point.
    void Make_Empty();

    // Create a box that contains everything.
    void Make_Full();

    bool Is_Full() const;

    // Enlarge this box (if necessary) so that pt also lies inside it.
    bool Test_Inside(const vec3& pt) const;
};

// Useful for debugging
std::ostream& operator<<(std::ostream& o, const Box& b);

#endif
