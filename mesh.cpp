#include "mesh.h"
#include <fstream>
#include <limits>
#include <string>
#include <algorithm>

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

Mesh::Mesh(const Parse* parse, std::istream& in)
{
    std::string file;
    in>>name>>file;
    Read_Obj(file.c_str());
}

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e, t;
    vec3 v;
    vec2 u;
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }

        if(sscanf(line.c_str(), "vt %lg %lg", &u[0], &u[1]) == 2)
        {
            uvs.push_back(u);
        }

        if(sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &e[0], &t[0], &e[1], &t[1], &e[2], &t[2]) == 6)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
            for(int i=0;i<3;i++) t[i]--;
            triangle_texture_index.push_back(t);
        }
    }
    num_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
    Debug_Scope scope;
    double min_t = std::numeric_limits<double>::max();
    Hit rt;
    if (part<0) {
        for (int i=0; i<(int)triangles.size(); ++i) {
            Hit h = Intersect_Triangle(ray, i);
            if (h.dist<small_t) Pixel_Print("No intersection with triangle: ", i);
            else {
                Pixel_Print("intersect test with ", name, "; hit: ", h);
                if (h.dist<min_t) {
                    min_t = h.dist;
                    rt = h;
                }
            }
        }
        return rt;
    } else {
        return Intersect_Triangle(ray, part);
    }  
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const Ray& ray, const Hit& hit) const
{
    assert(hit.triangle>=0);
    ivec3 tri = triangles[hit.triangle];
    vec3 v1=vertices[tri[0]], v2=vertices[tri[1]], v3=vertices[tri[2]];
    vec3 normal = cross(v2-v1, v3-v1).normalized();
    return normal;
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
Hit Mesh::Intersect_Triangle(const Ray& ray, int tri) const
{
    Hit rt;
    rt.triangle=tri;
    vec3 normal = Mesh::Normal(ray, rt);

    ivec3 triang = triangles[tri];
    vec3 a=vertices[triang[0]], b=vertices[triang[1]], c=vertices[triang[2]];

    //plane intersection
    double denominator = dot(normal, ray.direction);
    if (abs(denominator) <= small_t) return rt;
    double t = dot(a-ray.endpoint, normal)/denominator;
    if (t<=small_t) return rt;

    vec3 p = ray.Point(t);

    double alpha = dot(cross(b-p, c-p), normal)/(cross(b-a, c-a)).magnitude();
    double beta = dot(cross(p-a, c-a), normal)/(cross(b-a, c-a)).magnitude();
    double gamma = dot(cross(b-a, p-a), normal)/(cross(b-a, c-a)).magnitude();

    vec3 weights;
    weights[0]=alpha;
    weights[1]=beta;
    weights[2]=gamma;
    if (alpha > -1*weight_tolerance && beta > -1*weight_tolerance && gamma > -1*weight_tolerance) {
        rt.dist = t;
        rt.uv[0]=alpha*uvs[triangle_texture_index[tri][0]][0]+beta*uvs[triangle_texture_index[tri][1]][0]+gamma*uvs[triangle_texture_index[tri][2]][0];
        rt.uv[1]=alpha*uvs[triangle_texture_index[tri][0]][1]+beta*uvs[triangle_texture_index[tri][1]][1]+gamma*uvs[triangle_texture_index[tri][2]][1];
    }
    Pixel_Print("mesh ", name, " triangle ", tri, " intersected; weights: ", weights, "; dist: ", rt.dist);
    return rt;
}

std::pair<Box,bool> Mesh::Bounding_Box(int part) const
{
    if(part<0)
    {
        Box box;
        box.Make_Empty();
        for(const auto& v:vertices)
            box.Include_Point(v);
        return {box,false};
    }

    ivec3 e=triangles[part];
    vec3 A=vertices[e[0]];
    Box b={A,A};
    b.Include_Point(vertices[e[1]]);
    b.Include_Point(vertices[e[2]]);
    return {b,false};
}
