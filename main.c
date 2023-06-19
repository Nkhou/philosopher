/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:33:28 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/06/19 22:51:19 by nkhoudro         ###   ########.fr       */
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
	while (philo->num_time_was_eat <= philo->data.num_time_to_eat && philo->data.stop)
	{
		pthread_mutex_lock(&philo->data.write);
		pthread_mutex_lock(philo->left_fork);
		if (philo->data.stop)
			take_fork(philo);
		pthread_mutex_lock(philo->right_fork);
		if (philo->data.stop)
		{
			take_fork(philo);
			eating(philo);
		}
		if (philo->data.num_time_to_eat && philo->data.stop)
		{
			philo->num_time_was_eat = philo->num_time_was_eat + 1;
		}
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		if (philo->data.stop)
			sleeping(philo);
		pthread_mutex_unlock(&philo->data.write);
	}
	return (0);
}
void	*routune(void *tred)
{
	t_philosopher	*philo;

	philo = (t_philosopher *) tred;
	while (philo->num_time_was_eat <= philo->data.num_time_to_eat && philo->data.stop)
	{
		pthread_mutex_lock(&philo->data.write);
		if (philo->data.stop)
			sleeping(philo);
		pthread_mutex_unlock(&philo->data.write);
		pthread_mutex_lock(philo->left_fork);
		if (philo->data.stop)
			take_fork(philo);
		pthread_mutex_lock(philo->right_fork);
		if (philo->data.stop)
		{
			take_fork(philo);
			eating(philo);
		}
		if (philo->data.num_time_to_eat && philo->data.stop)
		{
			philo->num_time_was_eat = philo->num_time_was_eat + 1;
		}
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&philo->data.write);
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

void	ft_initial(char **argv, int ac, t_philosopher *philo)
{
	if (ac == 6)
		philo->data.num_time_to_eat = ft_atoi(argv[ac - 1]) - 1;
	else
		philo->data.num_time_to_eat = 0;
	philo->num_time_was_eat = 0;
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
		philo[i].time_to_eat_meal = 0;
		philo[i].start = get_time();
		philo[i].left_fork = &philo[i].data.forks[i];
		philo[i].right_fork = &philo[i].data.forks[(i + 1) % philo[i].data.num_philo];
		philo[i].id = i + 1;
		philo[i].data.stop = 1;
		i++;
	}
}

void	*check_philo(void *tread)
{
	int	i;
	t_philosopher	*philo;
	long long	tmp;

	i = -1;
	philo = (t_philosopher *) tread;
	while (1)
	{
		tmp = get_time() - philo[++i].start;
		 if (tmp - philo[i].time_to_eat_meal > philo->time_to_die)
		{
			pthread_mutex_lock(&philo->data.lock);
			printf("%lld ms %d is die \n", tmp, philo->id);
			i = 0;
			while(i < philo->data.num_philo)
			{
				philo[i].data.stop = 0;
				i++;
			}
			pthread_mutex_unlock(&philo->data.lock);
			return(NULL);
		}
		if (i >= philo->data.num_philo)
			i = -1;
	}
	return (0);
}
void philos(t_philosopher *philo)
{
	int i;
	pthread_t	check;
	

	i = 0;
	insial_fork(philo);
	while (philo && i < philo->data.num_philo)
	{
		
		if (philo[i].id % 2 != 0)
		{
			if (pthread_create(&philo[i].tread, NULL, &routune_philo, &philo[i]) != 0)
				ft_error("Error\n");
			pthread_detach(philo[i].tread);
		}
		else
		{
			if (pthread_create(&philo[i].tread, NULL, &routune, &philo[i]) != 0)
				ft_error("Error\n");
			pthread_detach(philo[i].tread);
		}
		
		i++;
	}
	if (pthread_create(&check, NULL, &check_philo, philo) != 0)
		ft_error("Error\n");
	i = 0;
	
			if (pthread_join(check, NULL) != 0)
				ft_error("Error\n");
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
	pthread_mutex_init(&(philo->data.lock), NULL);
	philos(philo);
	i = 0;
	while (i < philo->data.num_fork)
		pthread_mutex_destroy(&philo->data.forks[i++]);
	pthread_mutex_destroy(&philo->data.lock);
	return (0);
}
