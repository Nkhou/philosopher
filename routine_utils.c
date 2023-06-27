/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 10:57:10 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/06/27 11:22:32 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int ft_initial_mutex(t_data *data)
{
	int i;

	i = 0;
	while (i < data->num_fork)
	{
		if (!pthread_mutex_init(&(data->forks[i++]), NULL))
			return (0);
	}
		if (!pthread_mutex_init(&(data->write), NULL))
			return (0);
		if (!pthread_mutex_init(&(data->lock), NULL))
			return (0);
	return (1);
}

int	eating(t_philosopher	*philo)
{
	unsigned long	tmp;
	t_data *data;
	
	data = philo->data;
	tmp = get_time() - data->start;
	philo->time_to_eat_meal =  get_time();
	pthread_mutex_lock(&data->write);
	printf("%lu %d is eating \n", tmp, philo->id);
	pthread_mutex_unlock(&data->write);
	ft_usleep(data->time_to_eat);
	return (0);
}

void	take_fork(t_philosopher	*philo)
{
	unsigned long	tmp;

	tmp = get_time() - philo->data->start;
	pthread_mutex_lock(&philo->data->write);
	printf("%lu %d has taken a fork \n", tmp, philo->id);
	pthread_mutex_unlock(&philo->data->write);
}

void	sleeping(t_philosopher	*philo)
{
	unsigned long	tmp;

	tmp = get_time() - philo->data->start;
	pthread_mutex_lock(&philo->data->write);
	printf("%lu %d is sleeping \n", tmp, philo->id);
	pthread_mutex_unlock(&philo->data->write);
	ft_usleep(philo->data->time_to_sleep);
	pthread_mutex_lock(&philo->data->write);
	printf("%lu %d is thinking \n", get_time() - philo->data->start, philo->id);
	pthread_mutex_unlock(&philo->data->write);
}

int check_condition(t_data *data, int i)
{
	unsigned long	tmp;

	tmp = get_time();
	pthread_mutex_lock(&data->lock);
	if ((get_time() - data->philo[i].time_to_eat_meal) > (unsigned long)data->time_to_die)
	{
		pthread_mutex_lock(&data->write);
		printf("%lu %d is die \n", tmp - data->start, data->philo[i].id);
		data->stop = 0;
		pthread_mutex_unlock(&data->lock);
		i = 0;
		while (i < data->num_philo)
		{
			data->philo[i].data->stop = data->stop;
			i++;
		}
		return(0);
	}
	return (1);
}