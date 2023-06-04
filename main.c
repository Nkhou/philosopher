/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:33:28 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/06/04 14:19:04 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	*retune(void *tred)
{
	t_philosopher	*tread;

	tread = (t_philosopher *) tred;
	while (1)
	{
		pthread_mutex_lock(tread->left_fork);
		pthread_mutex_lock(tread->right_fork);
		printf("tread %d has taken a fork \n", tread->id);
		printf("tread %d has taken a fork \n", tread->id);
		printf("tread %d is is eating \n", tread->id);
		pthread_mutex_unlock(tread->left_fork);
		pthread_mutex_unlock(tread->right_fork);
		exit(1);
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
	int	i;

	i = 1;
	if (ac == 6)
		philo->data.num_time_to_eat = ft_atoi(argv[ac - 1]);
	philo->data.num_philo = ft_atoi(argv[1]);
	philo->data.num_fork = ft_atoi(argv[1]);
	philo->data.forks = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * philo->data.num_fork);;
	philo->data.tread = (pthread_t *) malloc(sizeof(pthread_t) * philo->data.num_philo);
	philo->data.time_to_die = ft_atoi(argv[2]);
	philo->data.time_to_eat = ft_atoi(argv[3]);
	philo->data.time_to_sleep = ft_atoi(argv[4]);
	philo->data.index = 0;
}
void philos(t_philosopher *philo)
{
	int i;

	i = 0;
	while (i < philo->data.num_philo)
	{
		philo->left_fork = &philo->data.forks[i];
		philo->right_fork = &philo->data.forks[i + 1 % philo->data.num_philo];
		if (pthread_create(&philo->data.tread[i], NULL, &retune, philo) != 0)
			ft_error("Error\n");
		philo->id = i++;
	}
	i = 0;
	while (i++ < philo->data.num_philo)
	{
		if (pthread_join(philo->data.tread[i], NULL) != 0)
			ft_error("Error\n");
	}
}

int	main(int ac, char **argv)
{
	// t_data	tread;
	t_philosopher philo;
	int i;

	if (ac != 5 && ac != 6)
		ft_error("Error in number of arguments\n");
	check_arguments(argv);
	ft_initial(argv, ac, &philo);
	i = 0;
	while (i++ < philo.data.num_fork)
		pthread_mutex_init(&(philo.data.forks[i]), NULL);
	philos(&philo);
	// ft_creat_fork(&tread);
	i = 0;
	while (i++ < philo.data.num_fork)
		pthread_mutex_destroy(&philo.data.forks[i]);
	return (0);
}
