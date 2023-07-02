/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 15:40:31 by nkhoudro          #+#    #+#             */
<<<<<<< HEAD:ft_error.c
/*   Updated: 2023/06/26 22:47:12 by nkhoudro         ###   ########.fr       */
=======
/*   Updated: 2023/07/02 14:45:06 by nkhoudro         ###   ########.fr       */
>>>>>>> temp-branch:philo/ft_error.c
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	ft_error(char *str)
{
	ft_putstr_fd(str, 2);
<<<<<<< HEAD:ft_error.c
=======
}

int	check_number(int ac)
{
	if (ac != 5 && ac != 6)
	{
		ft_error("Error \n");
		return (0);
	}
	return (1);
>>>>>>> temp-branch:philo/ft_error.c
}
