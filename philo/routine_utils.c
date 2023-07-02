/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 10:57:10 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/07/02 14:57:45 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	ft_initial_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_fork)
	{
		if (pthread_mutex_init(&(data->forks[i++]), NULL))
			return (0);
	}
	if (pthread_mutex_init(&(data->write), NULL))
		return (0);
	if (pthread_mutex_init(&(data->meal), NULL))
		return (0);
	if (pthread_mutex_init(&(data->nb_eat), NULL))
		return (0);
	return (1);
}

int	eating(t_philosopher	*philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->meal);
	philo->time_to_eat_meal = get_time();
	pthread_mutex_unlock(&data->meal);
	pthread_mutex_lock(&data->write);
	printf("%lu %d is eating \n", get_time() - philo->data->start, philo->id);
	pthread_mutex_unlock(&data->write);
	ft_usleep(data->time_to_eat);
	return (0);
}

void	take_fork(t_philosopher	*philo)
{
	pthread_mutex_lock(&philo->data->write);
	printf("%lu %d  has taken a fork \n",
		get_time() - philo->data->start, philo->id);
	pthread_mutex_unlock(&philo->data->write);
}

void	sleeping(t_philosopher	*philo)
{
	pthread_mutex_lock(&philo->data->write);
	printf("%lu %d is sleeping \n", get_time() - philo->data->start, philo->id);
	pthread_mutex_unlock(&philo->data->write);
	ft_usleep(philo->data->time_to_sleep);
	pthread_mutex_lock(&philo->data->write);
	printf("%lu %d is thinking \n", get_time() - philo->data->start, philo->id);
	pthread_mutex_unlock(&philo->data->write);
}

int	check_condition(t_data *data, int i)
{
	pthread_mutex_lock(&data->meal);
	if ((get_time() - data->philo[i].time_to_eat_meal)
		> (unsigned long)data->time_to_die)
	{
		pthread_mutex_unlock(&data->meal);
		pthread_mutex_lock(&data->write);
		printf("%lu %d died \n", get_time() - data->start, data->philo[i].id);
		return (0);
	}
	pthread_mutex_unlock(&data->meal);
	return (1);
}
