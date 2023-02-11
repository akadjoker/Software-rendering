/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 15:53:57 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/02/10 18:14:25 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
#include <cmath>


 struct Vector2
 {
    float x, y;
    Vector2() : x(0), y(0) {}
    Vector2(float x , float y ) : x(x), y(y) {}
     Vector2 operator+(const Vector2& v2) const {
      return Vector2(x + v2.x, y + v2.y);
    }

    Vector2 operator-(const Vector2& v2) const {
      return Vector2(x - v2.x, y - v2.y);
    }

    Vector2 operator*(float scalar) const {
      return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator/(float scalar) const {
      return Vector2(x / scalar, y / scalar);
    }

    float Magnitude() const {
      return std::sqrt(x * x + y * y);
    }

    void Normalize() 
    {
      float magnitude = Magnitude();
      if (magnitude > 0) {
        x /= magnitude;
        y /= magnitude;
      }
    }
} ;

 struct Vector3 
 {
    float x, y, z;
     Vector3() : x(0), y(0), z(0) {}
     Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
     

Vector3 cross(const Vector3 &v) const 
{
    return Vector3(y * v.z - z * v.y,
                   z * v.x - x * v.z,
                   x * v.y - y * v.x);
}

Vector3 normalize() const 
{
    float length = sqrt(x * x + y * y + z * z);
    return Vector3(x / length, y / length, z / length);
}

static Vector3 normalize(const Vector3 &v) 
{
float magnitude = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
return Vector3(v.x / magnitude, v.y / magnitude, v.z / magnitude);
}

static Vector3 cross(const Vector3 &a, const Vector3 &b) 
{
    return Vector3(a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x);
}

static float dot(const Vector3 &a, const Vector3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
    
float dot(const Vector3 &v) const {
    return x * v.x + y * v.y + z * v.z;
}

    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3 operator*(const Vector3& v) const {
        return Vector3(x * v.x, y * v.y, z * v.z);
    }

    Vector3 operator/(float scalar) const {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    Vector3& operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3& operator/=(float scalar) 
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
} ;

struct Vector4 
{
    float x, y, z, w;
    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

  Vector4 operator+(const Vector4& v) const {
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
  }

  Vector4 operator-(const Vector4& v) const {
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
  }

  Vector4 operator*(float s) const { return Vector4(x * s, y * s, z * s, w * s); }

  Vector4 operator/(float s) const { return Vector4(x / s, y / s, z / s, w / s); }

  float dot(const Vector4& v) const {    return x * v.x + y * v.y + z * v.z + w * v.w;
  }

  Vector4& operator/=(float scalar) 
  {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
  }


  Vector4 cross(const Vector4& v) const 
  {
    return Vector4(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x, 0);
  }

  float magnitude() const { return sqrt(x * x + y * y + z * z + w * w); }

  void normalize() 
  {
    float m = magnitude();
    x /= m;
    y /= m;
    z /= m;
    w /= m;
  }
  
} ;


inline Vector2 vec2_new(float x, float y) 
{
    return Vector2(x, y );
}

inline float vec2_length(Vector2 v)
 {
    return sqrt(v.x * v.x + v.y * v.y);
}

inline Vector2 vec2_add(Vector2 a, Vector2 b) 
{
 return Vector2(a.x + b.x,
               a.y + b.y);
}

inline Vector2 vec2_sub(Vector2 a, Vector2 b) {
   return Vector2( a.x - b.x,
        a.y - b.y);
    
}

inline Vector2 vec2_mul(Vector2 v, float factor)
 {
   return Vector2( v.x * factor, v.y * factor);
}

inline Vector2 vec2_div(Vector2 v, float factor)
{
   return Vector2( v.x / factor, v.y / factor);
}

inline float vec2_dot(Vector2 a, Vector2 b) 
{
    return (a.x * b.x) + (a.y * b.y);
}

inline void vec2_normalize(Vector2* v)
 {
    float length = sqrt(v->x * v->x + v->y * v->y);
    v->x /= length;
    v->y /= length;
}

inline  Vector4 vec4_from_vec3(Vector3 v) 
{
    return Vector4(v.x, v.y, v.z, 1.0 );

}


inline  Vector2 vec2_from_vec4(Vector4 v) 
{
    return Vector2(v.x, v.y);
   
}
inline  Vector3 vec3_from_vec4(Vector4 v) 
{
    return Vector3(v.x, v.y, v.z );
   
}

inline Vector3 vec3_new(float x, float y, float z) 
{
    return Vector3( x, y, z );
 
}

inline Vector3 vec3_clone(Vector3* v) 
{
  return Vector3( v->x, v->y, v->z );
  
}

inline float vec3_length(Vector3 v) 
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline Vector3 vec3_add(Vector3 a, Vector3 b) 
{
   return Vector3(
         a.x + b.x,
         a.y + b.y,
         a.z + b.z
  );
}

inline Vector3 vec3_sub(Vector3 a, Vector3 b) 
{
    return Vector3(
         a.x - b.x,
         a.y - b.y,
         a.z - b.z
  );
}

inline Vector3 vec3_mul(Vector3 v, float factor) {
    return Vector3(
         v.x * factor,
         v.y * factor,
         v.z * factor
   );
}

inline Vector3 vec3_div(Vector3 v, float factor) {
    return Vector3(
        v.x / factor,
        v.y / factor,
        v.z / factor
    );
}

inline Vector3 vec3_cross(Vector3 a, Vector3 b) {
    return Vector3(
         a.y * b.z - a.z * b.y,
         a.z * b.x - a.x * b.z,
         a.x * b.y - a.y * b.x
    );
    
}

inline float vec3_dot(Vector3 a, Vector3 b) 
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline void vec3_normalize(Vector3* v) 
{
    float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

inline Vector3 vec3_rotate_x(Vector3 v, float angle)
{
    return Vector3( v.x,
         v.y * cos(angle) - v.z * sin(angle),
         v.y * sin(angle) + v.z * cos(angle));
}

inline Vector3 vec3_rotate_y(Vector3 v, float angle) 
{
    return Vector3(
         v.x * cos(angle) + v.z * sin(angle),
         v.y,
        -v.x * sin(angle) + v.z * cos(angle));
 
}

inline Vector3 vec3_rotate_z(Vector3 v, float angle) 
{
   return Vector3(
         v.x * cos(angle) - v.y * sin(angle),
         v.x * sin(angle) + v.y * cos(angle),
         v.z   );
    
}

