/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 16:02:18 by nkhoudro          #+#    #+#             */
<<<<<<< HEAD:ft_usleep.c
/*   Updated: 2023/06/27 10:59:29 by nkhoudro         ###   ########.fr       */
=======
/*   Updated: 2023/07/01 14:35:37 by nkhoudro         ###   ########.fr       */
>>>>>>> temp-branch:philo/ft_usleep.c
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	ft_usleep(unsigned long time)
{
	unsigned long	tmp;

	tmp = get_time();
	while (get_time() - tmp < time)
		usleep(100);
}

<<<<<<< HEAD:ft_usleep.c
unsigned long	get_time()
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
=======
unsigned long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
>>>>>>> temp-branch:philo/ft_usleep.c
