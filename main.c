/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:33:28 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/06/09 18:51:38 by nkhoudro         ###   ########.fr       */
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

	tmp = get_time() - philo->start;
	printf("%lld ms %d is eating \n", tmp, philo->id);
	ft_usleep(philo->data.time_to_eat);
	philo[philo->id].time_to_eat_meal =  get_time() - philo->start;
	return (0);
}

void	take_fork(t_philosopher	*philo)
{
	long long	tmp;

	tmp = get_time() - philo->start;
	printf("%lld ms %d  has taken a fork \n", tmp, philo->id);
}

void	sleeping(t_philosopher	*philo)
{
	long long	tmp;

	tmp = get_time() - philo->start;
	printf("%lld ms %d is sleeping \n", tmp, philo->id);
	ft_usleep(philo->data.time_to_sleep);
}
void	*routune_philo(void *tred)
{
	t_philosopher	*philo;

	philo = (t_philosopher *) tred;
	// while (1)
	// {
		pthread_mutex_lock(&philo->data.write);
		pthread_mutex_lock(philo->left_fork);
		take_fork(philo);
		pthread_mutex_lock(philo->right_fork);
		take_fork(philo);
		eating(philo);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		// pthread_mutex_unlock(&philo->data.write);
		// pthread_mutex_lock(&philo->data.write);
		sleeping(philo);
		pthread_mutex_unlock(&philo->data.write);
	// }
		// exit(1);
	return (0);
}
void	*routune(void *tred)
{
	t_philosopher	*philo;

	philo = (t_philosopher *) tred;
	// while (1)
	// {
		pthread_mutex_lock(&philo->data.write);
		sleeping(philo);
		pthread_mutex_unlock(&philo->data.write);
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->data.write);
		take_fork(philo);
		eating(philo);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&philo->data.write);
	// }
		// exit(1);
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

void	ft_initial(char **argv, int ac, t_philosopher *philo)
{
	if (ac == 6)
		philo->data.num_time_to_eat = ft_atoi(argv[ac - 1]);
	philo->data.num_philo = ft_atoi(argv[1]);
	philo->data.num_fork = ft_atoi(argv[1]);
	philo->data.forks = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * philo->data.num_fork);;
	philo->data.time_to_die = ft_atoi(argv[2]);
	philo->data.time_to_eat = ft_atoi(argv[3]);
	philo->data.time_to_sleep = ft_atoi(argv[4]);
}

void	insial_fork(t_philosopher *philo)
{
	int i;

	i = 0;
	while (i < philo->data.num_fork)
	{
		philo[i].start = get_time();
		philo[i].left_fork = &philo[i].data.forks[i];
		philo[i].right_fork = &philo[i].data.forks[(i + 1) % philo[i].data.num_philo];
		philo[i].id = i;
		i++;
	}
}
// void	*check_philo(void *philo)
// {
	
// }
void philos(t_philosopher *philo)
{
	int i;

	i = 0;
	insial_fork(philo);
	while (philo && i < philo[i].data.num_philo)
	{
		if (philo[i].id % 2 == 0)
		{
			if (pthread_create(&philo[i].tread, NULL, &routune_philo, &philo[i]) != 0)
				ft_error("Error\n");
		}
		else
		{
			if (pthread_create(&philo[i].tread, NULL, &routune, &philo[i]) != 0)
				ft_error("Error\n");
		}
		i++;
	}
	// if (pthread_create(&philo[i].tread, NULL, &check_philo, &philo[i]) != 0)
	// 			ft_error("Error\n");
	i = 0;
	while (philo && i < philo[i].data.num_philo)
	{
		if (pthread_join(philo[i].tread, NULL) != 0)
			ft_error("Error\n");
		i++;
	}
}

int	main(int ac, char **argv)
{
	t_philosopher *philo;
	int i;

	if (ac != 5 && ac != 6)
		ft_error("Error in number of arguments\n");
	check_arguments(argv);
	philo = malloc(sizeof(t_philosopher) * ft_atoi(argv[1]));
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		ft_initial(argv, ac, &philo[i]);
		i++;
	}
	i = 0;
	while (i < philo->data.num_fork)
		pthread_mutex_init(&(philo->data.forks[i++]), NULL);
	pthread_mutex_init(&(philo->data.write), NULL);
	philos(philo);
	i = 0;
	while (i++ < philo->data.num_fork)
		pthread_mutex_destroy(&philo->data.forks[i]);
	return (0);
}
