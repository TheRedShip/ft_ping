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

char	*dns_lookup(char *addr)
{
	struct hostent	*host;
	struct in_addr	**addr_list;
	int				i;

	if ((host = gethostbyname(addr)) == NULL)
		ft_exit_message("ft_ping: unknown host %s", addr);
	addr_list = (struct in_addr **)host->h_addr_list;
	i = 0;
	while (addr_list[i] != NULL)
	{
		return (inet_ntoa(*addr_list[i]));
		i++;
	}
	return (NULL);
}