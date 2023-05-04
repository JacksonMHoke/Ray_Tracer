#include "parse.h"
#include "render_world.h"
#include <map>
#include <sstream>
#include <iostream>
#include <string>

void Parse::Parse_Input(Render_World& render_world, std::istream& in)
{
    std::string token,s0,s1,line;
    vec3 u,v,w;
    double f0;

    while(getline(in,line))
    {
        std::stringstream ss(line);
        ss>>token;
        if(token[0]=='#')
        {
        }
        else if(auto it=parse_objects.find(token); it!=parse_objects.end())
        {
            auto o=it->second(this,ss);
            objects[o->name]=o;
            render_world.all_objects.push_back(o);
        }
        else if(auto it=parse_shaders.find(token); it!=parse_shaders.end())
        {
            auto s=it->second(this,ss);
            shaders[s->name]=s;
            render_world.all_shaders.push_back(s);
        }
        else if(auto it=parse_lights.find(token); it!=parse_lights.end())
        {
            render_world.lights.push_back(it->second(this,ss));
        }
        else if(auto it=parse_colors.find(token); it!=parse_colors.end())
        {
            auto c=it->second(this,ss);
            colors[c->name]=c;
            render_world.all_colors.push_back(c);
        }
        else if(token=="shaded_object")
        {
            auto o=Get_Object(ss);
            auto s=Get_Shader(ss);
            render_world.objects.push_back({o,s});
        }
        else if(token=="background_shader")
        {
            render_world.background_shader=Get_Shader(ss);
        }
        else if(token=="ambient_light")
        {
            render_world.ambient_color=Get_Color(ss);
            ss>>render_world.ambient_intensity;
        }
        else if(token=="size")
        {
            ss>>width>>height;
        }
        else if(token=="camera")
        {
            ss>>u>>v>>w>>f0;
            render_world.camera.Position_And_Aim_Camera(u,v,w);
            render_world.camera.Focus_Camera(1,(double)width/height,f0*(pi/180));
        }
        else if(token=="enable_shadows")
        {
            ss>>render_world.enable_shadows;
        }
        else if(token=="recursion_depth_limit")
        {
            ss>>render_world.recursion_depth_limit;
        }
        else
        {
            std::cout<<"Failed to parse at: "<<token<<std::endl;
            exit(EXIT_FAILURE);
        }
        assert(ss);
    }
    render_world.camera.Set_Resolution(ivec2(width,height));
}

const Shader* Parse::Get_Shader(std::istream& in) const
{
    std::string token;
    in>>token;
        
    auto it=shaders.find(token);
    assert(it!=shaders.end());
    return it->second;
}

const Object* Parse::Get_Object(std::istream& in) const
{
    std::string token;
    in>>token;

    auto it=objects.find(token);
    assert(it!=objects.end());
    return it->second;
}

const Color* Parse::Get_Color(std::istream& in) const
{
    std::string token;
    in>>token;

    auto it=colors.find(token);
    assert(it!=colors.end());
    return it->second;
}
