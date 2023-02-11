/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 15:53:50 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/02/10 18:34:42 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
#include <cmath>
#include <array>
#include "vector.hpp"



 struct Matrix
{
    float m[4][4];
  Matrix() 
  {
    Identity();
  }
Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
               float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
    set(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
}
 void Identity() 
  {
 
    for (int i = 0; i < 4; i++) 
    {
      for (int j = 0; j < 4; j++) 
      {
        m[i][j] = (i == j) ? 1.0f : 0.0f;
      }
    }
    
  }
  
void set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
                 float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
    m[0][0]  = m11;
    m[0][1]  = m21;
    m[0][2]  = m31;
    m[0][3]  = m41;
  
    m[1][0]  = m12;
    m[1][1]  = m22;
    m[1][2]  = m32;
    m[1][3]  = m42;
   
    m[2][0] = m13;
    m[2][1] = m23;
    m[2][2] = m33;
    m[2][3] = m43;
 
    m[3][0] = m14;
    m[3][1] = m24;
    m[3][2] = m34;
    m[3][3] = m44;
}
  static Matrix identity() 
  {
    Matrix matrix;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        matrix.m[i][j] = (i == j) ? 1.0f : 0.0f;
      }
    }
    return matrix;
  }

static Matrix translation(float x, float y, float z)
{
    Matrix mat;
    mat.m[0][3] = x;
    mat.m[1][3] = y;
    mat.m[2][3] = z;
    return mat;
}
 static Matrix Rotate(float angle, float x, float y, float z) 
 {
    float c = cos(angle);
    float s = sin(angle);

    Matrix result;
    result.m[0][0] = x * x * (1 - c) + c;
    result.m[0][1] = y * x * (1 - c) + z * s;
    result.m[0][2] = x * z * (1 - c) - y * s;
    result.m[0][3] = 0.0f;

    result.m[1][0] = x * y * (1 - c) - z * s;
    result.m[1][1] = y * y * (1 - c) + c;
    result.m[1][2] = y * z * (1 - c) + x * s;
    result.m[1][3] = 0.0f;

    result.m[2][0] = x * z * (1 - c) + y * s;
    result.m[2][1] = y * z * (1 - c) - x * s;
    result.m[2][2] = z * z * (1 - c) + c;
    result.m[2][3] = 0.0f;

    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = 0.0f;
    result.m[3][3] = 1.0f;

    return result;
  }

static Matrix lookAt( const Vector3 &eye, const Vector3 &center, const Vector3 &up)
{
    Vector3 z = (center - eye).normalize();
    Vector3 x = (up.cross(z)).normalize();
    Vector3 y = z.cross(x);

    Matrix mat;
    mat.m[0][0] = x.x;
    mat.m[1][0] = x.y;
    mat.m[2][0] = x.z;
    mat.m[0][1] = y.x;
    mat.m[1][1] = y.y;
    mat.m[2][1] = y.z;
    mat.m[0][2] = z.x;
    mat.m[1][2] = z.y;
    mat.m[2][2] = z.z;
    mat.m[0][3] = -x.dot(eye);
    mat.m[1][3] = -y.dot(eye);
    mat.m[2][3] = -z.dot(eye);
    return mat;
}

static Matrix perspective(float fovy, float aspect, float zNear, float zFar)
{
    Matrix mat;
    float f = 1.0f / tan(fovy / 2.0f);
    mat.m[0][0] = f / aspect;
    mat.m[1][1] = f;
    mat.m[2][2] = (zFar + zNear) / (zNear - zFar);
    mat.m[2][3] = 2.0f * zFar * zNear / (zNear - zFar);
    mat.m[3][2] = -1.0f;
    mat.m[3][3] = 0.0f;
    return mat;
}
  Matrix operator*(const Matrix& other) const 
  {
    Matrix result;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        result.m[i][j] = 0.0f;
        for (int k = 0; k < 4; k++) {
          result.m[i][j] += m[i][k] * other.m[k][j];
        }
      }
    }
    return result;
  }

  Vector3 operator*(const Vector3& v) const {
    Vector3 result;
    result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
    result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
    result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
    return result;
  }

Vector4 operator*(const Vector4& v) const {
  Vector4 result;
  result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
  result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
  result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
  result.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
  return result;
}

};

inline Matrix mat4_zero(void) 
{
 
     Matrix m;

    for (int i = 0; i < 4; i++) 
    {
      for (int j = 0; j < 4; j++) 
      {
        m.m[i][j] = 0.0f;
      }
    }
    
    return m;
}

inline Matrix mat4_identity(void) 
{
    // | 1 0 0 0 |
    // | 0 1 0 0 |
    // | 0 0 1 0 |
    // | 0 0 0 1 |
     Matrix m;
    for (int i = 0; i < 4; i++) 
    {
      for (int j = 0; j < 4; j++) 
      {
        m.m[i][j] = (i == j) ? 1.0f : 0.0f;
      }
    }
    
    return m;
}

inline Matrix mat4_make_scale(float sx, float sy, float sz) {
    // | sx  0  0  0 |
    // |  0 sy  0  0 |
    // |  0  0 sz  0 |
    // |  0  0  0  1 |
    Matrix m = mat4_identity();
    m.m[0][0] = sx;
    m.m[1][1] = sy;
    m.m[2][2] = sz;
    return m;
}

inline Matrix mat4_make_translation(float tx, float ty, float tz) {
    // | 1  0  0  tx |
    // | 0  1  0  ty |
    // | 0  0  1  tz |
    // | 0  0  0  1  |
    Matrix m = mat4_identity();
    m.m[0][3] = tx;
    m.m[1][3] = ty;
    m.m[2][3] = tz;
    return m;
}

inline Matrix mat4_make_rotation_x(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    // | 1  0  0  0 |
    // | 0  c  s  0 |
    // | 0 -s  c  0 |
    // | 0  0  0  1 |
    Matrix m = mat4_identity();
    m.m[1][1] = c;
    m.m[1][2] = s;
    m.m[2][1] = -s;
    m.m[2][2] = c;
    return m;
}

inline Matrix mat4_make_rotation_y(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    // |  c  0 -s  0 |
    // |  0  1  0  0 |
    // |  s  0  c  0 |
    // |  0  0  0  1 |
    Matrix m = mat4_identity();
    m.m[0][0] = c;
    m.m[0][2] = -s;
    m.m[2][0] = s;
    m.m[2][2] = c;
    return m;
}

inline Matrix mat4_make_rotation_z(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    // |  c  s  0  0 |
    // | -s  c  0  0 |
    // |  0  0  1  0 |
    // |  0  0  0  1 |
    Matrix m = mat4_identity();
    m.m[0][0] = c;
    m.m[0][1] = s;
    m.m[1][0] = -s;
    m.m[1][1] = c;
    return m;
}

inline Vector4 mat4_mul_vec4(Matrix m, Vector4 v) 
{
    Vector4 result;
    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
    return result;
}

inline Matrix mat4_mul_mat4(Matrix a, Matrix b) 
{
    Matrix m;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
        }
    }
    return m;
}


inline Matrix mat4_perspective(float fovy, float aspect, float zNear, float zFar)
{
    Matrix mat;
    float f = 1.0f / tan(fovy / 2.0f);
    
    mat.m[0][0] = f / aspect;
    mat.m[1][1] = f;
    mat.m[2][2] = (zFar + zNear) / (zNear - zFar);
    mat.m[2][3] = 2.0f * zFar * zNear / (zNear - zFar);
    mat.m[3][2] = -1.0f;
    mat.m[3][3] = 0.0f;
    return mat;
}

inline Matrix mat4_look_at(Vector3 eye, Vector3 target, Vector3 up) {
    // Compute the forward (z), right (x), and up (y) vectors
    Vector3 z = vec3_sub(target, eye);
    vec3_normalize(&z);
    Vector3 x = vec3_cross(up, z);
    vec3_normalize(&x);
    Vector3 y = vec3_cross(z, x);

    // | x.x   x.y   x.z  -dot(x,eye) |
    // | y.x   y.y   y.z  -dot(y,eye) |
    // | z.x   z.y   z.z  -dot(z,eye) |
    // |   0     0     0            1 |
    Matrix mat;
        // { x.x, x.y, x.z, -vec3_dot(x, eye) },
        // { y.x, y.y, y.z, -vec3_dot(y, eye) },
        // { z.x, z.y, z.z, -vec3_dot(z, eye) },
        // {   0,   0,   0,                 1 }

    mat.m[0][0] = x.x;
    mat.m[1][0] = x.y;
    mat.m[2][0] = x.z;
    mat.m[3][0] = 0.0f;
    
    
    mat.m[0][1] = y.x;
    mat.m[1][1] = y.y;
    mat.m[2][1] = y.z;
    mat.m[3][1] = 0.0f;
    
    mat.m[0][2] = z.x;
    mat.m[1][2] = z.y;
    mat.m[2][2] = z.z;
    mat.m[3][2] = 0.0f;
    
    mat.m[0][3] =  -vec3_dot(x, eye);
    mat.m[1][3] =  -vec3_dot(y, eye);
    mat.m[2][3] =  -vec3_dot(z, eye);
    mat.m[3][3] = 1.0f;



    return mat;
}

