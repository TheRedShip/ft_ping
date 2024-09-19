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
		return (ft_strdup(inet_ntoa(*addr_list[i])));
		i++;
	}
	return (NULL);
}

char *reverse_dns_lookup(char *ip_addr)
{
    struct sockaddr_in sa;
    char host[NI_MAXHOST];
    int res;

    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    
    if (inet_pton(AF_INET, ip_addr, &(sa.sin_addr)) != 1)
        ft_exit_message("ft_ping: invalid IP address %s", ip_addr);

    res = getnameinfo((struct sockaddr *)&sa, sizeof(struct sockaddr_in),
                      host, NI_MAXHOST, NULL, 0, NI_NAMEREQD);

    if (res != 0)
        return (ft_strdup(ip_addr));
    return (ft_strdup(host));
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