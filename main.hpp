/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 15:54:06 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/02/10 17:56:32 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
#include <cmath>
#include "vector.hpp"
#include "matrix.hpp"
#include "math.hpp"
#include "render.hpp"
#include "mesh.hpp"



#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

bool is_running = false;
int previous_frame_time = 0;
float delta_time = 0; 
int triangles_to_render_count = 0;
