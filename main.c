/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:33:28 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/06/06 11:44:16 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
void	eating(t_philosopher	*philo)
{
		printf("philo %d is eating \n", philo->id - 1);
		usleep(philo->data.time_to_eat * 1000);
}
void	take_fork(t_philosopher	*philo)
{
		printf("philo %d has taken a fork \n", philo->id - 1);
		printf("philo %d has taken a fork \n", philo->id - 1 );
}
void	sleeping(t_philosopher	*philo)
{
	printf("philo %d is sleeping \n", philo->id - 1);
	usleep(philo->data.time_to_sleep * 1000);
}
void	*routune_philo(void *tred)
{
	t_philosopher	*philo;

	philo = (t_philosopher *) tred;
	// while (1)
	// {
		pthread_mutex_lock(&philo->data.write);
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->right_fork);
		take_fork(philo);
		eating(philo);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
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
	// philo->left_fork = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
	// philo->right_fork = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
}

void	insial_fork(t_philosopher *philo)
{
	int i;

	i = 0;
	while (i < philo->data.num_fork)
	{
		philo[i].left_fork = &philo[i].data.forks[i];
		philo[i].right_fork = &philo[i].data.forks[(i + 1) % philo[i].data.num_philo];
		philo[i].id = i + 1;
		i++;
	}
}
void philos(t_philosopher *philo)
{
	int i;

	i = 0;
	insial_fork(philo);
	while (philo && i < philo[i].data.num_philo)
	{
		if (philo[i].id % 2 != 0)
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
