/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_initial_mutex.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 22:28:37 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/07/01 14:35:15 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	ft_initial_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_fork)
		pthread_mutex_init(&(data->forks[i++]), NULL);
	pthread_mutex_init(&(data->write), NULL);
	pthread_mutex_init(&(data->lock), NULL);
	pthread_mutex_init(&(data->meal), NULL);
	pthread_mutex_init(&(data->nb_eat), NULL);
}
