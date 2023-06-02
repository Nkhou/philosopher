/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:35:16 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/06/02 21:19:30 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>

typedef struct s_data
{
	pthread_t	*tread;
	int			num_philo;
	int			num_fork;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			index;
	int			num_time_to_eat;
	pthread_mutex_t forks;
	pthread_mutex_t lock;
	pthread_mutex_t write;
}	t_data;

typedef struct s_philosopher
{
	// struct *data;
	pthread_t tmp;
	int id;
	int			num_time_to_eat;
	int			num_time_was_eat;
	int			time_to_die;
	
} t_philosopher;
void	ft_error(char *str);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
int		ft_atoi(const char *str);
#endif