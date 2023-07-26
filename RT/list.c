/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhihi <fhihi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:57:03 by fhihi             #+#    #+#             */
/*   Updated: 2023/07/21 15:57:31 by fhihi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"head.h"

t_hittable_list* create_hittable_list()
{
    t_hittable_list* list = (t_hittable_list*)malloc(sizeof(t_hittable_list));
    list->base.hit_func = hittable_list_hit;
    list->objects = NULL;
    list->size = 0;
    return list;
}

void hittable_list_clear(t_hittable_list* list)
{
    if (list == NULL)
        return;

    if (list->objects != NULL) {
        for (size_t i = 0; i < list->size; i++) {
            free(list->objects[i]);
        }
        free(list->objects);
        list->objects = NULL;
    }

    list->size = 0;
}

void hittable_list_add(t_hittable_list* list, t_hittable* object)
{
    if (list == NULL || object == NULL)
        return;

    list->objects = (t_hittable**)realloc(list->objects, (list->size + 1) * sizeof(t_hittable*));
    list->objects[list->size] = object;
    list->size++;
}

int hittable_list_hit(const t_hittable* obj, const t_ray* r, double t_min, double t_max, t_hit_record* rec)
{
    const t_hittable_list* list = (const t_hittable_list*)obj;

    t_hit_record temp_rec;
    int hit_anything = 0;
    double closest_so_far = t_max;
    for (size_t i = 0; i < list->size; i++) {
        const t_hittable* object = list->objects[i];
        if (object->hit_func(object, r, t_min, closest_so_far, &temp_rec))
		{
            hit_anything = 1;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }
	
    return hit_anything;
}