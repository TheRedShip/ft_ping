/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 01:11:42 by TheRed            #+#    #+#             */
/*   Updated: 2024/09/13 01:11:42 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	setup_ping(t_s_ping *s_ping, t_host host, t_argv av, int seq)
{
	struct icmp *icmp_header;
	int			packet_size;

	memset(s_ping, 0, sizeof(t_s_ping));

	packet_size = av.payload_size + sizeof(struct icmp);

	icmp_header = (struct icmp *)s_ping->packet;
	icmp_header->icmp_type = ICMP_ECHO;
	icmp_header->icmp_code = 0;
	icmp_header->icmp_id = getpid();
	icmp_header->icmp_seq = htons(seq);

	memset(s_ping->packet + sizeof(struct icmp), 0x42, av.payload_size);

	icmp_header->icmp_cksum = 0;
	icmp_header->icmp_cksum = in_cksum((unsigned short *)icmp_header, packet_size);

	s_ping->dest_addr.sin_family = AF_INET;
	s_ping->dest_addr.sin_addr.s_addr = inet_addr(host.rhost);
}

void	send_ping(t_host host, t_argv av, int seq)
{
	t_s_ping	s_ping;

	setup_ping(&s_ping, host, av, seq);
	if (sendto(host.sockfd, s_ping.packet, av.payload_size + sizeof(struct icmp), 0,
					(struct sockaddr *)&s_ping.dest_addr, sizeof(s_ping.dest_addr)) < 0)
		ft_exit_message("ft_ping: sending packet: %s\n", strerror(errno));
}