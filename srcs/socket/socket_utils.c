/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 01:09:51 by TheRed            #+#    #+#             */
/*   Updated: 2024/09/13 01:09:51 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

char			*dns_lookup(char *addr)
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

unsigned short	in_cksum(unsigned short *addr, int len)
{
	int nleft = len;
	int sum = 0;
	unsigned short *w = addr;
	unsigned short answer = 0;

	while (nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1)
	{
		*(unsigned char *)(&answer) = *(unsigned char *)w;
		sum += answer;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}