#include <vector>
#include <string>
#include <iostream>
#include "common.h"
#include <math.h>

using namespace std;

void Rasterize(Pixel* pixels, int width, int height, const std::vector<Triangle>& tris)
{
    vector<vector<double>> zbuff(height, vector<double>(width, std::numeric_limits<double>::max()));
    for (auto t : tris) {
        vec4 aH=t.A, bH=t.B, cH=t.C; //homogenous coords
        vec3 a,b,c; //euclidean coords
        a[0]=aH[0]*(width/2)/aH[3]+0.5; a[1]=aH[1]*(height/2)/aH[3]+0.5; a[2]=aH[2]/aH[3];
        b[0]=bH[0]*(width/2)/bH[3]+0.5; b[1]=bH[1]*(height/2)/bH[3]+0.5; b[2]=bH[2]/bH[3];
        c[0]=cH[0]*(width/2)/cH[3]+0.5; c[1]=cH[1]*(height/2)/cH[3]+0.5; c[2]=cH[2]/cH[3];

        double min_x=floor(min(a[0], min(b[0], c[0])));
        double max_x=floor(max(a[0], max(b[0], c[0])));
        double min_y=floor(min(a[1], min(b[1], c[1])));
        double max_y=floor(max(a[1], max(b[1], c[1])));

        vec3 a2=a, b2=b, c2=c;
        a2[2]=0; b2[2]=0; c2[2]=0;

        for (double i=min_y; i<max_y; ++i) {
            for (double j=min_x; j<max_x; ++j) {
                vec3 pos; pos[0]=j; pos[1]=i;
                vec3 v0=b2-a2, v1=c2-a2, v2=pos-a2;
                double d00 = dot(v0, v0), d01 = dot(v0, v1), d11 = dot(v1, v1), d20 = dot(v2, v0), d21 = dot(v2, v1);
                double denom = d00 * d11 - d01 * d01;
                double beta = (d11 * d20 - d01 * d21) / denom;
                double gamma = (d00 * d21 - d01 * d20) / denom;
                double alpha = 1.0 - gamma - beta;
                
                if (min(alpha, min(beta, gamma))>=0) {
                    double z_val = alpha*a[2]+beta*b[2]+gamma*c[2];
                    double wp = 1.0/(alpha/aH[3]+beta/bH[3]+gamma/cH[3]);
                    alpha*=wp/aH[3];
                    beta*=wp/bH[3];
                    gamma*=wp/cH[3];
                    if (z_val<zbuff[i+height/2][j+width/2]) {
                        zbuff[i+height/2][j+width/2]=z_val;
                        set_pixel(pixels, width, height, j+width/2, i+height/2, t.Ca*alpha+t.Cb*beta+t.Cc*gamma);
                    }
                }
            }
        }
    }
}

/*
// initialize the z-buffer with maximum depth
z_buffer = create_2d_array(screen_width, screen_height, max_depth_value)

// loop over all triangles
for each triangle in mesh:

  // convert vertex coordinates from homogenous to euclidean
  vertex_0 = triangle.vertex_0.xyz / triangle.vertex_0.w
  vertex_1 = triangle.vertex_1.xyz / triangle.vertex_1.w
  vertex_2 = triangle.vertex_2.xyz / triangle.vertex_2.w

  // calculate the bounding box for the triangle in screen space
  min_x = floor(min(vertex_0.x, vertex_1.x, vertex_2.x))
  max_x = ceil(max(vertex_0.x, vertex_1.x, vertex_2.x))
  min_y = floor(min(vertex_0.y, vertex_1.y, vertex_2.y))
  max_y = ceil(max(vertex_0.y, vertex_1.y, vertex_2.y))

  // loop over all pixels inside the bounding box
  for y = min_y to max_y:
    for x = min_x to max_x:
      
      // calculate the barycentric coordinates of the pixel
      barycentric = calculate_barycentric_coordinates(vertex_0, vertex_1, vertex_2, x, y)

      // check if the pixel is inside the triangle
      if barycentric is valid:

        // calculate the z-value of the pixel
        z_value = calculate_interpolated_z_value(vertex_0, vertex_1, vertex_2, barycentric)

        // check if the pixel is closer to the camera than the existing pixel in the z-buffer
        if z_value < z_buffer[x][y]:
          
          // update the z-buffer with the new z-value
          z_buffer[x][y] = z_value

          // calculate the color of the pixel and draw it on the screen
          pixel_color = calculate_interpolated_color(vertex_0_color, vertex_1_color, vertex_2_color, barycentric)
          draw_pixel(x, y, pixel_color)
*/