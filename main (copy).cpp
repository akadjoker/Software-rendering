
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




Vector3 cube_vertices[8] = {
       Vector3(-0.5f, -0.5f, -0.5f),
       Vector3(-0.5f, -0.5f,  0.5f),
       Vector3(-0.5f,  0.5f, -0.5f),
       Vector3(-0.5f,  0.5f,  0.5f),
       Vector3( 0.5f, -0.5f, -0.5f),
       Vector3( 0.5f, -0.5f,  0.5f),
       Vector3( 0.5f,  0.5f, -0.5f),
       Vector3( 0.5f,  0.5f,  0.5f)};

     int faces[12][3] = {
        {0, 1, 2}, {2, 1, 3},
        {4, 5, 6}, {6, 5, 7},
        {0, 2, 4}, {4, 2, 6},
        {1, 3, 5}, {5, 3, 7},
        {2, 3, 6}, {6, 3, 7},
        {0, 1, 4}, {4, 1, 5}
    };

 FPSCamera camera;
 camera.GetViewMatrix();

  set_render_method(RENDER_TEXTURED);
  set_cull_method(CULL_BACKFACE);

    // Initialize the scene light d

Mesh mesh;
// mesh.load("assets/cube.obj",1.0f);
// mesh.texture.load("assets/pikuma.png");

mesh.load("assets/f22.obj",1.0f);
mesh.texture.load("assets/f22.png");

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
          camera.ProcessKeyboard(event.key.keysym.sym, 0.03f);
          break;
        case SDL_KEYUP:
          //camera.ProcessKeyboard(event.key.keysym.sym, false);
          break;
        case SDL_MOUSEMOTION:
          camera.ProcessMouseMovement(event.motion.xrel, event.motion.yrel);
          break;
        case SDL_MOUSEWHEEL:
          camera.ProcessMouseScroll(event.wheel.y);
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


    clear_color_buffer(0xFF000000);
    clear_z_buffer();
    draw_grid();



Vector3 position = Vector3(0.0f, 0.0f, -25.0f);



// Criar as matrizes de projeção, visualização e modelo
Matrix projection = Matrix::perspective(radians(45.0f), (float)get_window_width() / (float)get_window_height(), 0.1f, 100.0f);
Matrix view =  Matrix::lookAt(Vector3(0.0f, 0.0f, -5.0f), position, Vector3(0.0f, 1.0f, 0.0f));
Matrix model = Matrix::identity();


model = Matrix::identity();
model = Matrix::Rotate( (float)SDL_GetTicks()/1000.0f, 1.0f, 0.0f, 0.0f) * model;
model = Matrix::Rotate( (float)SDL_GetTicks()/1000.0f, 0.0f, 1.0f, 0.0f) * model;
model = Matrix::Rotate( (float)SDL_GetTicks()/1000.0f, 0.0f, 0.0f, 1.0f) * model;


    //   for (int i = 0; i < 12; i++) 
    //   {
    //       // Aplicar as transformações aos vértices
    //       Vector3 t0 =Vector3(cube_vertices[faces[i][0]]);
    //       Vector3 t1 =Vector3(cube_vertices[faces[i][1]]);
    //       Vector3 t2 =Vector3(cube_vertices[faces[i][2]]);

    //       Vector4 vtx[3];
    //       vtx[0]= projection * view * model * Vector4(t0.x,t0.y,t0.z, 1.0f);
    //       vtx[1]= projection * view * model * Vector4(t1.x,t1.y,t1.z, 1.0f);
    //       vtx[2]= projection * view * model * Vector4(t2.x,t2.y,t2.z, 1.0f);

    //       // Normalizar as coordenadas homogêneas
    //       vtx[0] /= vtx[0].w;
    //       vtx[1] /= vtx[1].w;
    //       vtx[2] /= vtx[2].w;

    //       SDL_Point p1 = { (int)((vtx[0].x + 1.0f) * (float)get_window_width()/2.0f), (int)((1.0f - vtx[0].y) * (float)get_window_height()/2.0f) };
    //       SDL_Point p2 = { (int)((vtx[1].x + 1.0f) * (float)get_window_width()/2.0f), (int)((1.0f - vtx[1].y) * (float)get_window_height()/2.0f) };
    //       SDL_Point p3 = { (int)((vtx[2].x + 1.0f) * (float)get_window_width()/2.0f), (int)((1.0f - vtx[2].y) * (float)get_window_height()/2.0f) };

    //       Vector3 face_normal = get_triangle_normal(vtx);

    // // Find the vector between vertex A in the triangle and the camera origin
    //         Vector3 camera_ray = vec3_sub(vec3_new(0, 0, 0), vec3_from_vec4(vtx[0]));

    //         // Calculate how aligned the camera ray is with the face normal (using dot product)
    //         float dot_normal_camera = vec3_dot(face_normal, camera_ray);

    //         // Backface culling, bypassing triangles that are looking away from the camera
    //         if (dot_normal_camera > 0)
    //         {
    //             continue;
    //         }

    //       draw_filled_triangle(
    //        p1.x,p1.y, vtx[0].z, vtx[0].w,
    //        p2.x,p2.y, vtx[1].z, vtx[1].w,
    //        p3.x,p3.y, vtx[2].z, vtx[2].w, 0xFF0000FF);

    //       //  draw_triangle(
    //       //   p1.x,p1.y, 
    //       //  p2.x,p2.y, 
    //       //  p3.x,p3.y,  0xFF0000FF);


           
    
    //   }

if (mesh.faces.size()>=3 && mesh.vertices.size()>=3)
 for (int i=0; i < (int)mesh.faces.size();i++)
{
  // Aplicar as transformações aos vértices
    
     Vector3 transformed_vertices[3];

     for (int v = 0; v < 3; v++) 
     {
            Vector4 transformed_vertex = vec4_from_vec3(mesh.vertices[v]);
            

            // Create a World Matrix combining scale, rotation, and translation matrices
            world_matrix = mat4_identity();

            // Order matters: First scale, then rotate, then translate. [T]*[R]*[S]*v
            world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
            world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

            // Multiply the world matrix by the original vector
            transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

            // Multiply the view matrix by the vector to transform the scene to camera space
            transformed_vertex = mat4_mul_vec4(view_matrix, transformed_vertex);

            // Save transformed vertex in the array of transformed vertices
            transformed_vertices[v] = transformed_vertex;
        }


          Vector3 t0 = mesh.vertices[mesh.faces[i].a - 1];
          Vector3 t1 = mesh.vertices[mesh.faces[i].b - 1];
          Vector3 t2 = mesh.vertices[mesh.faces[i].c - 1];

          Texcoods uv0 = mesh.faces[i].a_uv;
          Texcoods uv1 = mesh.faces[i].b_uv;
          Texcoods uv2 = mesh.faces[i].c_uv;
          

          Vector4 vtx[3];
          vtx[0]= projection * view * model * Vector4(t0.x,t0.y,t0.z, 1.0f);
          vtx[1]= projection * view * model * Vector4(t1.x,t1.y,t1.z, 1.0f);
          vtx[2]= projection * view * model * Vector4(t2.x,t2.y,t2.z, 1.0f);

          // Normalizar as coordenadas homogêneas
          vtx[0] /= vtx[0].w;
          vtx[1] /= vtx[1].w;
          vtx[2] /= vtx[2].w;

          SDL_Point p1 = { (int)((vtx[0].x + 1.0f) * (float)get_window_width()/2.0f), (int)((1.0f - vtx[0].y) * (float)get_window_height()/2.0f) };
          SDL_Point p2 = { (int)((vtx[1].x + 1.0f) * (float)get_window_width()/2.0f), (int)((1.0f - vtx[1].y) * (float)get_window_height()/2.0f) };
          SDL_Point p3 = { (int)((vtx[2].x + 1.0f) * (float)get_window_width()/2.0f), (int)((1.0f - vtx[2].y) * (float)get_window_height()/2.0f) };

          Vector3 face_normal = get_triangle_normal(vtx);

    // Find the vector between vertex A in the triangle and the camera origin
            Vector3 camera_ray = vec3_sub(position, vec3_from_vec4(vtx[0]));

            // Calculate how aligned the camera ray is with the face normal (using dot product)
            float dot_normal_camera = vec3_dot(face_normal, camera_ray);

            // Backface culling, bypassing triangles that are looking away from the camera
            if (dot_normal_camera > 0)
            {
                continue;
            }

          // draw_filled_triangle(
          //  p1.x,p1.y, vtx[0].z, vtx[0].w,
          //  p2.x,p2.y, vtx[1].z, vtx[1].w,
          //  p3.x,p3.y, vtx[2].z, vtx[2].w, 0xFF0000FF);

draw_textured_triangle(
    p1.x,p1.y, vtx[0].z, vtx[0].w,uv0.u,uv0.v,
    p2.x,p2.y, vtx[1].z, vtx[1].w,uv1.u,uv1.v,
    p3.x,p3.y, vtx[2].z, vtx[2].w,uv2.u,uv2.v,
    &mesh.texture);

          //  draw_triangle(
          //   p1.x,p1.y, 
          //  p2.x,p2.y, 
          //  p3.x,p3.y,  0xFF0000FF);


}
    render_color_buffer();
//   SDL_SetRenderDrawColor(renderer, 0, 0, 45, 255);
//   SDL_RenderClear(renderer);



//    // Criar as matrizes de projeção, visualização e modelo
//     Matrix projection = Matrix::perspective(radians(45.0f), (float)get_window_width() / (float)get_window_height(), 0.1f, 100.0f);
//     Matrix view = camera.GetViewMatrix();// Matrix::lookAt(Vector3(0.0f, 0.0f, -5.0f), Vector3(0.0f, 0.0f, -25.0f), Vector3(0.0f, 1.0f, 0.0f));
//     Matrix model = Matrix::identity();
  


// SDL_RenderPresent(renderer);
}


free(colorbuffer);
free(zbuffer);
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();


    return 0;
}