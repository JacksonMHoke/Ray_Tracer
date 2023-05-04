// Student Name: Jackson Hoke
// Student ID: 862161195

#include <limits>
#include "box.h"

// Return whether the ray intersects this box.
std::pair<bool,double> Box::Intersection(const Ray& ray) const
{
    TODO;
    return {true,0};
}

// Compute the smallest box that contains both *this and bb.
Box Box::Union(const Box& bb) const
{
    Box box;
    TODO;
    return box;
}

// Compute the smallest box that contains both *this and bb.
Box Box::Intersection(const Box& bb) const
{
    Box box;
    TODO;
    return box;
}

// Enlarge this box (if necessary) so that pt also lies inside it.
void Box::Include_Point(const vec3& pt)
{
    TODO;
}

// Create a box to which points can be correctly added using Include_Point.
void Box::Make_Empty()
{
    lo.fill(std::numeric_limits<double>::infinity());
    hi=-lo;
}

// Create a box that contains everything.
void Box::Make_Full()
{
    hi.fill(std::numeric_limits<double>::infinity());
    lo=-hi;
}

bool Box::Test_Inside(const vec3& pt) const
{
    for(int i=0;i<3;i++)
        if(pt[i]<lo[i] || pt[i]>hi[i])
            return false;
    return true;
}

// Useful for debugging
std::ostream& operator<<(std::ostream& o, const Box& b)
{
    return o << "(lo: " << b.lo << "; hi: " << b.hi << ")";
}
