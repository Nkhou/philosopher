/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:33:28 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/06/27 11:00:16 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	*routune_philo(void *tred)
{
	t_philosopher			*philo;
	t_data *data;

	philo = (t_philosopher *)tred;
	data = philo->data;
	while (1)
	{
		if (!(philo->num_time_was_eat < data->num_time_to_eat || ( data->num_time_to_eat == -1) || data->stop))
			return (0);
		pthread_mutex_lock(philo->left_fork);
		if (data->stop)
			take_fork(philo);
		else
		{
			pthread_mutex_unlock(philo->left_fork);
			return (0);
		}
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
		if (data->num_time_to_eat && data->stop)
		{
			philo->num_time_was_eat = philo->num_time_was_eat + 1;
		}
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		if (data->stop)
			sleeping(philo);
	}
	return (0);
}

void	*check_philo(void *tread)
{
	int	i;
	t_data	*data;

	i = 0;
	data = (t_data *) tread;
	while (1)
	{
		if (!(data->philo->num_time_was_eat < data->num_time_to_eat || ( data->num_time_to_eat == -1)))
			return (0);
		if (!check_condition(data, i))
			return (0);
		i++;
		i = i % data->num_philo;
	}
	return (0);
}


int philos(t_data *data)
{
	int i;

	i = 0;
	insial_fork(data);
	while (i < data->num_philo)
	{
		if (data->philo[i].id % 2 == 0)
		{
			usleep(100);
			if (pthread_create(&data->philo[i].tread, NULL, &routune_philo, &data->philo[i]) != 0)
				return (0);
		}
		else
		{
			if (pthread_create(&data->philo[i].tread, NULL, &routune_philo, &data->philo[i]) != 0)
				return (0);
		}
		pthread_detach(data->philo[i].tread);
		i++;
	}
	check_philo(data);
	return (1);
}

void tt()
{
	system("leaks philo");
}
int	main(int ac, char **argv)
{
	t_data *data;

	if (ac != 5 && ac != 6)
		ft_error("Error in number of arguments\n");
	if (!check_arguments(argv))
		return (0);
	data = (t_data *)malloc(sizeof(t_data) );
	if (!data)
		return (0);
	memset(data,0,sizeof(t_data ));
	ft_initial_mutex(data);
	if (!ft_initial(argv, ac, data))
	{
		if (!ft_clear(data))
			return (0);
		return (0);
	}
	if (!philos(data))
	{
		printf("ha ana\n");
		if (!ft_clear(data))
			return (0);
	}
	return (0);
}