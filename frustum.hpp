
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
#include "matrix.hpp"

struct Frustum 
{

  Vector4 planes[6];

  void update(const Matrix &projectionMatrix, const Matrix &viewMatrix) 
  {
     Matrix viewProj = projectionMatrix * viewMatrix;

    // Extrair os coeficientes dos planos do Frustum a partir da matriz de visão-projeção
    planes[0].x = viewProj.m[0][3] + viewProj.m[0][0];
    planes[0].y = viewProj.m[1][3] + viewProj.m[1][0];
    planes[0].z = viewProj.m[2][3] + viewProj.m[2][0];
    planes[0].w = viewProj.m[3][3] + viewProj.m[3][0];

    planes[1].x = viewProj.m[0][3] - viewProj.m[0][0];
    planes[1].y = viewProj.m[1][3] - viewProj.m[1][0];
    planes[1].z = viewProj.m[2][3] - viewProj.m[2][0];
    planes[1].w = viewProj.m[3][3] - viewProj.m[3][0];

    planes[2].x = viewProj.m[0][3] + viewProj.m[0][1];
    planes[2].y = viewProj.m[1][3] + viewProj.m[1][1];
    planes[2].z = viewProj.m[2][3] + viewProj.m[2][1];
    planes[2].w = viewProj.m[3][3] + viewProj.m[3][1];

    planes[3].x = viewProj.m[0][3] - viewProj.m[0][1];
    planes[3].y = viewProj.m[1][3] - viewProj.m[1][1];
    planes[3].z = viewProj.m[2][3] - viewProj.m[2][1];
    planes[3].w = viewProj.m[3][3] - viewProj.m[3][1];

    planes[4].x = viewProj.m[0][3] + viewProj.m[0][2];
    planes[4].y = viewProj.m[1][3] + viewProj.m[1][2];
    planes[4].z = viewProj.m[2][3] + viewProj.m[2][2];
    planes[4].w = viewProj.m[3][3] + viewProj.m[3][2];

    planes[5].x = viewProj.m[0][3] - viewProj.m[0][2];
    planes[5].y = viewProj.m[1][3] - viewProj.m[1][2];
    planes[5].z = viewProj.m[2][3] - viewProj.m[2][2];
    planes[5].w = viewProj.m[3][3] - viewProj.m[3][2];

// Normalizar os planos
      for (int i = 0; i < 6; i++)
      {
        float len = sqrt(planes[i].x * planes[i].x + planes[i].y * planes[i].y + planes[i].z * planes[i].z);
        planes[i].x /= len;
        planes[i].y /= len;
        planes[i].z /= len;
        planes[i].w /= len;
      }
  }

  
  // Verifica se um ponto está dentro do Frustum
  bool pointInFrustum(const Vector3 &p) const
   {
    for (int i = 0; i < 6; i++) {
      if (planes[i].x * p.x + planes[i].y * p.y + planes[i].z * p.z + planes[i].w <= 0) {
        return false;
      }
    }
    return true;
  }

  // Verifica se uma esfera está dentro do Frustum
  bool sphereInFrustum(const Vector3 &center, float radius) const
   {
    for (int i = 0; i < 6; i++) {
      if (planes[i].x * center.x + planes[i].y * center.y + planes[i].z * center.z + planes[i].w <= -radius) {
        return false;
      }
    }
    return true;
  }

  // Verifica se um triângulo está dentro do Frustum
  bool triangleInFrustum(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2) const
   {
    for (int i = 0; i < 6; i++) {
      if (planes[i].x * v0.x + planes[i].y * v0.y + planes[i].z * v0.z + planes[i].w <= 0 &&
          planes[i].x * v1.x + planes[i].y * v1.y + planes[i].z * v1.z + planes[i].w <= 0 &&
          planes[i].x * v2.x + planes[i].y * v2.y + planes[i].z * v2.z + planes[i].w <= 0) {
        return false;
      }
    }
    return true;
  }
  
};