/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fail_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azebahad <azebahad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:02:13 by azebahad          #+#    #+#             */
/*   Updated: 2026/08/08 19:14:47 by azebahad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codixion.h"

int	fail_mutex(t_dongle *dongles, int i)
{
	cleanup_init_dongle(dongles, i);
	return (0);
}

int	fail_cond(t_dongle *dongles, int i)
{
	cleanup_init_dongle(dongles, i);
	return (0);
}
