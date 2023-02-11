
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "main.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  





SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* colorbuffer = NULL;
float* zbuffer = NULL;

SDL_Texture* colorbuffer_texture = NULL;
int window_width = 1024;
int window_height = 720;

int render_method = 0;
int cull_method = 0;




int get_window_width(void) 
{
    return window_width;
}

int get_window_height(void) 
{
    return window_height;
}




void set_render_method(int method) 
{
    render_method = method;
}

void set_cull_method(int method) 
{
    cull_method = method;
}

bool should_render_wire(void)
 {
    return (
        render_method == RENDER_WIRE ||
        render_method == RENDER_WIRE_VERTEX ||
        render_method == RENDER_FILL_TRIANGLE_WIRE ||
        render_method == RENDER_TEXTURED_WIRE
    );
}

bool should_render_wire_vertex(void) 
{
    return (
        render_method == RENDER_WIRE_VERTEX
    );
}

bool should_render_filled_triangle(void)
 {
    return (
        render_method == RENDER_FILL_TRIANGLE ||
        render_method == RENDER_FILL_TRIANGLE_WIRE 
    );
}

bool should_render_textured_triangle(void) 
{
    return (
        render_method == RENDER_TEXTURED ||
        render_method == RENDER_TEXTURED_WIRE
    );
}

bool should_cull_backface(void)
 {
    return cull_method == CULL_BACKFACE;
}

void draw_grid(void) {

    for (int y = 0; y < window_height; y += 10) 
    {
        for (int x = 0; x < window_width; x += 10) 
        {
            colorbuffer[(window_width * y) + x] = 0xFF444444;
        }
    }
}

void draw_pixel(int x, int y, uint32_t color) 
{
    if (x < 0 || x >= window_width || y < 0 || y >= window_height) {
        return;
    }
    colorbuffer[(window_width * y) + x] = color;
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) 
{
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

    float x_inc = delta_x / (float)longest_side_length;
    float y_inc = delta_y / (float)longest_side_length;

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i <= longest_side_length; i++) 
    {
        draw_pixel(round(current_x), round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

void draw_rect(int x, int y, int width, int height, uint32_t color)
 {
    for (int i = 0; i < width; i++) 
    {
        for (int j = 0; j < height; j++)
         {
            int current_x = x + i;
            int current_y = y + j;
            draw_pixel(current_x, current_y, color);
        }
    }
}

void render_color_buffer(void) 
{
    SDL_UpdateTexture(colorbuffer_texture,NULL,colorbuffer,(int)(window_width * sizeof(uint32_t)));
    SDL_RenderCopy(renderer, colorbuffer_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void clear_color_buffer(uint32_t color) 
{
    for (int i = 0; i < window_width * window_height; i++) 
    {
        colorbuffer[i] = color;
    }
}

void clear_z_buffer(void) {
    for (int i = 0; i < window_width * window_height; i++) 
    {
        zbuffer[i] = 1.0;
    }
}

float get_zbuffer_at(int x, int y) 
{
    if (x < 0 || x >= window_width || y < 0 || y >= window_height) 
    {
        return 1.0;
    }
    return zbuffer[(window_width * y) + x];
}

void update_zbuffer_at(int x, int y, float value) 
{
    if (x < 0 || x >= window_width || y < 0 || y >= window_height) 
    {
        return;
    }
    zbuffer[(window_width * y) + x] = value;
}

uint32_t apply_light_intensity(uint32_t original_color, float factor) {
    if (factor < 0) factor = 0;
    if (factor > 1) factor = 1;

    uint32_t a = (original_color & 0xFF000000);
    uint32_t r = (original_color & 0x00FF0000) * factor;
    uint32_t g = (original_color & 0x0000FF00) * factor;
    uint32_t b = (original_color & 0x000000FF) * factor;

    uint32_t new_color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);

    return new_color;
}


int main()
{  
  
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
  {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    // Set width and height of the SDL window with the max screen resolution
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
   // int fullscreen_width = display_mode.w;
  //  int fullscreen_height = display_mode.h;

 //   window_width = fullscreen_width / 2;
   // window_height = fullscreen_height / 2;

    // Create a SDL Window
    window = SDL_CreateWindow(NULL,  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
    if (!window) 
    {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    // Create a SDL renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    // Allocate the required memory in bytes to hold the color buffer and the z-buffer
    colorbuffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
    zbuffer = (float*)malloc(sizeof(float) * window_width * window_height);

    // Creating a SDL texture that is used to display the color buffer
    colorbuffer_texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STREAMING,window_width,window_height  );



  set_render_method(RENDER_TEXTURED);
  set_cull_method(CULL_BACKFACE);

    // Initialize the scene light d

Mesh mesh;
// mesh.load("assets/cube.obj",1.0f);
// mesh.texture.load("assets/pikuma.png");

// mesh.load("assets/f22.obj",1.0f);
// mesh.texture.load("assets/f22.png");

mesh.load("assets/f117.obj",1.0f);
mesh.texture.load("assets/f117.png");

int frameCount = 0;
Uint32 startTime = SDL_GetTicks();

bool running = true;
while (running) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) 
  {
    switch (event.type)
      {
        case SDL_QUIT:
          running = false;
          break;
        case SDL_KEYDOWN:

          break;
        case SDL_KEYUP:
   
          break;
        case SDL_MOUSEMOTION:
        
          break;
        case SDL_MOUSEWHEEL:
          
          break;
      }
  }


 int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    // Only delay execution if we are running too fast
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) 
    {
        SDL_Delay(time_to_wait);
    }

    // Get a delta time factor converted to seconds to be used to update our game objects
    delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0;
    previous_frame_time = SDL_GetTicks();
    triangles_to_render_count = 0;


    clear_color_buffer(0);
    clear_z_buffer();
    draw_grid();


Vector3 position = Vector3(0.0f, 0.0f, -25.0f);



// Criar as matrizes de projeção, visualização e modelo
Matrix view =  mat4_look_at(Vector3(0.0f, 0.0f, -5.0f), position, Vector3(0.0f, 1.0f, 0.0f));
Matrix projection =mat4_perspective(radians(45.0f), (float)get_window_width() / (float)get_window_height(), 0.1f, 100.0f);

 
 Matrix model = mat4_identity();
 model = Matrix::Rotate( (float)SDL_GetTicks()/3000.0f, 1.0f, 0.0f, 0.0f) * model;
 model = Matrix::Rotate( (float)SDL_GetTicks()/2000.0f, 0.0f, 1.0f, 0.0f) * model;
 model = Matrix::Rotate( (float)SDL_GetTicks()/2000.0f, 0.0f, 0.0f, 1.0f) * model;


for (int i=0; i < (int)mesh.faces.size();i++)
{
  // Aplicar as transformações aos vértices
    
    Vector4 transformed_vertices[3];
    Vector4 projected_points[3];

    Vector3 face_vertices[3];
    face_vertices[0] = mesh.vertices[mesh.faces[i].a - 1];
    face_vertices[1] = mesh.vertices[mesh.faces[i].b - 1];
    face_vertices[2] = mesh.vertices[mesh.faces[i].c - 1];


     for (int v = 0; v < 3; v++) 
     {
            Vector4 transformed_vertex = vec4_from_vec3(face_vertices[v]);
             transformed_vertex = mat4_mul_vec4(model, transformed_vertex);
             transformed_vertex = mat4_mul_vec4(view, transformed_vertex);
             transformed_vertices [v] = transformed_vertex;
             projected_points[v] = mat4_mul_vec4(projection, transformed_vertex);
             
             // Perform perspective divide
                if (projected_points[v].w != 0) 
                {
                    projected_points[v].x /= projected_points[v].w;
                    projected_points[v].y /= projected_points[v].w;
                    projected_points[v].z /= projected_points[v].w;
                }

                // Flip vertically since the y values of the 3D mesh grow bottom->up and in screen space y values grow top->down
                projected_points[v].y *= -1;

                // Scale into the view
                projected_points[v].x *= (get_window_width() / 2.0);
                projected_points[v].y *= (get_window_height() / 2.0);

                // Translate the projected points to the middle of the screen
                projected_points[v].x += (get_window_width() / 2.0);
                projected_points[v].y += (get_window_height() / 2.0);

      }

        Vector3 face_normal = get_triangle_normal(transformed_vertices);

        // Find the vector between vertex A in the triangle and the camera origin
        Vector3 camera_ray = vec3_sub(Vector3(0.0f, 0.0f, 0.0f), vec3_from_vec4(transformed_vertices[0]));

        // Calculate how aligned the camera ray is with the face normal (using dot product)
        float dot_normal_camera = vec3_dot(face_normal, camera_ray);

        // Backface culling, bypassing triangles that are looking away from the camera
        if (dot_normal_camera < 0)
        {
           continue;
        }


   
          Texcoods uv0 = mesh.faces[i].a_uv;
          Texcoods uv1 = mesh.faces[i].b_uv;
          Texcoods uv2 = mesh.faces[i].c_uv;



        float light_intensity_factor = -vec3_dot(face_normal, Vector3(0.0f,0.0,1.0f));

        // Calculate the triangle color based on the light angle
        uint32_t triangle_color = apply_light_intensity(0xFFFF00FF, light_intensity_factor);


        //   draw_filled_triangle(
        //     projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w,
        //     projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w,
        //      projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w, triangle_color);


        draw_textured_triangle(
            projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w,uv0.u,uv0.v,
            projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w,uv1.u,uv1.v,
            projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w,uv2.u,uv2.v,
            &mesh.texture);
            triangles_to_render_count++;

 
        //    draw_triangle(
        //     projected_points[0].x, projected_points[0].y, 
        //     projected_points[1].x, projected_points[1].y,
        //     projected_points[2].x, projected_points[2].y,  
        //    0xFF0000FF);

         




}

 
    render_color_buffer();


         frameCount++;
        if (SDL_GetTicks() - startTime >= 1000)
        {
            char title[150];

                snprintf(title, sizeof( title), "  FPS: %d TRIS: %d ", frameCount,triangles_to_render_count);

            SDL_SetWindowTitle(window, title);
            frameCount = 0;
            startTime = SDL_GetTicks();
        }


}


free(colorbuffer);
free(zbuffer);
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();


    return 0;
}