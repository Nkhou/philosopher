/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:35:16 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/05/30 11:52:05 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>

typedef struct s_philosopher
{
	pthread_t	*tread;
	int			num_philo;
	int			num_fork;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			num_time_to_eat;
	pthread_mutex_t mutex;
}	t_philosopher;

void	ft_error(char *str);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
int		ft_atoi(const char *str);
#endif