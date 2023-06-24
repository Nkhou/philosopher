/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:33:28 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/06/24 14:22:38 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

long long	get_time()
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	eating(t_philosopher	*philo)
{
	long long	tmp;
	t_data *data;
	
	data = philo->data;
	tmp = get_time() - data->start;
	ft_usleep(data->time_to_eat);
	philo->time_to_eat_meal =  get_time() - data->start;
	printf("**********%d\n",philo->time_to_eat_meal);
	pthread_mutex_lock(&data->write);
	printf("%lld ms %d is eating \n", tmp, philo->id);
	pthread_mutex_unlock(&data->write);
	return (0);
}

void	take_fork(t_philosopher	*philo)
{
	long long	tmp;

	tmp = get_time() - philo->data->start;
	pthread_mutex_lock(&philo->data->write);
	printf("%lld ms %d  has taken a fork \n", tmp, philo->id);
	pthread_mutex_unlock(&philo->data->write);
}

void	sleeping(t_philosopher	*philo)
{
	long long	tmp;

	tmp = get_time() - philo->data->start;
	ft_usleep(philo->data->time_to_sleep);
	pthread_mutex_lock(&philo->data->write);
	printf("%lld ms %d is sleeping \n", tmp, philo->id);
	pthread_mutex_unlock(&philo->data->write);
}
void	*routune_philo(void *tred)
{
	t_philosopher			*philo;
	t_data *data;

	philo = (t_philosopher *)tred;
	data = philo->data;
	while (philo->num_time_was_eat <= data->num_time_to_eat && data->stop)
	{
		pthread_mutex_lock(philo->left_fork);
		if (data->stop)
			take_fork(philo);
		pthread_mutex_lock(philo->right_fork);
		if (data->stop)
		{
			take_fork(philo);
			eating(philo);
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
void	*routune(void *tred)
{
	t_philosopher			*philo;
	t_data *data;

	philo = (t_philosopher *)tred;
	data = philo->data;
	while (philo->num_time_was_eat <= data->num_time_to_eat && data->stop)
	{
		if (data->stop)
			sleeping(philo);
		pthread_mutex_lock(philo->left_fork);
		if (data->stop)
			take_fork(philo);
		pthread_mutex_lock(philo->right_fork);
		if (data->stop)
		{
			take_fork(philo);
			eating(philo);
		}
		if (data->num_time_to_eat && data->stop)
		{
			philo->num_time_was_eat = philo->num_time_was_eat + 1;
		}
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
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
	data->philo = (t_philosopher *) malloc(sizeof(t_philosopher) * data->num_philo);
	if (!data->philo)
		ft_error("Error\n");
	data->forks = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * data->num_fork);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
}

void	insial_fork(t_data *data)
{
	int i;

	i = 0;
	data->start = get_time();
	while (i < data->num_fork)
	{
		data->philo[i].data = data;
		data->philo[i].num_time_was_eat = 0;
		data->philo[i].time_to_eat_meal = 0;
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
	t_check	*check;
	t_data	*data;
	long long	tmp;

	i = -1;
	check = (t_check *) tread;
	data = check->data;
	while (1)
	{
		tmp = get_time() - data->start;
		if ((tmp - data->philo[i].time_to_eat_meal) > data->time_to_die)
		{
			pthread_mutex_lock(&data->lock);
			printf("%lld ms %d is die \n", tmp, data->philo[data->index].id);
			data->stop = 0;
			pthread_mutex_unlock(&data->lock);
			i = 0;
			while (i < data->num_philo)
			{
				data->philo[i].data->stop = data->stop;
				i++;
			}
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
	t_check	check;

	i = 0;
	insial_fork(data);
	while (i < data->num_philo)
	{
		if (data->philo[i].id % 2 == 0 && get_time() == data->start)
			usleep(100);
		if (pthread_create(&data->philo[i].tread, NULL, &routune_philo, &data->philo[i]) != 0)
			ft_error("Error\n");
		pthread_detach(data->philo[i].tread);
		i++;
	}
	check.data = data;
	if (pthread_create(&check.tread, NULL, &check_philo, &check) != 0)
		ft_error("Error\n");
	if (pthread_join(check.tread, NULL) != 0)
			ft_error("Error\n");
}

int	main(int ac, char **argv)
{
	t_data *data;
	int i;

	if (ac != 5 && ac != 6)
		ft_error("Error in number of arguments\n");
	check_arguments(argv);
	data = (t_data *)malloc(sizeof(t_data) );
	if (!data)
		return (0);

	memset(data,0,sizeof(t_data ));
	ft_initial(argv, ac, data);
	i = 0;
	while (i < data->num_fork)
		pthread_mutex_init(&(data->forks[i++]), NULL);
	pthread_mutex_init(&(data->write), NULL);
	pthread_mutex_init(&(data->lock), NULL);
	philos(data);
	i = 0;
	while (i < data->num_fork)
		pthread_mutex_destroy(&data->forks[i++]);
	pthread_mutex_destroy(&data->lock);
	return (0);
}
