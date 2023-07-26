/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhihi <fhihi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:19:55 by fhihi             #+#    #+#             */
/*   Updated: 2023/07/26 13:26:16 by fhihi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAD_H
#define HEAD_H

#include<mlx.h>
#include<math.h>
#include<stdlib.h>
#include"../other/libft/libft.h"
#define HEIGHT 720
#define WIDTH 1280

typedef struct s_material t_material;

typedef struct s_vec3
{
	double e[3];
}			t_vec3;

typedef t_vec3 t_point3;   // 3D point
typedef t_vec3 t_color;    // RGB color

typedef struct s_ray{
	t_point3	orig;
	t_vec3		dir;
}				t_ray;

typedef struct s_camera 
{
	t_point3	origin;
	double		fov;
	t_vec3		o_vec;
	t_point3	lower_left_corner;
	t_vec3		horizontal;
	t_vec3		vertical;
}			t_camera;

typedef struct s_hit_record
{
	t_point3	p;
	t_vec3		normal;
	t_color		color;
	t_material *mat;
	double		t;
	double		u;
	double		v;
	int			front_face;
}				t_hit_record;


typedef struct s_material_vtable{
    int (*scatter)(
        const t_material* material, const t_ray* r_in, const t_hit_record* rec, t_color* attenuation, t_ray* scattered);
} t_material_vtable;

struct s_material {
    t_material_vtable* vtable;
};

typedef struct s_hittable
{
    int	(*hit_func)(const struct s_hittable* obj, const t_ray* r, double t_min, double t_max, t_hit_record* rec);
} 			t_hittable;

typedef struct s_sphere
{
    t_hittable	base;
	t_material *mat;
    t_point3	center;
    double		radius;
} 				t_sphere;

typedef struct s_plane
{
    t_hittable	base;
	t_material *mat;
    t_point3	point;
    t_vec3		normal;
} 				t_plane;

typedef struct s_cylander
{
    t_hittable	base;
	t_material *mat;
    t_point3	center;
    double		height;
    double		radius;
	t_vec3		vec;
} 				t_cylinder;

typedef struct s_cone
{
    t_hittable	base;
	double		theta;
	t_material	*mat;
}				t_cone;

typedef struct s_lambertian {
    t_material	base;
	t_color		albedo;
} 				t_lambertian;

typedef struct s_hittable_list{
    t_hittable base;
    t_hittable** objects;
    size_t size;
} t_hittable_list;

typedef struct	s_info 
{
	void			*mlx_ptr;
	void			*win_ptr;
	void			*img_ptr;
	char			*buffer;
	int				bits;
	int				line_bytes;
	int				endian;
	double			infinity;
	double			pi;
	int				spp; //sampels_per_pixel
	t_camera		camera;
}				t_info;

void	vec3_init(t_vec3 *v);
void	vec3_init_with_values(t_vec3 *v, double e0, double e1, double e2);
double	vec3_x(const t_vec3 *v);
double	vec3_y(const t_vec3 *v);
double	vec3_z(const t_vec3 *v);
t_vec3	vec3_negate(const t_vec3 *v);
double	vec3_get(const t_vec3 *v, int i);
void	vec3_set(t_vec3 *v, int i, double value);
void	vec3_add1(t_vec3 *v1, const t_vec3 *v2);
void	vec3_multiply1(t_vec3 *v, double t);
void	vec3_divide(t_vec3 *v, double t);
double	vec3_length(const t_vec3 *v);
double	vec3_length_squared(const t_vec3 *v);
void	vec3_output_stream(const t_vec3* v);
t_vec3	vec3_add2(const t_vec3* u, const t_vec3* v);
t_vec3	vec3_subtract(const t_vec3* u, const t_vec3* v);
t_vec3	vec3_multiply2(const t_vec3* u, const t_vec3* v);
t_vec3	vec3_multiply_scalar(double t, const t_vec3* v);
t_vec3	vec3_multiply_scalar_reverse(const t_vec3* v, double t);
t_vec3	vec3_divide_scalar(const t_vec3* v, double t);
double	vec3_dot_product(const t_vec3* u, const t_vec3* v);
t_vec3	vec3_cross_product(const t_vec3* u, const t_vec3* v);
t_vec3	vec3_unit_vector(t_vec3 v);
t_vec3 reflect(const t_vec3* v, const t_vec3* n);
double random_double();
double random_double_range(double min, double max);
t_vec3 vec3_random();
t_vec3 vec3_random_range(double min, double max);
t_vec3 random_in_unit_sphere();
t_vec3 random_unit_vector();
double clamp(double x, double min, double max);
int near_zero(const t_vec3* v);

t_color add_colors(t_color c1, t_color c2);
t_color ray_color1(t_info *info, t_ray r, const t_hittable_list* world, int depth);

t_ray create_ray(t_point3 origin, t_vec3 direction);
t_point3 ray_origin(t_ray r);
t_vec3 ray_direction(t_ray r);
t_point3 ray_at(t_ray r, double t);
static void	set_dir(t_vec3 *dir, t_camera *camera, double u, double v);
t_vec3 ray_dir(t_camera* cam, double u, double v);

double hit_sphere(t_point3 center, double radius, t_ray r);
void hit_record_set_face_normal(t_hit_record* rec, const t_ray* r, const t_vec3* outward_normal);
int sphere_hit(const t_hittable* obj, const t_ray* r, double t_min, double t_max, t_hit_record* rec);
int cone_hit(const t_hittable* obj, const t_ray* r, double t_min, double t_max, t_hit_record* rec);
int plane_hit(const t_hittable* obj, const t_ray* r, double t_min, double t_max, t_hit_record* rec);
int cone_hit(const t_hittable* obj, const t_ray* r, double t_min, double t_max, t_hit_record* rec);
t_plane* create_plane(t_point3 point, t_vec3 normal, t_material *mat);
t_sphere* create_sphere(t_point3 center, double radius, t_material *mat);
t_cylinder* create_cylinder(t_point3 center, double height, double diameter, t_material *mat);
t_cone* create_cone(double theta, t_material* mat);
int lambertian_scatter(const t_material* material, const t_ray* r_in, const t_hit_record* rec, t_color* attenuation, t_ray* scattered);
t_lambertian* create_lambertian(const t_color *a);

t_hittable_list* create_hittable_list();
void hittable_list_clear(t_hittable_list* list);
void hittable_list_add(t_hittable_list* list, t_hittable* object);
int hittable_list_hit(const t_hittable* obj, const t_ray* r, double t_min, double t_max, t_hit_record* rec);

void	init_camera_window_light(t_info **info);

#endif