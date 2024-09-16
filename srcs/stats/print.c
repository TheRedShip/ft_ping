/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 01:15:16 by TheRed            #+#    #+#             */
/*   Updated: 2024/09/13 01:15:16 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	show_response(t_r_ping r_ping, double time)
{
	int	payload_size;

	if (r_ping.bytes < 0)
		return ;
	payload_size = r_ping.bytes - (r_ping.ip_head->ip_hl * 4);
	if (r_ping.icmp_head->icmp_type == ICMP_ECHOREPLY)
		printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%0.3f ms\n", payload_size, inet_ntoa(r_ping.src_addr.sin_addr),
														htons(r_ping.icmp_head->icmp_seq), r_ping.ip_head->ip_ttl, time);
	else if (r_ping.icmp_head->icmp_type == ICMP_TIME_EXCEEDED)
		printf("%d bytes from %s: Time to live exceeded\n", payload_size, inet_ntoa(r_ping.src_addr.sin_addr));
}

void	show_stats(t_host host, t_stats stats)
{
	printf("--- %s ft_ping statistics ---\n", host.host);
	printf("%d packets transmitted, %d packets received, %d%% packet loss\n",
			stats.sent, stats.recv, 100 - (100 * stats.recv / stats.sent));
	
	if (stats.recv != 0)
		printf("round-trip min/avg/max/stddev = %0.3f/%0.3f/%0.3f/%0.3f ms\n",
				stats.min, stats.avg / stats.recv, stats.max, calculate_standard_deviation(stats));
	printf("\n");	
}