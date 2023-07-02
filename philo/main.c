/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:33:28 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/07/02 14:59:20 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	ft_unloc(t_data *data, t_philosopher *philo)
{
	pthread_mutex_unlock(&data->nb_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	*routune_philo(void *tred)
{
	t_philosopher			*philo;

	philo = (t_philosopher *)tred;
	if ((get_time() - philo->data->start == 0
			|| get_time() - philo->data->start == 1) && philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		pthread_mutex_lock(&philo->data->nb_eat);
		if (!(philo->num_time_was_eat < philo->data->num_time_to_eat
				|| (philo->data->num_time_to_eat == -1)))
		{
			pthread_mutex_unlock(&philo->data->nb_eat);
			return (0);
		}
		pthread_mutex_unlock(&philo->data->nb_eat);
		left_fork(philo);
		right_fork(philo);
		pthread_mutex_lock(&philo->data->nb_eat);
		if (philo->data->num_time_to_eat)
			philo->num_time_was_eat = philo->num_time_was_eat + 1;
		ft_unloc(philo->data, philo);
		sleeping(philo);
	}
	return (0);
}

int	check_philo(void *tread)
{
	t_data	*data;
	int		i;

	i = 0;
	data = (t_data *) tread;
	while (1)
	{
		pthread_mutex_lock(&data->nb_eat);
		if (!(data->philo->num_time_was_eat < data->num_time_to_eat
				|| (data->num_time_to_eat == -1)))
		{
			pthread_mutex_unlock(&data->nb_eat);
			return (0);
		}
		pthread_mutex_unlock(&data->nb_eat);
		if (!check_condition(data, i))
			return (0);
		i++;
		i = i % data->num_philo;
	}
	return (1);
}

int	philos(t_data *data)
{
	int	i;

	i = 0;
	insial_fork(data);
	while (i < data->num_philo)
	{
		if (pthread_create(&data->philo[i].tread, NULL,
				&routune_philo, &data->philo[i]) != 0)
			return (0);
		pthread_detach(data->philo[i].tread);
		i++;
	}
	if (!check_philo(data))
		return (0);
	return (1);
}

int	main(int ac, char **argv)
{
	t_data	*data;

	if (!check_number(ac))
		return (0);
	if (!check_arguments(argv))
		return (0);
	data = (t_data *) malloc(sizeof(t_data));
	if (!data)
		return (0);
	memset(data, 0, sizeof(t_data));
	if (!ft_initial(argv, ac, data))
	{
		free(data->forks);
		free(data->philo);
		free(data);
		return (0);
	}
	if (!ft_initial_mutex(data))
		return (0);
	if (!philos(data))
	{
		if (!ft_clear(data))
			return (0);
	}
	return (0);
}
