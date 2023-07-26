/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhihi <fhihi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 12:46:00 by fhihi             #+#    #+#             */
/*   Updated: 2023/07/26 13:18:58 by fhihi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"head.h"

t_point3	point_on_vec(t_camera cam)
{
	t_point3	lookat;
	
	lookat = (t_point3)vec3_add2(&cam.origin, &cam.o_vec);
	return (lookat);
}

double degrees_to_radians(t_info *info, double degrees) {
    return (degrees * info->pi / 180.0);
}

void	window(t_info **info)
{
	int	end;
	int	bits;
	int	line;

	*info = (t_info *)malloc(sizeof(t_info ));
	if (!*info)
		return ;
	line = (*info)->line_bytes;
	bits = (*info)->bits;
	end = (*info)->endian;
	(*info)->mlx_ptr = mlx_init();
	(*info)->win_ptr = mlx_new_window((*info)->mlx_ptr, WIDTH, HEIGHT, "SCENE");
	(*info)->img_ptr = mlx_new_image((*info)->mlx_ptr, WIDTH, HEIGHT);
	(*info)->buffer = mlx_get_data_addr((*info)->img_ptr, &bits, &line, &end);
	(*info)->line_bytes = line;
	(*info)->endian = end;
	(*info)->bits = bits;
	(*info)->infinity = INFINITY;
	(*info)->spp = 100;
	(*info)->pi = 3.1415926535897932385;
}

t_vec3	get_lower_left_corner(t_vec3 orig, t_vec3 h, t_vec3 v, t_vec3 w)
{
	t_vec3 tmp;
	t_vec3 llc;

	vec3_divide(&v, 2);
	vec3_divide(&h, 2);
	tmp = vec3_subtract(&orig, &h);
	tmp = vec3_subtract(&tmp, &v);
	llc = vec3_subtract(&tmp, &w);
	return (llc);
}

void	init_camera(t_info *info)
{
    t_point3 lookat = point_on_vec(info->camera);
    t_vec3   vup = { 0, 1, 0 };
	double theta = degrees_to_radians(info, info->camera.fov);
	double h = tan(theta / 2);
	const double aspect_ratio = 16.0 / 9.0;
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;

	t_vec3 w = vec3_unit_vector(vec3_subtract(&info->camera.origin, &lookat));
	t_vec3 u = vec3_unit_vector(vec3_cross_product(&vup, &w));
	t_vec3 v = vec3_cross_product(&w, &u);

	(info)->camera.horizontal = vec3_multiply_scalar(viewport_width, &u);
	(info)->camera.vertical = vec3_multiply_scalar(viewport_height, &v);
	(info)->camera.lower_left_corner = get_lower_left_corner(info->camera.origin, info->camera.horizontal, info->camera.vertical, w);
}

void	parse_camera(t_info *info)
{
	info->camera.fov = 45.0;
	info->camera.origin = (t_point3){ -13, 2, -2 };
	info->camera.o_vec = (t_vec3){13, -2, 2};
}

void	init_camera_window_light(t_info **info)
{
	window(info);
	parse_camera(*info);
	init_camera(*info);
}