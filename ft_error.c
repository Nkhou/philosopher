/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhoudro <nkhoudro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 15:40:31 by nkhoudro          #+#    #+#             */
/*   Updated: 2023/07/02 14:45:06 by nkhoudro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	ft_error(char *str)
{
	ft_putstr_fd(str, 2);
}

int	check_number(int ac)
{
	if (ac != 5 && ac != 6)
	{
		ft_error("Error \n");
		return (0);
	}
	return (1);
}
