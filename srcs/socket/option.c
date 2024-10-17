/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:26:32 by TheRed            #+#    #+#             */
/*   Updated: 2024/09/21 16:26:32 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#define MAX_IPOPTLEN 40

void	set_option(t_argv av, int sockfd)
{
	int				route_opt = (int)av.no_route;
	int				tos_value = av.tos;
	struct timeval	tv_out;

	setsockopt(sockfd, IPPROTO_IP, IP_TTL, &av.ttl, sizeof(av.ttl));

	tv_out.tv_sec = av.wait;
	tv_out.tv_usec = 0;

	if (av.force)
	{
		tv_out.tv_sec = 0;
		tv_out.tv_usec = 1000 * 10;
	}

	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out);

	setsockopt(sockfd, SOL_SOCKET, SO_DONTROUTE, &route_opt, sizeof(route_opt));
	
	setsockopt(sockfd, IPPROTO_IP, IP_TOS, &tos_value, sizeof(tos_value));

}