/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 15:54:06 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/02/09 17:17:01 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
#include <cmath>

# define MAX_DOUBLE 9223372036854
# define EPSILON 0.00001
# define PI 3.14

inline float deg2rad(float degrees) {
    return degrees * (M_PI / 180.0f);
}

inline double	max_module(double a, double b)
{
	if (a * (double)(1 - (2 * (a < 0))) > b * (double)(1 - (2 * (b < 0))))
		return (a * (double)(1 - (2 * (a < 0))));
	else
		return (b * (double)(1 - (2 * (b < 0))));
}

inline double	min_module(double a, double b)
{
	if (a * (double)(1 - (2 * (a < 0))) < b * (double)(1 - (2 * (b < 0))))
		return (a * (double)(1 - (2 * (a < 0))));
	else
		return (b * (double)(1 - (2 * (b < 0))));
}

inline double	min(double a, double b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

inline double	max(double a, double b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

inline double	module(double a)
{
	if (a < 0)
		return (-1 * a);
	return (a);
}


inline double	radians(double degrees)
{
	return (degrees / 180 * PI);
}


inline bool	equal(double a, double b)
{
	return (module(a - b) < EPSILON);
}

inline void int_swap(int* a, int* b) 
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

inline void float_swap(float* a, float* b) 
{
    float tmp = *a;
    *a = *b;
    *b = tmp;
}