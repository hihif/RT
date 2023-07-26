/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhihi <fhihi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:39:59 by fhihi             #+#    #+#             */
/*   Updated: 2023/07/25 18:44:07 by fhihi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"head.h"

void	ft_pixel_color(int x, int y, t_color color, void *param)
{
	t_info	*data;
	double c[4];
	int		pixel;

	data = param;
	double scale = 1.0 / data->spp;

	c[0] = sqrt(0.0 * scale);
	c[1] = sqrt(color.e[0] * scale);
	c[2] = sqrt(color.e[1] * scale);
	c[3] = sqrt(color.e[2] * scale);
	pixel = ((y * data->line_bytes) + (x * 4));
	if (data->endian == 1)
	{
		data->buffer[pixel + 0] = (int)(clamp(c[0], 0, 0.99) * 255.99);
		data->buffer[pixel + 1] = (int)(clamp(c[1], 0, 0.99) * 255.99);
		data->buffer[pixel + 2] = (int)(clamp(c[2], 0, 0.99) * 255.99);
		data->buffer[pixel + 3] = (int)(clamp(c[3], 0, 0.99) * 255.99);
	}
	else if (data->endian == 0)
	{
		data->buffer[pixel + 0] = (int)(clamp(c[3], 0, 0.99) * 255.99);
		data->buffer[pixel + 1] = (int)(clamp(c[2], 0, 0.99) * 255.99);
		data->buffer[pixel + 2] = (int)(clamp(c[1], 0, 0.99) * 255.99);
		data->buffer[pixel + 3] = (int)(clamp(c[0], 0, 0.99) * 255.99);
	}
}

int mirror_y_axis(int y, int image_height) {
    return image_height - 1 - y;
}

t_hittable_list	*ft_scene(t_info **info, char *file)
{
	t_hittable_list* world = create_hittable_list();

	init_camera_window_light(info);
    t_material* material2 = (t_material *)create_lambertian(&(t_color){0.9, 0.6, 0.5});
    t_cylinder* sphere2 = create_cylinder((t_point3){0, 0, 0}, 4.0, 3.0, material2);
    hittable_list_add(world, (t_hittable*)sphere2);

    t_material* material3 = (t_material *)create_lambertian(&(t_color){0.5, 0.5, 0.5});
    t_plane* sphere3 = create_plane((t_point3){0, -1, 0}, (t_vec3){0, 1, 0}, material3);
    hittable_list_add(world, (t_hittable*)sphere3);

    t_material* material4 = (t_material *)create_lambertian(&(t_color){0.4, 0.2, 0.1});
    t_sphere* sphere4 = create_sphere((t_point3){-4, 0, 0}, 1, material4);
    hittable_list_add(world, (t_hittable*)sphere4);
	return (world);
}

int main(int ac, char *av[]) {
	t_info *info;
	t_color color;
	t_vec3 dir;
	int max_depth = 50;

	//world
	if (ac == 2)
	{
		t_hittable_list* world = ft_scene(&info, av[1]);
		
		for (double j = HEIGHT - 1; j >= 0; --j) {
			dprintf(2, "\rScanlines remaining: %d", (int)j);
			fflush(stderr);
			for (double i = 0; i < WIDTH; ++i) {
				vec3_init_with_values(&color, 0, 0, 0);
				for(int s = 0; s < info->spp; ++s) {
					double u = (i ) / (WIDTH - 1);
    	        	double v = (j ) / (HEIGHT - 1);
					dir = ray_dir(&(info->camera), u, v);
					t_ray r = create_ray(info->camera.origin, dir);
					color = add_colors(ray_color1(info, r, world, max_depth), color);
				}
				double j1 = (double)mirror_y_axis((int)j, HEIGHT);
				ft_pixel_color(i, j1, color, info);
			}
		}
		mlx_put_image_to_window(info->mlx_ptr, info->win_ptr, info->img_ptr, 0, 0);
		mlx_loop(info->mlx_ptr);
	}
	return (0);
}
