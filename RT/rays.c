/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhihi <fhihi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:38:33 by fhihi             #+#    #+#             */
/*   Updated: 2023/07/21 15:52:39 by fhihi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"head.h"

t_ray create_ray(t_point3 origin, t_vec3 direction)
{
    t_ray r;
    r.orig = origin;
    r.dir = direction;
    return r;
}

t_point3 ray_origin(t_ray r)
{
    return r.orig;
}

t_vec3 ray_direction(t_ray r)
{
    return r.dir;
}

t_point3 ray_at(t_ray r, double t)
{
    t_point3 result;
    result.e[0] = r.orig.e[0] + t * r.dir.e[0];
    result.e[1] = r.orig.e[1] + t * r.dir.e[1];
    result.e[2] = r.orig.e[2] + t * r.dir.e[2];
    return result;
}

static void	set_dir(t_vec3 *dir, t_camera *camera, double u, double v)
{
	t_vec3 temp;
	t_vec3 temp1;
	t_vec3 temp2;
	
	temp1 = camera->horizontal;
	temp2 = camera->vertical;
	vec3_multiply1(&temp1, u);
	vec3_multiply1(&temp2, v);
	vec3_add1(dir, &camera->lower_left_corner);
	vec3_add1(dir, &temp1);
	vec3_add1(dir, &temp2);
	temp = vec3_negate(&camera->origin);
	vec3_add1(dir, &temp);
}

t_vec3 ray_dir(t_camera* cam, double u, double v)
{
	t_vec3 dir;

	vec3_init(&dir);
	set_dir(&dir, cam, u, v);
	return (dir);
}
