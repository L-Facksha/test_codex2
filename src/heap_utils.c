/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azebahad <azebahad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:02:08 by azebahad          #+#    #+#             */
/*   Updated: 2026/08/06 00:47:56 by azebahad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codixion.h"

void	heap_swap(t_request *c1, t_request *c2)
{
	t_request	temp;

	temp = *c1;
	*c1 = *c2;
	*c2 = temp;
}

int	request_has_higher_priority(t_request *f, t_request *s, char *scheduler)
{
	if (strcmp(scheduler, "edf") == 0)
	{
		if (f->deadline != s->deadline)
			return (f->deadline < s->deadline);
	}
	if (f->timestamp != s->timestamp)
		return (f->timestamp < s->timestamp);
	return (f->coder_id < s->coder_id);
}
