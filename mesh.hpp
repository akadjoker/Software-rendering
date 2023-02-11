/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 17:21:21 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/02/09 19:46:14 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#pragma once
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdint.h>
#include <cmath>
#include <vector>
#include "vector.hpp"
#include "matrix.hpp"
#include "math.hpp"
#include "render.hpp"


class Mesh
{

 public:
    std::vector<Vector3> vertices;         
    std::vector<Face>    faces;            
    Texture texture;  

public:
        Mesh(){};
        ~Mesh()
        {

            
        };
        
        
        bool load(const std::string &filename, float scale)
        {
        FILE* file;
        file = fopen(filename.c_str(), "r");
        if (!file)
            return false;
        char line[1024];
        std::vector<Texcoods> texcoords;
        
            while (fgets(line, 1024, file)) 
            {
                // Vertex information
                if (strncmp(line, "v ", 2) == 0) 
                {
                    Vector3 vertex ;
                    sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                    vertices.push_back(vertex);
                    
                }
                // Texture coordinate information
                if (strncmp(line, "vt ", 3) == 0) 
                {
                    Texcoods texcoord ;
                    sscanf(line, "vt %f %f", &texcoord.u, &texcoord.v);
                    texcoords.push_back(texcoord);
                 }
                // Face information
                if (strncmp(line, "f ", 2) == 0)
                {
                    int vertex_indices[3];
                    int texture_indices[3];
                    int normal_indices[3];
                    sscanf(
                        line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                        &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                        &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                        &vertex_indices[2], &texture_indices[2], &normal_indices[2]
                    );
             
                    Face face;
                    face.a = vertex_indices[0];
                    face.b = vertex_indices[1];
                    face.c = vertex_indices[2];
                    face.a_uv = texcoords[texture_indices[0] - 1];
                    face.b_uv = texcoords[texture_indices[1] - 1];
                    face.c_uv = texcoords[texture_indices[2] - 1];
                    face.color = 0xFFFFFFFF;
                    
                    faces.push_back(face);
                }
            }
            fclose(file);
            return true;
        }      
    
};
