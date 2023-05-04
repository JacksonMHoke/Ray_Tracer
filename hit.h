#ifndef __HIT_H__
#define __HIT_H__

#include "vec.h"

// Records information about an intersection, which may be needed later for a
// subsequent call to Normal.
struct Hit
{
    // Distance along the ray at which this occurred; if there was no
    // intersection, set this to a negative value.
    double dist = -1;

    // Which triangle was intersected (for meshes)
    int triangle = -1;

    // Barycentric weights of intersection within triangle (for meshes)
    vec2 uv = {};

    bool Valid() const {return dist>=0;}
};

// Useful for debugging
inline std::ostream& operator<<(std::ostream& o, const Hit& h)
{
    return o << "(dist: " << h.dist << "; triangle: " << h.triangle
             << "; uv: " << h.uv<<")";
}

#endif
