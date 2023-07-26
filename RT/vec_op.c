/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_op.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhihi <fhihi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:28:34 by fhihi             #+#    #+#             */
/*   Updated: 2023/07/26 13:09:51 by fhihi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"head.h"

void vec3_init(t_vec3 *v)
{
    v->e[0] = 0;
    v->e[1] = 0;
    v->e[2] = 0;
}

void vec3_init_with_values(t_vec3 *v, double e0, double e1, double e2)
{
	v->e[0] = e0;
	v->e[1] = e1;
	v->e[2] = e2;
}

double vec3_x(const t_vec3 *v)
{
	return v->e[0];
}

double vec3_y(const t_vec3 *v)
{
	return v->e[1];
}

double vec3_z(const t_vec3 *v)
{
	return v->e[2];
}

t_vec3	vec3_negate(const t_vec3 *v)
{
	t_vec3 result;
	result.e[0] = -v->e[0];
	result.e[1] = -v->e[1];
	result.e[2] = -v->e[2];
	return result;
}

double vec3_get(const t_vec3 *v, int i)
{
	return v->e[i];
}

void vec3_set(t_vec3 *v, int i, double value)
{
	v->e[i] = value;
}

void vec3_add1(t_vec3 *v1, const t_vec3 *v2)
{
	v1->e[0] += v2->e[0];
	v1->e[1] += v2->e[1];
	v1->e[2] += v2->e[2];
}

void vec3_multiply1(t_vec3 *v, double t)
{
	v->e[0] *= t;
	v->e[1] *= t;
	v->e[2] *= t;
}

void vec3_divide(t_vec3 *v, double t)
{
	double inv_t = 1 / t;
	vec3_multiply1(v, inv_t);
}

double vec3_length(const t_vec3 *v)
{
	return sqrt(vec3_length_squared(v));
}

double vec3_length_squared(const t_vec3 *v)
{
	return v->e[0] * v->e[0] + v->e[1] * v->e[1] + v->e[2] * v->e[2];
}

// Output stream operator
void vec3_output_stream(const t_vec3* v)
{
	printf("%lf %lf %lf\n", v->e[0], v->e[1], v->e[2]);
}

// Vector addition
t_vec3 vec3_add2(const t_vec3* u, const t_vec3* v)
{
	t_vec3 result;
	result.e[0] = u->e[0] + v->e[0];
	result.e[1] = u->e[1] + v->e[1];
	result.e[2] = u->e[2] + v->e[2];
	return result;
}

// Vector subtraction
t_vec3 vec3_subtract(const t_vec3* u, const t_vec3* v)
{
	t_vec3 result;
	result.e[0] = u->e[0] - v->e[0];
	result.e[1] = u->e[1] - v->e[1];
	result.e[2] = u->e[2] - v->e[2];
	return result;
}

// Element-wise vector multiplication
t_vec3 vec3_multiply2(const t_vec3* u, const t_vec3* v)
{
	t_vec3 result;
	result.e[0] = u->e[0] * v->e[0];
	result.e[1] = u->e[1] * v->e[1];
	result.e[2] = u->e[2] * v->e[2];
	return result;
}

// Scalar-vector multiplication
t_vec3 vec3_multiply_scalar(double t, const t_vec3* v)
{
	t_vec3 result;
	result.e[0] = t * v->e[0];
	result.e[1] = t * v->e[1];
	result.e[2] = t * v->e[2];
	return result;
}

// Vector-scalar multiplication
t_vec3 vec3_multiply_scalar_reverse(const t_vec3* v, double t)
{
	return vec3_multiply_scalar(t, v);
}

// Vector-scalar division
t_vec3 vec3_divide_scalar(const t_vec3* v, double t)
{
	return vec3_multiply_scalar_reverse(v, 1 / t);
}

// Dot product
double vec3_dot_product(const t_vec3* u, const t_vec3* v)
{
	return u->e[0] * v->e[0] + u->e[1] * v->e[1] + u->e[2] * v->e[2];
}

// Cross product
t_vec3 vec3_cross_product(const t_vec3* u, const t_vec3* v)
{
	t_vec3 result;
	result.e[0] = (u->e[1] * v->e[2]) - (u->e[2] * v->e[1]);
	result.e[1] = (u->e[2] * v->e[0]) - (u->e[0] * v->e[2]);
	result.e[2] = (u->e[0] * v->e[1]) - (u->e[1] * v->e[0]);
	return result;
}

// Unit vector
t_vec3 vec3_unit_vector(t_vec3 v)
{
	double length = vec3_length(&v);
	return vec3_divide_scalar(&v, length);
}

t_vec3 reflect(const t_vec3* v, const t_vec3* n)
{
	t_vec3 reflection;
	double dot_product = vec3_dot_product(v, n);
	t_vec3 scaled_normal = vec3_multiply_scalar(2 * dot_product, n);
	reflection = vec3_subtract(v, &scaled_normal);
	return reflection;
}

double random_double()
{
	// Returns a random real in [0,1).
	return rand() / (RAND_MAX + 1.0);
}

double random_double_range(double min, double max)
{
	// Returns a random real in [min,max).
	return min + (max - min) * random_double();
}

t_vec3 vec3_random()
{
	t_vec3 result;
	result.e[0] = random_double();
	result.e[1] = random_double();
	result.e[2] = random_double();
	return result;
}

t_vec3 vec3_random_range(double min, double max)
{
	t_vec3 result;
	result.e[0] = random_double_range(min, max);
	result.e[1] = random_double_range(min, max);
	result.e[2] = random_double_range(min, max);
	return result;
}

t_vec3 random_in_unit_sphere()
{
	while (1) {
	t_vec3 p = vec3_random_range(-1, 1);
	if (vec3_length_squared(&p) >= 1)
	continue ;
	return p;
	}
}

t_vec3 random_unit_vector()
{
	return vec3_unit_vector(random_in_unit_sphere());
}

double clamp(double x, double min, double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

int near_zero(const t_vec3* v)
{
	// Return 1 if the vector is close to zero in all dimensions.
	const double s = 1e-8;
	return (fabs(vec3_x(v)) < s) && (fabs(vec3_y(v)) < s) && (fabs(vec3_z(v)) < s);
}

