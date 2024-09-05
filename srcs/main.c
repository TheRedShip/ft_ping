/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 19:38:54 by TheRed            #+#    #+#             */
/*   Updated: 2024/09/05 19:38:54 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

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

int main(int argc, char **argv)
{
	t_argv	av;

	av = parse_argv(argc, argv);
	printf("PING %s (%s) 56(84) bytes of data.\n", av.host, dns_lookup(av.host));
	return (0);
}