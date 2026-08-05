/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_request_from_sch.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azebahad <azebahad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 18:35:46 by azebahad          #+#    #+#             */
/*   Updated: 2026/08/05 18:47:33 by azebahad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codixion.h"

static void	fix_heap_after_removal(t_heap *heap, t_coder *coder, int index)
{
	int	left_child;
	int	right_child;
	int	best_child;

	while (1)
	{
		left_child = (index * 2) + 1;
		right_child = left_child + 1;
		best_child = index;
		if (left_child < heap->size
			&& request_has_higher_priority(&heap->data[left_child],
				&heap->data[best_child], coder->config->scheduler))
			best_child = left_child;
		if (right_child < heap->size
			&& request_has_higher_priority(&heap->data[right_child],
				&heap->data[best_child], coder->config->scheduler))
			best_child = right_child;
		if (best_child == index)
			return ;
		heap_swap(&heap->data[index], &heap->data[best_child]);
		index = best_child;
	}
}

static int	replace_the_removal(t_heap *heap, t_dongle *dongle, t_coder *coder,
		int index)
{
	int	parent;

	heap->data[index] = heap->data[heap->size - 1];
	heap->size--;
	if (heap->size == 0)
	{
		pthread_cond_broadcast(&dongle->scheduler.cond);
		pthread_mutex_unlock(&dongle->mutex);
		return (-1);
	}
	if (index > 0)
	{
		parent = (index - 1) / 2;
		while (index > 0 && request_has_higher_priority(&heap->data[index],
				&heap->data[parent], coder->config->scheduler))
		{
			heap_swap(&heap->data[index], &heap->data[parent]);
			index = parent;
			parent = (index - 1) / 2;
		}
	}
	fix_heap_after_removal(heap, coder, index);
	return (0);
}

void	remove_request(t_coder *coder, t_dongle *dongle)
{
	t_heap	*heap;
	int		index;

	pthread_mutex_lock(&dongle->mutex);
	heap = &dongle->scheduler.pending;
	index = 0;
	while (index < heap->size)
	{
		if (heap->data[index].coder_id == coder->id)
			break ;
		index++;
	}
	if (index >= heap->size)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	if (replace_the_removal(heap, dongle, coder, index) == -1)
		return ;
	pthread_cond_broadcast(&dongle->scheduler.cond);
	pthread_mutex_unlock(&dongle->mutex);
}
