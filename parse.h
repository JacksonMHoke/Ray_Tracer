#ifndef __PARSE_H__
#define __PARSE_H__

#include <map>
#include "object.h"
#include "light.h"
#include "shader.h"
#include "color.h"

class Render_World;

class Parse
{
    // Lookup shaders/objects/colors by name.
    std::map<std::string,const Shader*> shaders;
    std::map<std::string,const Object*> objects;
    std::map<std::string,const Color*> colors;

    // These are factories.  Given the class's parse name, construct an object
    // of the correct type.  The object's constructor will parse from the input
    // stream to initialize itself.  Note that the key is a string and the data
    // is a function pointer.  This function is called to construct the object.
    // These are populated by the registration routines below.
    std::map<std::string,Shader*(*)(const Parse* parse,std::istream& in)> parse_shaders;
    std::map<std::string,Object*(*)(const Parse* parse,std::istream& in)> parse_objects;
    std::map<std::string,Light*(*)(const Parse* parse,std::istream& in)> parse_lights;
    std::map<std::string,Color*(*)(const Parse* parse,std::istream& in)> parse_colors;

    // image dimensions
    int width=-1;
    int height=-1;
public:
    void Parse_Input(Render_World& render_world, std::istream& in);

    // Public access to the stored objects.
    const Color* Get_Color(std::istream& in) const;
    const Shader* Get_Shader(std::istream& in) const;
    const Object* Get_Object(std::istream& in) const;

    // These are the routines that populate the factories above.  They are
    // called in registration.cpp.  You will need to modify that file as you get
    // additional source files in later homework assignments.  There is a bit of
    // "magic" happening in the routines below.  Interesting things to note:

    // 1. The routine is called with a template argument that indicates the type
    //    being registered (such as Register_Object<Sphere>() or
    //    Register_Shader<Flat_Shader>()).

    // 2. The routine then generates a function (a lambda function) that
    //    allocate an object of the appropriate type, passes its arguments to
    //    the constructor (which actually does the parsing), and then returns
    //    the pointer.  Caller owns the allocated memory and must free it.

    // 3. The generated function is then inserted into the lookup table.  The
    //    parse key is Type::parse_name.  This is a fixed string defined inside
    //    each derived class that can be parsed.  In particular, classes like
    //    Sphere or Flat_Shader know how to parse themselves.  Although this
    //    makes parsing more complicated, it lets us add new files, types, and
    //    the ability to parse them.

    // 4. One might ask whether it is possible for registration of Sphere to
    //    occur in sphere.cpp so that the file could be added without modifying
    //    any existing files.  In C and C++, this is not reliably possible.
    //    Without some other file referring to something (anything at all!) in
    //    sphere.cpp, the linker is free to discard it in its entirety.  The
    //    file registration.cpp (and only that file!) refers to something in
    //    those files (their constructors!).

    template<class Type> void Register_Object()
    {
        parse_objects[Type::parse_name]=
            [](const Parse* parse,std::istream& in) -> Object*
            {return new Type(parse,in);};
    }
    template<class Type> void Register_Shader()
    {
        parse_shaders[Type::parse_name]=
            [](const Parse* parse,std::istream& in) -> Shader*
            {return new Type(parse,in);};
    }
    template<class Type> void Register_Light()
    {
        parse_lights[Type::parse_name]=
            [](const Parse* parse,std::istream& in) -> Light*
            {return new Type(parse,in);};
    }
    template<class Type> void Register_Color()
    {
        parse_colors[Type::parse_name]=
            [](const Parse* parse,std::istream& in) -> Color*
            {return new Type(parse,in);};
    }
};

#endif
