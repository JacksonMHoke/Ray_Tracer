#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

#include <math.h>
#include <vector>
#include <iostream>
#include <limits>
#include "vec.h"
#include "light.h"

class Color;

class Point_Light : public Light
{
public:
    const Color* color = nullptr; // RGB color components
    double brightness = 0;

    Point_Light(const Parse* parse,std::istream& in);
    virtual ~Point_Light() = default;

    virtual vec3 Emitted_Light(const vec3& vector_to_light) const override;

    static constexpr const char* parse_name = "point_light";
};
#endif
