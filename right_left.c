/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   right_left.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 11:14:55 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/06/27 11:15:04 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int left_fork(t_philosopher *philo)
{
	t_data *data;

	data = philo->data;
	pthread_mutex_lock(philo->left_fork);
	if (data->stop)
		take_fork(philo);
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	return (1);
}
int right_fork(t_philosopher *philo)
{
	t_data *data;

	data = philo->data;
	pthread_mutex_lock(philo->right_fork);
	if (data->stop)
	{
		take_fork(philo);
		eating(philo);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	return (1);
}