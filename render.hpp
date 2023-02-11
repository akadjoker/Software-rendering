/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 17:21:21 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/02/10 17:49:20 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#pragma once
#include <stdint.h>
#include <cmath>
#include "vector.hpp"
#include "matrix.hpp"
#include "math.hpp"


#include "stb_image.h" 


enum cull_method 
{
    CULL_NONE,
    CULL_BACKFACE
};

enum render_method 
{
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE,
    RENDER_TEXTURED,
    RENDER_TEXTURED_WIRE
};


struct Texcoods
{
    float u;
    float v;
    Texcoods():u(0.0f),v(0.0f){};
    Texcoods(float x, float y):u(x),v(y){};
    
};

 struct Face
{
    int a;
    int b;
    int c;
    Texcoods a_uv;
    Texcoods b_uv;
    Texcoods c_uv;
    uint32_t color;
} ;



struct Texture
{

  	int		width;
	int		height;
    int             channels;
    unsigned char*	buffer;

    Texture():width(0),height(0),buffer(NULL){};
    
    Texture(const std::string &file_name)
    {
        stbi_set_flip_vertically_on_load(1);
        
       buffer = stbi_load(file_name.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    }
    bool load(const std::string &file_name)
    {
      //  stbi_set_flip_vertically_on_load(1);
      stbi_set_flip_vertically_on_load(true); 
        
        if(buffer)
            free(buffer);
               buffer = stbi_load(file_name.c_str(), &width, &height, &channels, STBI_rgb_alpha);
 
        return true;
    }
    
    ~Texture()
    {
        if(buffer)
            free(buffer);
    }
    

};

struct Triangle
{
    Vector4    points[3];
    Texcoods   texcoords[3];
    uint32_t color;
    Texture* texture;
};



inline Vector3 get_triangle_normal(Vector4 vertices[3]) 
{
    // Get individual vectors from A, B, and C vertices to compute normal
    Vector3 vector_a = vec3_from_vec4(vertices[0]);  /*   A   */
    Vector3 vector_b = vec3_from_vec4(vertices[1]);  /*  / \  */
    Vector3 vector_c = vec3_from_vec4(vertices[2]);  /* C---B */

    // Get the vector subtraction of B-A and C-A
    Vector3 vector_ab = vec3_sub(vector_b, vector_a);
    Vector3 vector_ac = vec3_sub(vector_c, vector_a);
    vec3_normalize(&vector_ab);
    vec3_normalize(&vector_ac);

    // Compute the face normal (using cross product to find perpendicular)
    Vector3 normal = vec3_cross(vector_ab, vector_ac);
    vec3_normalize(&normal);

    return normal;
}


inline Vector3 barycentric_weights(Vector2 a, Vector2 b, Vector2 c, Vector2 p) 
{
    // Find the vectors between the vertices ABC and point p
    Vector2 ac = vec2_sub(c, a);
    Vector2 ab = vec2_sub(b, a);
    Vector2 ap = vec2_sub(p, a);
    Vector2 pc = vec2_sub(c, p);
    Vector2 pb = vec2_sub(b, p);

    // Compute the area of the full parallegram/triangle ABC using 2D cross product
    float area_parallelogram_abc = (ac.x * ab.y - ac.y * ab.x); // || AC x AB ||

    // Alpha is the area of the small parallelogram/triangle PBC divided by the area of the full parallelogram/triangle ABC
    float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;

    // Beta is the area of the small parallelogram/triangle APC divided by the area of the full parallelogram/triangle ABC
    float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;

    // Weight gamma is easily found since barycentric coordinates always add up to 1.0
    float gamma = 1 - alpha - beta;

    return Vector3( alpha, beta, gamma );
}

extern void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
extern void draw_pixel(int x, int y, uint32_t color);
extern float get_zbuffer_at(int x, int y) ;
extern void update_zbuffer_at(int x, int y, float value);

inline void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}




///////////////////////////////////////////////////////////////////////////////
// Function to draw a solid pixel at position (x,y) using depth interpolation
///////////////////////////////////////////////////////////////////////////////
void draw_triangle_pixel(int x, int y, Vector4 point_a, Vector4 point_b, Vector4 point_c,uint32_t color) 
{
    // Create three vec2 to find the interpolation
    Vector2 p = Vector2(x, y );
    Vector2 a = vec2_from_vec4(point_a);
    Vector2 b = vec2_from_vec4(point_b);
    Vector2 c = vec2_from_vec4(point_c);

    // Calculate the barycentric coordinates of our point 'p' inside the triangle
    Vector3 weights = barycentric_weights(a, b, c, p);

    // Fetch alpha, beta, and gamma from the barycentric coordinate calculation
    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;
    
    // Interpolate the value of 1/w for the current pixel
    float interpolated_reciprocal_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta + (1 / point_c.w) * gamma;

    // Adjust 1/w so the pixels that are closer to the camera have smaller values
    interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

    // Only draw the pixel if the depth value is less than the one previously stored in the z-buffer
    if (interpolated_reciprocal_w < get_zbuffer_at(x, y))
    {
        
        // Draw a pixel at position (x,y) with a solid color
        draw_pixel(x, y, color);

        // Update the z-buffer value with the 1/w of this current pixel
        update_zbuffer_at(x, y, interpolated_reciprocal_w);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Function to draw the textured pixel at position (x,y) using depth interpolation
///////////////////////////////////////////////////////////////////////////////
void draw_triangle_texel(int x, int y,
    Vector4 point_a, Vector4 point_b, Vector4 point_c,
    Texcoods a_uv, Texcoods b_uv, Texcoods c_uv,
    Texture *texture) 
    {
    Vector2 p = Vector2(x, y);
    Vector2 a = vec2_from_vec4(point_a);
    Vector2 b = vec2_from_vec4(point_b);
    Vector2 c = vec2_from_vec4(point_c);

    // Calculate the barycentric coordinates of our point 'p' inside the triangle
    Vector3 weights = barycentric_weights(a, b, c, p);

    // Fetch alpha, beta, and gamma from the barycentric coordinates calculation
    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;
    
    // Variables to store the interpolated values of U, V, and also 1/w for the current pixel
    float interpolated_u;
    float interpolated_v;
    float interpolated_reciprocal_w;

    // Perform the interpolation of all U/w and V/w values using barycentric weights and a factor of 1/w
    interpolated_u = (a_uv.u / point_a.w) * alpha + (b_uv.u / point_b.w) * beta + (c_uv.u / point_c.w) * gamma;
    interpolated_v = (a_uv.v / point_a.w) * alpha + (b_uv.v / point_b.w) * beta + (c_uv.v / point_c.w) * gamma;

    // Also interpolate the value of 1/w for the current pixel
    interpolated_reciprocal_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta + (1 / point_c.w) * gamma;

    // Now we can divide back both interpolated values by 1/w
    interpolated_u /= interpolated_reciprocal_w;
    interpolated_v /= interpolated_reciprocal_w;

    // Get the mesh texture width and height dimensions
    int texture_width =  texture->width;
    int texture_height = texture->height;

    // Map the UV coordinate to the full texture width and height
    int tex_x = abs((int)(interpolated_u * texture_width)) % texture_width;
    int tex_y = abs((int)(interpolated_v * texture_height)) % texture_height;

    // Adjust 1/w so the pixels that are closer to the camera have smaller values
    interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

    // Only draw the pixel if the depth value is less than the one previously stored in the z-buffer
    if (interpolated_reciprocal_w < get_zbuffer_at(x, y)) 
    {
        // Draw a pixel at position (x,y) with the color that comes from the mapped texture
        uint32_t* texture_buffer = (uint32_t*)texture->buffer;
        draw_pixel(x, y, texture_buffer[(texture_width * tex_y) + tex_x]);

        // Update the z-buffer value with the 1/w of this current pixel
        update_zbuffer_at(x, y, interpolated_reciprocal_w);
    }
}

void draw_textured_triangle(
    int x0, int y0, float z0, float w0, float u0, float v0,
    int x1, int y1, float z1, float w1, float u1, float v1,
    int x2, int y2, float z2, float w2, float u2, float v2,
    Texture *texture
) {
    // We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
    if (y0 > y1) 
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&z0, &z1);
        float_swap(&w0, &w1);
        float_swap(&u0, &u1);
        float_swap(&v0, &v1);
    }
    if (y1 > y2)
     {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
        float_swap(&z1, &z2);
        float_swap(&w1, &w2);
        float_swap(&u1, &u2);
        float_swap(&v1, &v2);
    }
    if (y0 > y1) 
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&z0, &z1);
        float_swap(&w0, &w1);
        float_swap(&u0, &u1);
        float_swap(&v0, &v1);
    }

    // Flip the V component to account for inverted UV-coordinates (V grows downwards)
    v0 = 1.0 - v0;
    v1 = 1.0 - v1;
    v2 = 1.0 - v2;

    // Create vector points and texture coords after we sort the vertices
    Vector4 point_a = Vector4( x0, y0, z0, w0 );
    Vector4 point_b = Vector4( x1, y1, z1, w1 );
    Vector4 point_c = Vector4( x2, y2, z2, w2 );
    Texcoods a_uv = Texcoods( u0, v0 );
    Texcoods b_uv = Texcoods( u1, v1 );
    Texcoods c_uv = Texcoods( u2, v2 );

    ///////////////////////////////////////////////////////
    // Render the upper part of the triangle (flat-bottom)
    ///////////////////////////////////////////////////////
    float inv_slope_1 = 0;
    float inv_slope_2 = 0;

    if (y1 - y0 != 0) inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
    if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

    if (y1 - y0 != 0) 
    {
        for (int y = y0; y <= y1; y++) 
        {
            int x_start = x1 + (y - y1) * inv_slope_1;
            int x_end = x0 + (y - y0) * inv_slope_2;

            if (x_end < x_start) 
            {
                int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
            }

            for (int x = x_start; x < x_end; x++) 
            {
                // Draw our pixel with the color that comes from the texture
                draw_triangle_texel(x, y, point_a, point_b, point_c, a_uv, b_uv, c_uv, texture);
            }
        }
    }

    ///////////////////////////////////////////////////////
    // Render the bottom part of the triangle (flat-top)
    ///////////////////////////////////////////////////////
    inv_slope_1 = 0;
    inv_slope_2 = 0;

    if (y2 - y1 != 0) inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
    if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

    if (y2 - y1 != 0) 
    {
        for (int y = y1; y <= y2; y++) 
        {
            int x_start = x1 + (y - y1) * inv_slope_1;
            int x_end = x0 + (y - y0) * inv_slope_2;

            if (x_end < x_start) 
            {
                int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
            }

            for (int x = x_start; x < x_end; x++) 
            {
                draw_triangle_texel(x, y, point_a, point_b, point_c, a_uv, b_uv, c_uv, texture);
            }
        }
    }
}

void draw_filled_triangle(
    int x0, int y0, float z0, float w0,
    int x1, int y1, float z1, float w1,
    int x2, int y2, float z2, float w2,
    uint32_t color) 
    {
    // We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
    if (y0 > y1) 
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&z0, &z1);
        float_swap(&w0, &w1);
    }
    if (y1 > y2) 
    {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
        float_swap(&z1, &z2);
        float_swap(&w1, &w2);
    }
    if (y0 > y1) 
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&z0, &z1);
        float_swap(&w0, &w1);
    }

    // Create three vector points after we sort the vertices
    Vector4 point_a = Vector4(x0, y0, z0, w0 );
    Vector4 point_b = Vector4(x1, y1, z1, w1 );
    Vector4 point_c = Vector4(x2, y2, z2, w2 );

    ///////////////////////////////////////////////////////
    // Render the upper part of the triangle (flat-bottom)
    ///////////////////////////////////////////////////////
    float inv_slope_1 = 0;
    float inv_slope_2 = 0;

    if (y1 - y0 != 0) inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
    if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

    if (y1 - y0 != 0) 
    {
        for (int y = y0; y <= y1; y++) 
        {
            int x_start = x1 + (y - y1) * inv_slope_1;
            int x_end = x0 + (y - y0) * inv_slope_2;

            if (x_end < x_start) 
            {
                int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
            }

            for (int x = x_start; x < x_end; x++) 
            {
               draw_triangle_pixel(x, y, point_a, point_b, point_c, color);
            }
        }
    }

    ///////////////////////////////////////////////////////
    // Render the bottom part of the triangle (flat-top)
    ///////////////////////////////////////////////////////
    inv_slope_1 = 0;
    inv_slope_2 = 0;

    if (y2 - y1 != 0) inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
    if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

    if (y2 - y1 != 0) 
    {
        for (int y = y1; y <= y2; y++) 
        {
            int x_start = x1 + (y - y1) * inv_slope_1;
            int x_end = x0 + (y - y0) * inv_slope_2;

            if (x_end < x_start) 
            {
                int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
            }

            for (int x = x_start; x < x_end; x++) 
            {
                 draw_triangle_pixel(x, y, point_a, point_b, point_c, color);
            }
        }
    }
}