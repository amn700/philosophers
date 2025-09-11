/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:00:00 by codespace         #+#    #+#             */
/*   Updated: 2025/09/11 10:41:12 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void	print_state_write_mandatory(long long timestamp, int id, char *state)
{
	ssize_t	ret;

	ft_putlongnbr(timestamp);
	ret = write(1, " ", 1);
	(void)ret;
	ft_putlongnbr(id);
	ret = write(1, " ", 1);
	(void)ret;
	if (state[0] == 'h' && state[1] == 'a')
		ret = write(1, "has taken a fork\n", 17);
	else if (state[0] == 'i' && state[1] == 's' && state[2] == ' '
		&& state[3] == 'e')
		ret = write(1, "is eating\n", 10);
	else if (state[0] == 'i' && state[1] == 's' && state[2] == ' '
		&& state[3] == 's')
		ret = write(1, "is sleeping\n", 12);
	else if (state[0] == 'i' && state[1] == 's' && state[2] == ' '
		&& state[3] == 't')
		ret = write(1, "is thinking\n", 12);
	(void)ret;
}
