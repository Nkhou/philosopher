/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:33:28 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/06/28 12:33:33 by nkhoudro         ###   ########.fr       */
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
		// pthread_mutex_lock(&data->start_m);
		if ((get_time() - data->start == 0 || get_time() - data->start == 1) && philo->id % 2 == 0)
			usleep(100);
		// pthread_mutex_unlock(&data->start_m);
		// pthread_mutex_lock(&data->lock);
		// pthread_mutex_lock(&data->nb_eat);
		if (!(philo->num_time_was_eat < data->num_time_to_eat || ( data->num_time_to_eat == -1) || data->stop))
		{
			// pthread_mutex_unlock(&data->nb_eat);
			// pthread_mutex_unlock(&data->lock);
			return (0);
		}
			// pthread_mutex_unlock(&data->nb_eat);
			pthread_mutex_unlock(&data->lock);
		if (!left_fork(philo))
			return (0);
		if (!right_fork(philo))
			return (0);
		pthread_mutex_lock(&data->lock);
		pthread_mutex_lock(&data->nb_eat);
		if (data->num_time_to_eat && data->stop)
		{
			philo->num_time_was_eat = philo->num_time_was_eat + 1;
		}
		pthread_mutex_unlock(&data->nb_eat);
		pthread_mutex_unlock(&data->lock);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_lock(&data->lock);
		if (data->stop)
			sleeping(philo);
		pthread_mutex_unlock(&data->lock);
	}
	return (0);
}

int	check_philo(void *tread)
{
	int	i;
	t_data	*data;

	i = 0;
	data = (t_data *) tread;
	while (1)
	{
		// pthread_mutex_lock(&data->lock);
		pthread_mutex_lock(&data->nb_eat);
		if (!(data->philo->num_time_was_eat < data->num_time_to_eat || ( data->num_time_to_eat == -1)))
		{
			pthread_mutex_unlock(&data->nb_eat);
			// pthread_mutex_unlock(&data->lock);
			return (0);
		}
		pthread_mutex_unlock(&data->nb_eat);
		if (!check_condition(data, i))
			return (0);
		// pthread_mutex_unlock(&data->lock);
		i++;
		i = i % data->num_philo;
		usleep(200);
	}
	return (1);
}


int philos(t_data *data)
{
	int i;

	i = 0;
	insial_fork(data);
	while (i < data->num_philo)
	{
		if (pthread_create(&data->philo[i].tread, NULL, &routune_philo, &data->philo[i]) != 0)
			return (0);
		pthread_detach(data->philo[i].tread);
		i++;
	}
	if (!check_philo(data))
		return (0);
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
	if (!ft_initial(argv, ac, data))
	{
		free(data->forks);
		free(data->philo);
		free(data);
		return (0);
	}
	ft_initial_mutex(data);
	if (!philos(data))
	{
		if (!ft_clear(data))
			return (0);
	}
	return (0);
}