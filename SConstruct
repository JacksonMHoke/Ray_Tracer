import os
import glob
env = Environment(ENV = os.environ)

env.Append(LIBS=["png"])
env.Append(CXXFLAGS=["-std=c++17","-g","-Wall","-O3","-I/usr/include/libpng12"])
env.Append(LINKFLAGS=["-L/usr/local/lib"])

env.Program("ray_tracer",glob.glob("*.cpp"));
