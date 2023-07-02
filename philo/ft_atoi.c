/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 18:21:38 by nkhoudro          #+#    #+#             */
<<<<<<< HEAD:ft_atoi.c
/*   Updated: 2023/06/26 22:47:31 by nkhoudro         ###   ########.fr       */
=======
/*   Updated: 2023/07/02 13:59:02 by nkhoudro         ###   ########.fr       */
>>>>>>> temp-branch:philo/ft_atoi.c
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	ft_at(const char *str, int sgn)
{
	long	i;
	long	res;
	long	rv;

	i = 0;
	res = 0;
	while (str[i] >= '0' && str[i] <= '9' && str[i] != '\0')
	{
		rv = res;
		res = (res * 10) + (str[i] - '0');
		if (res / 10 != rv)
		{
			if (sgn == -1)
				return (0);
			return (-1);
		}
		i++;
	}
	return (res);
}

int	ft_atoi(const char *str)
{
	long	i;
	long	sgn;
	long	res;

	i = 0;
	sgn = 1;
	res = 0;
	if (!str)
<<<<<<< HEAD:ft_atoi.c
		return(0);
=======
	{
		return (0);
	}
>>>>>>> temp-branch:philo/ft_atoi.c
	while ((str[i] > 8 && str[i] < 14) || (str[i] == ' '))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
<<<<<<< HEAD:ft_atoi.c
			return(0);
=======
			return (0);
>>>>>>> temp-branch:philo/ft_atoi.c
		i++;
	}
	res = ft_at(str + i, sgn);
	return (res * sgn);
}
