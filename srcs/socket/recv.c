/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 01:14:41 by TheRed            #+#    #+#             */
/*   Updated: 2024/09/13 01:14:41 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

t_r_ping	receive_ping(t_host host)
{
	t_r_ping	r_ping;
	char		*buf;
	socklen_t	src_addr_len = sizeof(struct sockaddr_in);

	memset(&r_ping, 0, sizeof(t_r_ping));

	buf = ft_calloc(1, 4096);
	if (!buf)
		ft_exit_message("ft_ping: %s\n", strerror(errno));

	r_ping.bytes = recvfrom(host.sockfd, buf, 4096, 0, (struct sockaddr *)&r_ping.src_addr, &src_addr_len);
	if (r_ping.bytes < 0)
	{
		free(buf);
		return (r_ping);
	}

	r_ping.ip_head = (struct ip *)buf;
	r_ping.icmp_head = (struct icmp *)(buf + (r_ping.ip_head->ip_hl * 4));

	return (r_ping);
}
