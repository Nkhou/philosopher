/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   right_left.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 11:14:55 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/07/01 14:41:41 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	left_fork(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(philo->left_fork);
	take_fork(philo);
	return (1);
}

int	right_fork(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(philo->right_fork);
	take_fork(philo);
	eating(philo);
	return (1);
}
