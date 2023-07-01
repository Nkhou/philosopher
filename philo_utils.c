/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 10:54:50 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/07/01 14:39:11 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	check_arguments(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				ft_error("Error\n");
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	check_intial(t_data *data)
{
	if (data->num_time_to_eat == 0)
		return (0);
	if (!data->num_philo)
		return (0);
	if (!data->num_fork)
		return (0);
	if (!data->philo)
		return (0);
	if (!data->forks)
		return (0);
	if (!data->time_to_die)
		return (0);
	if (!data->time_to_eat)
		return (0);
	if (!data->time_to_sleep)
		return (0);
	return (1);
}

int	ft_initial(char **argv, int ac, t_data *data)
{
	if (ac == 6)
		data->num_time_to_eat = ft_atoi(argv[ac - 1]);
	else
		data->num_time_to_eat = -1;
	data->num_philo = ft_atoi(argv[1]);
	data->num_fork = ft_atoi(argv[1]);
	data->philo = (t_philosopher *)
		malloc(sizeof(t_philosopher) * data->num_philo);
	if (!data->philo)
		return (0);
	data->forks = (pthread_mutex_t *)
		malloc(sizeof(pthread_mutex_t) * data->num_fork);
	if (!data->forks)
		return (0);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (!check_intial(data))
		return (0);
	return (1);
}

void	insial_fork(t_data *data)
{
	int	i;

	i = 0;
	data->start = get_time();
	while (i < data->num_fork)
	{
		data->philo[i].data = data;
		data->philo[i].num_time_was_eat = 0;
		data->philo[i].time_to_eat_meal = get_time();
		data->philo[i].left_fork = &data->forks[i];
		data->philo[i].right_fork = &data->forks[(i + 1) % data->num_philo];
		data->philo[i].id = i + 1;
		i++;
	}
}

int	ft_clear(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_fork)
		pthread_mutex_destroy(&data->forks[i++]);
	pthread_mutex_destroy(&data->meal);
	pthread_mutex_destroy(&data->nb_eat);
	pthread_mutex_destroy(&data->write);
	while (i < data->num_fork)
	{
		free(&data->forks[i]);
		i++;
	}
	free(data->forks);
	free(data->philo);
	free(data);
	return (0);
}
