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

void set_ip_timestamp(int sockfd, int mode)
{
    unsigned char opt[MAX_IPOPTLEN];
    memset(opt, 0, sizeof(opt));

    // Define the IP Timestamp option
    opt[0] = 0x44;  // Option type: Timestamp (68 in decimal or 0x44 in hex)
    opt[1] = 4 + 4; // Option length: 4 bytes for header + 4 bytes per entry
    opt[2] = 5;     // Pointer: Start after the initial 4 bytes (position in option array)
    
    // Mode (tsonly or tsaddr)
    opt[3] = (mode == 1) ? 0 : 1;  // 0 = tsonly, 1 = tsaddr
	
    // Set the IP_OPTIONS to the socket
    if (setsockopt(sockfd, IPPROTO_IP, IP_OPTIONS, opt, sizeof(opt)) < 0) {
        perror("Error setting IP timestamp option");
        exit(EXIT_FAILURE);
    }
}

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
		tv_out.tv_usec = 1000 * 100;
	}
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out);

	setsockopt(sockfd, SOL_SOCKET, SO_DONTROUTE, &route_opt, sizeof(route_opt));
	
	setsockopt(sockfd, IPPROTO_IP, IP_TOS, &tos_value, sizeof(tos_value));

	if (av.ip_timestamp != -1)
		set_ip_timestamp(sockfd, av.ip_timestamp);
}