/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 10:57:10 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/06/28 20:23:00 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int ft_initial_mutex(t_data *data)
{
	int i;

	i = 0;
	while (i < data->num_fork)
	{
		if (!pthread_mutex_init(&(data->forks[i]), NULL))
			return (0);
		i++;
	}
		if (!pthread_mutex_init(&(data->eat), NULL))
			return (0);
		if (!pthread_mutex_init(&(data->write), NULL))
			return (0);
		if (!pthread_mutex_init(&(data->meal), NULL))
			return (0);
		if (!pthread_mutex_init(&(data->nb_eat), NULL))
			return (0);
		if (!pthread_mutex_init(&(data->lock), NULL))
			return (0);
		if (!pthread_mutex_init(&(data->start_m), NULL))
			return (0);
	return (1);
}
int ft_print(t_philosopher *philo, int i)
{
	int j;

	j = 0;
	
	pthread_mutex_lock(&philo->data->meal);
	pthread_mutex_lock(&philo->data->lock);
	if ((philo->num_time_was_eat < philo->data->num_time_to_eat ||  philo->data->num_time_to_eat == -1) && philo->data->stop)
	{
		pthread_mutex_unlock(&philo->data->lock);
		pthread_mutex_unlock(&philo->data->meal);
		if (i == 0)
		{
			pthread_mutex_lock(&philo->data->eat);
			pthread_mutex_lock(&philo->data->write);
			printf("%lu %d is eating \n", get_time() - philo->data->start, philo->id);
			pthread_mutex_unlock(&philo->data->write);
			pthread_mutex_unlock(&philo->data->eat);
		}
		else if (i == 1)
		{
			pthread_mutex_lock(&philo->data->start_m);
			pthread_mutex_lock(&philo->data->write);
			printf("%lu %d has taken a fork \n", get_time() - philo->data->start, philo->id);
			pthread_mutex_unlock(&philo->data->write);
			pthread_mutex_unlock(&philo->data->start_m);
		}
		else if (i == 2)
		{
			pthread_mutex_lock(&philo->data->start_m);
			pthread_mutex_lock(&philo->data->write);
			printf("%lu %d is sleeping \n", get_time() - philo->data->start, philo->id);
			pthread_mutex_unlock(&philo->data->write);
			pthread_mutex_unlock(&philo->data->start_m);
		}
		else if (i == 3)
		{
			pthread_mutex_lock(&philo->data->start_m);
			pthread_mutex_lock(&philo->data->write);
			printf("%lu %d is thinking \n", get_time() - philo->data->start, philo->id);
			pthread_mutex_unlock(&philo->data->write);
			pthread_mutex_unlock(&philo->data->start_m);
		}
		else
		{
			pthread_mutex_lock(&philo->data->start_m);
			pthread_mutex_lock(&philo->data->write);
			printf("ha ana\n");
			printf("%lu %d is die \n", get_time() - philo->data->start, philo->data->philo[i].id);
			pthread_mutex_unlock(&philo->data->start_m);
		}
		return (1);
	}
	pthread_mutex_unlock(&philo->data->lock);
	pthread_mutex_unlock(&philo->data->meal);
	return(0);
}
int	eating(t_philosopher	*philo)
{
	t_data *data;
	
	data = philo->data;
	pthread_mutex_lock(&data->meal);
	philo->time_to_eat_meal =  get_time();
	pthread_mutex_unlock(&data->meal);
	pthread_mutex_lock(&data->start_m);
	if (!ft_print(philo, 0))
		return (0);
	pthread_mutex_unlock(&data->start_m);
	ft_usleep(data->time_to_eat);
	return (1);
}

int	take_fork(t_philosopher	*philo)
{
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_unlock(&philo->data->lock);
	if (!ft_print(philo, 1))
		return (0);
	return (1);
}

int	sleeping(t_philosopher	*philo)
{
	pthread_mutex_lock(&philo->data->start_m);
	if (!ft_print(philo, 2))
	{
		pthread_mutex_unlock(&philo->data->start_m);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->start_m);
	ft_usleep(philo->data->time_to_sleep);
	pthread_mutex_lock(&philo->data->start_m);
	if (!ft_print(philo, 3))
	{
		pthread_mutex_unlock(&philo->data->start_m);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->start_m);
	return (1);
}

int check_condition(t_data *data, int i)
{
	pthread_mutex_lock(&data->meal);
	if ((get_time() - data->philo[i].time_to_eat_meal) > (unsigned long)data->time_to_die)
	{
		pthread_mutex_unlock(&data->meal);
		// pthread_mutex_lock(&data->start_m);
		if (!ft_print(data->philo, 4))
		{
			// pthread_mutex_unlock(&data->start_m);
			pthread_mutex_lock(&data->lock);
			data->stop = 0;
			pthread_mutex_unlock(&data->lock);
			return (0);
		}
		pthread_mutex_unlock(&data->start_m);
		pthread_mutex_lock(&data->lock);
		data->stop = 0;
		pthread_mutex_unlock(&data->lock);
		return(0);
	}
	pthread_mutex_unlock(&data->meal);
	return (1);
}