/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:33:28 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/06/20 22:32:55 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

long long	get_time()
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	eating(t_data	*data)
{
	long long	tmp;
	
	tmp = get_time() - data->start;
	ft_usleep(data->time_to_eat * 1000);
	printf("%lld ms %d is eating \n", tmp, data->philo[data->index].id);
	data->philo[data->index].time_to_eat_meal =  get_time() - data->start;
	return (0);
}

void	take_fork(t_data	*data)
{
	long long	tmp;

	tmp = get_time() - data->start;
	printf("%lld ms %d  has taken a fork \n", tmp, data->philo[data->index].id);
}

void	sleeping(t_data	*data)
{
	long long	tmp;

	tmp = get_time() - data->start;
	ft_usleep(data->time_to_sleep * 1000);
	printf("%lld ms %d is sleeping \n", tmp,data->philo[data->index].id);
}
void	*routune_philo(void *tred)
{
	t_data			*data;

	data = (t_data *)tred;
	while (data->philo[data->index].num_time_was_eat <= data->num_time_to_eat && data->stop)
	{
		printf("index = %d\n", data->index);
		// pthread_mutex_lock(&data->write);
		pthread_mutex_lock(data->philo[data->index].left_fork);
		if (data->stop)
			take_fork(data);
		pthread_mutex_lock(data->philo[data->index].right_fork);
		if (data->stop)
		{
			take_fork(data);
			eating(data);
		}
		if (data->num_time_to_eat && data->stop)
		{
			data->philo[data->index].num_time_was_eat = data->philo[data->index].num_time_was_eat + 1;
		}
		pthread_mutex_unlock(data->philo[data->index].right_fork);
		pthread_mutex_unlock(data->philo[data->index].left_fork);
		if (data->stop)
			sleeping(data);
		// pthread_mutex_unlock(&data->write);
	}
	return (0);
}
void	*routune(void *tred)
{
	t_data			*data;

	data = (t_data *)tred;
	while (data->philo[data->index].num_time_was_eat <= data->num_time_to_eat && data->stop)
	{
		printf("index = %d\n", data->index);
		// pthread_mutex_lock(&data->write);
		if (data->stop)
			sleeping(data);
		// pthread_mutex_unlock(&data->write);
		pthread_mutex_lock(data->philo[data->index].left_fork);
		if (data->stop)
			take_fork(data);
		pthread_mutex_lock(data->philo[data->index].right_fork);
		if (data->stop)
		{
			take_fork(data);
			eating(data);
		}
		if (data->num_time_to_eat && data->stop)
		{
			data->philo[data->index].num_time_was_eat = data->philo[data->index].num_time_was_eat + 1;
		}
		pthread_mutex_unlock(data->philo[data->index].right_fork);
		pthread_mutex_unlock(&data->write);
	}
	return (0);
}

void	check_arguments(char **argv)
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
				ft_error("Error in number of arguments\n");
			j++;
		}
		i++;
	}
}

void	ft_initial(char **argv, int ac, t_data *data)
{
	if (ac == 6)
		data->num_time_to_eat = ft_atoi(argv[ac - 1]) - 1;
	else
		data->num_time_to_eat = 0;
	data->num_philo = ft_atoi(argv[1]);
	data->num_fork = ft_atoi(argv[1]);
	data->forks = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * data->num_fork);
	data->philo = (t_philosopher *) malloc(sizeof(t_philosopher) * data->num_fork);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
}

void	insial_fork(t_data *data)
{
	int i;

	i = 0;
	while (i < data->num_fork)
	{
		data->philo[i].num_time_was_eat = 0;
		data->philo[i].time_to_eat_meal = 0;
		data->start = get_time();
		data->philo[i].left_fork = &data->forks[i];
		data->philo[i].right_fork = &data->forks[(i + 1) % data->num_philo];
		data->philo[i].id = i + 1;
		data->stop = 1;
		i++;
	}
}

void	*check_philo(void *tread)
{
	int	i;
	t_data	*data;
	long long	tmp;

	i = -1;
	data = (t_data *) tread;
	while (1)
	{
		tmp = get_time() - data->start;
		if (tmp - data->philo[i].time_to_eat_meal > data->time_to_die)
		{
			pthread_mutex_lock(&data->lock);
			printf("ha ana %lld  %d %lld\n",tmp - data->philo[i].time_to_eat_meal, data->time_to_die, tmp);
			printf("%d ms %d is die \n", data->philo[i].time_to_eat_meal, data->philo[data->index].id);
			data->stop = 0;
			pthread_mutex_unlock(&data->lock);
			return(NULL);
		}
		if (i >= data->num_philo)
			i = -1;
	}
	return (0);
}
void philos(t_data *data)
{
	int i;
	pthread_t	check;

	i = 0;
	insial_fork(data);
	while (data && i < data->num_philo)
	{
		data->index = i;
		if (data->philo[i].id % 2 != 0)
		{
			if (pthread_create(&data->philo[i].tread, NULL, &routune_philo, data) != 0)
				ft_error("Error\n");
			pthread_detach(data->philo[i].tread);
		}
		else
		{
			if (pthread_create(&data->philo[i].tread, NULL, &routune, data) != 0)
				ft_error("Error\n");
			pthread_detach(data->philo[i].tread);
		}
		
		i++;
	}
	if (pthread_create(&check, NULL, &check_philo, data) != 0)
		ft_error("Error\n");
	i = 0;
	
			if (pthread_join(check, NULL) != 0)
				ft_error("Error\n");
}

int	main(int ac, char **argv)
{
	t_data data;
	int i;

	if (ac != 5 && ac != 6)
		ft_error("Error in number of arguments\n");
	check_arguments(argv);
	i = 0;
	ft_initial(argv, ac, &data);
	while (i < data.num_fork)
		pthread_mutex_init(&(data.forks[i++]), NULL);
	pthread_mutex_init(&(data.write), NULL);
	pthread_mutex_init(&(data.lock), NULL);
	philos(&data);
	i = 0;
	while (i < data.num_fork)
		pthread_mutex_destroy(&data.forks[i++]);
	pthread_mutex_destroy(&data.lock);
	return (0);
}
