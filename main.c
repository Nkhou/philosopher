/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:33:28 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/06/01 21:29:05 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	*retune()
{
	// pthread_mutex_lock(&tread.mutex);
	printf("hello everyone\n");
	printf("hello goodbye\n");
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

void	ft_initial(char **argv, int ac, t_philosopher *tread)
{
	int	i;

	i = 1;
	if (ac == 6)
		tread->num_time_to_eat = ft_atoi(argv[ac - 1]);
	tread->num_philo = ft_atoi(argv[1]);
	tread->num_fork = ft_atoi(argv[1]);
	tread->tread = (pthread_t *) malloc(sizeof(pthread_t) * tread->num_philo);
	tread->time_to_die = ft_atoi(argv[2]);
	tread->time_to_eat = ft_atoi(argv[3]);
	tread->time_to_sleep = ft_atoi(argv[4]);
}

void	ft_creat_fork(t_philosopher *tread)
{
	int i;

	i = 0;
	while (i < tread->num_fork)
	{
		if (pthread_create(&tread->tread[i], NULL, &retune, NULL) != 0)
			ft_error("Error\n");
		if (pthread_join(tread->tread[i], NULL) != 0)
			ft_error("Error\n");
		i++;
	}
}
int	main(int ac, char **argv)
{
	t_philosopher	tread;

	if (ac != 5 && ac != 6)
		ft_error("Error in number of arguments\n");
	pthread_mutex_init(&(tread.mutex), NULL);
	check_arguments(argv);
	ft_initial(argv, ac, &tread);
	ft_creat_fork(&tread);
	pthread_mutex_destroy(&tread.mutex);
	return (0);
}
