#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "vec.h"
#include "misc.h"
#include <iostream>
#include <limits>
#include <math.h>
#include <vector>

class Ray;
class Parse;

class Light
{
public:
    std::string name;
    vec3 position;
    
    Light() = default;
    virtual ~Light() = default;

    virtual vec3 Emitted_Light(const vec3& vector_to_light) const=0;
};
#endif
