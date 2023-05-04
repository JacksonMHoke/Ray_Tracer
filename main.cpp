#include <filesystem>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "common.h"
namespace fs = std::filesystem;

int max_triangles=4;
int max_cases=10;
int width=640;
int height=480;

static std::random_device rd;
static std::mt19937 mt(rd());

static std::uniform_real_distribution<> dist(0, 1);
inline double get_rand(){return dist(mt);}

static std::uniform_int_distribution<std::mt19937::result_type> idist(0, std::mt19937::max());
inline unsigned rand_int(){return idist(mt);}

template<int n> void fill_rand(vec<double,n>& u,double a,double b)
{
    for(int i=0;i<n;i++) u[i]=get_rand()*(b-a)+a;
}

// Make sure w>0 and also the vertices are inside the viewing volume.
void fix_pt(vec4& A)
{
    double w=fabs(A[3]);
    A[3]=1;
    A*=w;
}

inline void fill_rand(Triangle& t)
{
    fill_rand(t.A,-1,1);
    fill_rand(t.B,-1,1);
    fill_rand(t.C,-1,1);
    fix_pt(t.A);
    fix_pt(t.B);
    fix_pt(t.C);
    fill_rand(t.Ca,0,1);
    fill_rand(t.Cb,0,1);
    fill_rand(t.Cc,0,1);
}

bool Test(int num_tri, int test_case)
{
    // Make results reproducible, even if tests are run one at a time.
    mt.seed(1000*num_tri+test_case);

    // Generate the triangles randomly.
    std::vector<Triangle> tris;
    for(int j=0;j<num_tri;j++)
    {
        Triangle t;
        fill_rand(t);
        tris.push_back(t);
    }
            
    // Allocate pixels and set black.
    std::vector<Pixel> pixels(width*height);
    fill(pixels.begin(),pixels.end(),0xff);

    // Rasterize triangles
    Rasterize(&pixels[0], width, height, tris);

    char name[100];
    sprintf(name,"out-%d-%d.png",num_tri,test_case);
    Dump_png(&pixels[0], width, height, name);

    sprintf(name,"sol-%d-%d.png",num_tri,test_case);
    if(fs::exists(name))
    {
        Pixel* data_sol = nullptr;
        int w,h;
        Read_png(data_sol, w, h, name);
        assert(w==width && h==height);

        sprintf(name,"dif-%d-%d.png",num_tri,test_case);
        
        // For each pixel, check to see if it matches solution
        double error = 0, total = 0;
        for(int i=0; i<height*width; i++)
        {
            vec3 a=From_Pixel(pixels[i]);
            vec3 b=From_Pixel(data_sol[i]);
            for(int c=0; c<3; c++)
            {
                double e = fabs(a[c]-b[c]);
                error += e;
                total++;
                b[c] = e;
            }
            data_sol[i]=Pixel_Color(b);
        }

        // Output information on how well it matches. Optionally save to file
        // to avoid getting confused by debugging print statements.
        error/=total;
        bool pass=error<0.01;
        printf("%s   test %d %d   diff: %.2f\n",pass?"PASS":"FAIL",
            num_tri, test_case, error*100);

        // Output images showing the error that was computed to aid debugging
        Dump_png(data_sol,width,height,name);
        delete [] data_sol;

        return pass;
    }

    // No solution to compare to; treat this as a fail.
    return false;
}

int main(int argc, char* argv[])
{
    bool pass = true;

    if(argc>2)
    {
        if(!Test(atoi(argv[1]),atoi(argv[2])))
            pass=false;
    }
    else if(argc>1)
    {
        int num_tri=atoi(argv[1]);
        for(int test_case=0;test_case<max_cases;test_case++)
            if(!Test(num_tri,test_case))
                pass=false;
    }
    else
    {
        for(int num_tri=1;num_tri<=max_triangles;num_tri++)
            for(int test_case=0;test_case<max_cases;test_case++)
                if(!Test(num_tri,test_case))
                    pass=false;
    }

    if(pass) std::cout<<"PASS"<<std::endl;
    else std::cout<<"FAIL"<<std::endl;

    // Detect early termination.
    if(argv[1]) std::cout<<"TOKEN "<<argv[1]<<std::endl;
    
    return 0;
}
