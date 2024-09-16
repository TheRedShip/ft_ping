/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 20:03:36 by TheRed            #+#    #+#             */
/*   Updated: 2024/09/05 20:03:36 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	ft_exit_message(char *message, ...)
{
	va_list	args;

	va_start(args, message);
	vfprintf(stderr, message, args);
	va_end(args);
	fprintf(stderr, "\n");
	exit(1);
}

double	get_time(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000.0) + (ts.tv_nsec / 1e6);
}

bool	is_param_value(char *param)
{
	if (ft_strncmp(param, "--ttl", 5) == 0)
		return (true);
	if (ft_strncmp(param, "-s", 2) == 0)
		return (true);
	if (ft_strncmp(param, "-c", 2) == 0)
		return (true);
	if (ft_strncmp(param, "-W", 2) == 0)
		return (true);
	if (ft_strncmp(param, "-i", 2) == 0)
		return (true);
	if (ft_strncmp(param, "-l", 2) == 0)
		return (true);
	if (ft_strncmp(param, "-f", 2) == 0)
		return (false);
	return (false);
}