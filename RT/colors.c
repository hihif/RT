/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhihi <fhihi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:44:12 by fhihi             #+#    #+#             */
/*   Updated: 2023/07/26 14:23:30 by fhihi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"head.h"

t_color add_colors(t_color c1, t_color c2)
{
    t_color result = { c1.e[0] + c2.e[0], c1.e[1] + c2.e[1], c1.e[2] + c2.e[2] };
    return result;
}

t_color multiply_color_by_scalar(t_color c, double scalar)
{
    t_color result = {  c.e[0] * scalar, c.e[1] * scalar, c.e[2] * scalar };
    return result;
}



t_color ray_color1(t_info *info, t_ray r, const t_hittable_list* world, int depth)
{
	t_color temp1;
	t_hit_record rec;
	if (depth <= 0)
        return (t_color) { 1, 0, 0 };

	int t1 = hittable_list_hit((t_hittable *)world, &r, 0.001, info->infinity, &rec);
	if (t1)
	{
		t_color	c;
		t_ray	ray;
		int l = rec.mat->vtable->scatter(rec.mat, &r, &rec, &c, &ray);
		if (l){
			t_color tmp = ray_color1(info, ray, world, depth-1);
			return (vec3_multiply2(&tmp, &c));
		}
		return (t_color) { 0, 0 ,0 };
    }
    t_vec3 unit_direction = vec3_unit_vector(r.dir);
    rec.t = 0.5 * (unit_direction.e[1] + 1.0);
    t_color c1 = { 1, 1, 1 };
    t_color c2 = { 0.9, 0.3, 0.3 };
    t_color background_color = add_colors(multiply_color_by_scalar(c1, 1.0 - rec.t), multiply_color_by_scalar(c2, rec.t));
    return c1;
}