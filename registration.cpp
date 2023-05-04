#include "flat_shader.h"
#include "mesh.h"
#include "parse.h"
#include "phong_shader.h"
#include "plane.h"
#include "point_light.h"
#include "reflective_shader.h"
#include "sphere.h"
#include "texture.h"
#include "transparent_shader.h"

void Setup_Parsing(Parse& parse)
{
    parse.template Register_Object<Sphere>();
    parse.template Register_Object<Plane>();
    parse.template Register_Object<Mesh>();

    parse.template Register_Light<Point_Light>();

    parse.template Register_Shader<Flat_Shader>();
    parse.template Register_Shader<Phong_Shader>();
    parse.template Register_Shader<Reflective_Shader>();
    parse.template Register_Shader<Transparent_Shader>();

    parse.template Register_Color<Fixed_Color>();
    parse.template Register_Color<Texture>();
}
