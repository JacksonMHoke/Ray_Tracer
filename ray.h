#ifndef __RAY_H__
#define __RAY_H__

#include "vec.h"

class Object;
class Ray
{
public:
    vec3 endpoint; // endpoint of the ray where t=0
    vec3 direction; // direction the ray sweeps out - unit vector

    Ray()
        :endpoint(0,0,0),direction(0,0,1)
    {}

    Ray(const vec3& endpoint_input,const vec3& direction_input)
        :endpoint(endpoint_input),direction(direction_input.normalized())
    {}

    vec3 Point(double t) const
    {
        return endpoint+direction*t;
    }
};

// Useful for debugging
inline std::ostream& operator<<(std::ostream& o, const Ray& r)
{
    return o << "(end: " << r.endpoint << "; dir: " << r.direction << ")";
}

#endif
