/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:35:16 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/06/27 10:59:47 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <string.h>
 #include <sys/time.h>

typedef struct s_philosopher
{
	pthread_t	tread;
	int			id;
	unsigned long	time_to_eat_meal;
	int			num_time_was_eat;
	int			num_time_to_eat;
	struct s_data *data;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
} t_philosopher;

typedef struct s_data
{
	struct s_philosopher *philo;
	int			num_fork;
	int			index;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			num_philo;
	int			stop;
	unsigned long		start;
	int			num_time_to_eat;
	pthread_mutex_t *forks;
	pthread_mutex_t *meal;
	pthread_mutex_t lock;
	pthread_mutex_t write;
}	t_data;

typedef struct s_check
{
	pthread_t tread;
	struct s_data *data;
} t_check;

void	ft_error(char *str);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
int		ft_atoi(const char *str);
void	ft_usleep(unsigned long time);
unsigned long	get_time();
int	check_arguments(char **argv);
int check_intial(t_data *data);
int	ft_initial(char **argv, int ac, t_data *data);
void	insial_fork(t_data *data);
int	ft_clear(t_data *data);
unsigned long	get_time();
int	eating(t_philosopher	*philo);
void	take_fork(t_philosopher	*philo);
void	sleeping(t_philosopher	*philo);
int check_condition(t_data *data, int i);
int ft_initial_mutex(t_data *data);
#endif