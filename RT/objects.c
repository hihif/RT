/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhihi <fhihi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:20:35 by fhihi             #+#    #+#             */
/*   Updated: 2023/07/25 15:27:02 by fhihi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"head.h"

double hit_sphere(t_point3 center, double radius, t_ray r)
{
    t_vec3 oc = vec3_subtract(&r.orig, &center);
    double a = vec3_dot_product(&r.dir, &r.dir);
    double half_b = vec3_dot_product(&oc, &r.dir);
    double c = vec3_length_squared(&oc) - radius * radius;
    double discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return -1.0;
    else
        return (-half_b - sqrt(discriminant) ) / a;
}

void hit_record_set_face_normal(t_hit_record* rec, const t_ray* r, const t_vec3* outward_normal)
{
    t_vec3 direction;

    vec3_init_with_values(&direction, r->dir.e[0], r->dir.e[1], r->dir.e[2]);
    rec->front_face = vec3_dot_product(&direction, outward_normal) < 0 ? 1 : 0;
    if (rec->front_face)
        vec3_init_with_values(&(rec->normal), outward_normal->e[0], outward_normal->e[1], outward_normal->e[2]);
    else
        rec->normal = vec3_negate(outward_normal);
}

int sphere_hit(const t_hittable* obj, const t_ray* r, double t_min, double t_max, t_hit_record* rec)
{
	t_sphere *sphere = (t_sphere *)obj;
    t_vec3 oc;
    t_vec3 temp;
	temp = ray_origin(*r);
    oc = vec3_subtract(&temp, &(sphere->center));
	temp = ray_direction(*r);
    double a = vec3_length_squared(&temp);
    double half_b = vec3_dot_product(&oc, &temp);
    double c = vec3_length_squared(&oc) - (sphere->radius * sphere->radius);

    double discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return 0;

    double sqrtd = sqrt(discriminant);

    double root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return 0;
    }

    rec->t = root;
    rec->p = ray_at(*r, rec->t);
	t_vec3 outward_normal = vec3_subtract(&rec->p, &sphere->center);
	vec3_divide(&outward_normal, sphere->radius);
    rec->normal = vec3_subtract(&(rec->p), &(sphere->center));
    vec3_divide(&(rec->normal), sphere->radius);
	hit_record_set_face_normal(rec, r, &outward_normal);
	rec->mat = sphere->mat;
    return 1;
}

int plane_hit(const t_hittable* obj, const t_ray* r, double t_min, double t_max, t_hit_record* rec)
{
    t_plane* plane = (t_plane*)obj;
    
    // Calculate the denominator of the ray-plane intersection formula
    double denominator = vec3_dot_product(&(plane->normal), &(r->dir));
    
    // Check if the ray is parallel or almost parallel to the plane
    if (fabs(denominator) < 1e-6)
        return 0;
    
    // Calculate the numerator of the ray-plane intersection formula
    t_vec3 origin_to_point = vec3_subtract(&(plane->point), &(r->orig));
    double numerator = vec3_dot_product(&origin_to_point, &(plane->normal));
    
    // Calculate the intersection parameter t
    double t = numerator / denominator;
    
    // Check if the intersection is within the valid range
    if (t < t_min || t > t_max)
        return 0;
    
    // Store the intersection information in the hit record
    rec->t = t;
    rec->p = ray_at(*r, t);
    rec->normal = plane->normal;
    hit_record_set_face_normal(rec, r, &(plane->normal));
	rec->mat = plane->mat;
    return 1;
}

int cylinder_hit(const t_hittable* obj, const t_ray* r, double t_min, double t_max, t_hit_record* rec)
{
    t_cylinder* cylinder = (t_cylinder*)obj;

    // Transform the ray into the local coordinate system of the cylinder
    t_vec3 local_origin = vec3_subtract(&r->orig, &cylinder->center);
    t_ray local_ray = {local_origin, r->dir};

    double a = local_ray.dir.e[0] * local_ray.dir.e[0] + local_ray.dir.e[2] * local_ray.dir.e[2];
    double half_b = local_ray.orig.e[0] * local_ray.dir.e[0] + local_ray.orig.e[2] * local_ray.dir.e[2];
    double c = local_ray.orig.e[0] * local_ray.orig.e[0] + local_ray.orig.e[2] * local_ray.orig.e[2] - cylinder->radius * cylinder->radius;

    double discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return 0;

    double sqrtd = sqrt(discriminant);

    double root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return 0;
    }

    double hit_y = local_ray.orig.e[1] + root * local_ray.dir.e[1];
    if (hit_y < 0 || hit_y > cylinder->height)
        return 0;

    rec->t = root;
    rec->p = ray_at(*r, rec->t);

    // Calculate the outward normal at the point of intersection
    t_vec3 outward_normal = {rec->p.e[0] - cylinder->center.e[0], 0, rec->p.e[2] - cylinder->center.e[2]};
    hit_record_set_face_normal(rec, r, &(outward_normal));
    rec->mat = cylinder->mat;
    return 1;
}

int cone_hit(const t_hittable* obj, const t_ray* r, double t_min, double t_max, t_hit_record* rec)
{
    t_cone* cone = (t_cone*)obj;
    
    // Precompute some values
    double tan_theta = tan(cone->theta);
    double tan_theta_squared = tan_theta * tan_theta;

    // Compute the quadratic coefficients
    double a = r->dir.e[0] * r->dir.e[0] + r->dir.e[2] * r->dir.e[2] - tan_theta_squared * r->dir.e[1] * r->dir.e[1];
    double b = 2 * (r->dir.e[0] * r->orig.e[0] + r->dir.e[2] * r->orig.e[2] - tan_theta_squared * r->dir.e[1] * r->orig.e[1]);
    double c = r->orig.e[0] * r->orig.e[0] + r->orig.e[2] * r->orig.e[2] - tan_theta_squared * r->orig.e[1] * r->orig.e[1];
    
    // Solve the quadratic equation
    double discriminant = b*b - 4*a*c;
    if (discriminant < 0)
        return 0;
    double sqrt_discriminant = sqrt(discriminant);
    
    // Find the two intersection t values
    double t0 = (-b - sqrt_discriminant) / (2*a);
    double t1 = (-b + sqrt_discriminant) / (2*a);
    
    // Ensure t0 is the smaller t value
    if (t0 > t1)
    {
        double temp = t0;
        t0 = t1;
        t1 = temp;
    }
    
    // Check if the intersection is within the valid range
    if (t0 > t_max || t1 < t_min)
        return 0;
    double t = (t0 < t_min) ? t1 : t0;
    
    // Store the intersection information in the hit record
    rec->t = t;
    rec->p = ray_at(*r, t);
    // For simplicity, normal calculation is omitted here, but it would involve some calculus.
    // rec->normal = ...;
    // hit_record_set_face_normal(rec, r, &(rec->normal));
    rec->mat = cone->mat;
    return 1;
}

t_cone* create_cone(double theta, t_material* mat)
{
    t_cone* cone = (t_cone*) malloc(sizeof(t_cone));
	cone->base.hit_func = cone_hit;
    cone->theta = theta;
    cone->mat = mat;
    return cone;
}

t_plane* create_plane(t_point3 point, t_vec3 normal, t_material *mat)
{
    t_plane* plane = (t_plane*)malloc(sizeof(t_plane));
    plane->base.hit_func = plane_hit;
    plane->point = point;
    plane->normal = normal;
	plane->mat = mat;
    return plane;
}

t_sphere* create_sphere(t_point3 center, double radius, t_material *mat)
{
    t_sphere* sphere = (t_sphere*)malloc(sizeof(t_sphere));
    sphere->base.hit_func = sphere_hit;
    sphere->center = center;
    sphere->radius = radius;
	sphere->mat = mat;
    return sphere;
}

t_cylinder* create_cylinder(t_point3 center, double height, double diameter, t_material *mat)
{
    t_cylinder* cylinder = (t_cylinder*)malloc(sizeof(t_cylinder));
    cylinder->base.hit_func = cylinder_hit;
    cylinder->center = center;
    cylinder->radius = diameter / 2.0;
    cylinder->height = height;
    cylinder->mat = mat;
    return cylinder;
}

int lambertian_scatter(const t_material* material, const t_ray* r_in, const t_hit_record* rec, t_color* attenuation, t_ray* scattered)
{
    const t_lambertian* lamb = (const t_lambertian*)material; // Cast material to t_lambertian

	t_vec3 tmp = random_unit_vector();
    t_vec3 scatter_direction = vec3_add2(&rec->normal, &tmp);
	if (near_zero(&scatter_direction))
        scatter_direction = rec->normal;
    *scattered = create_ray(rec->p, scatter_direction);
    *attenuation = lamb->albedo;
    return 1;
}

t_lambertian* create_lambertian(const t_color *a)
{
    t_lambertian* lamb = malloc(sizeof(t_lambertian));
	lamb->base.vtable = malloc(sizeof(t_material_vtable));
    lamb->base.vtable->scatter = lambertian_scatter; // Initialize vtable to NULL if not used
    lamb->albedo = *a;
    return lamb;
}