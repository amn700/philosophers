/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:00:00 by codespace         #+#    #+#             */
/*   Updated: 2025/09/11 10:41:12 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

void	ft_write_error(char *msg)
{
	ssize_t	ret;

	ret = write(1, msg, ft_strlen(msg));
	(void)ret;
}

void	ft_putlongnbr(long long nbr)
{
	ssize_t	ret;

	if (nbr == LLONG_MIN)
	{
		ret = write(1, "-9223372036854775808", 20);
		(void)ret;
		return ;
	}
	if (nbr < 0)
	{
		ret = write(1, "-", 1);
		(void)ret;
		nbr = -nbr;
	}
	if (nbr > 9)
	{
		ft_putlongnbr(nbr / 10);
		ret = write(1, &"0123456789"[nbr % 10], 1);
		(void)ret;
	}
	else
	{
		ret = write(1, &"0123456789"[nbr % 10], 1);
		(void)ret;
	}
}
