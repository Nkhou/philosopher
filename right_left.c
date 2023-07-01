/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   right_left.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 11:14:55 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/07/01 13:58:14 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int left_fork(t_philosopher *philo)
{
	t_data *data;

	data = philo->data;
	pthread_mutex_lock(philo->left_fork);
	take_fork(philo);
    // pthread_mutex_lock(&data->lock);
	// if (data->stop)
	// else
	// {
		// pthread_mutex_unlock(&data->lock);
		// pthread_mutex_unlock(philo->left_fork);
		// return (0);
	// }
    // pthread_mutex_unlock(&data->lock);
	return (1);
}
int right_fork(t_philosopher *philo)
{
	t_data *data;

	data = philo->data;
	pthread_mutex_lock(philo->right_fork);
	take_fork(philo);
	eating(philo);
    // pthread_mutex_lock(&data->lock);
	// if (data->stop)
	// {
	// }
	// else
	// {
    //     pthread_mutex_unlock(&data->lock);
	// 	pthread_mutex_unlock(philo->left_fork);
	// 	pthread_mutex_unlock(philo->right_fork);
	// 	return (0);
	// }
    // pthread_mutex_unlock(&data->lock);
	return (1);
}