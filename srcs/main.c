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

void int_handler(int sig)
{
	(void) sig;
	ft_exit_message("Catching signal.. Exiting");
}
unsigned short in_cksum(unsigned short *addr, int len)
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

t_r_ping	receive_ping(t_argv av)
{
	char buf[4096];
	socklen_t src_addr_len = sizeof(struct sockaddr_in);
	t_r_ping	r_ping;

	r_ping.bytes = recvfrom(av.sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&r_ping.src_addr, &src_addr_len);
	if (r_ping.bytes < 0)
		return (r_ping);

	r_ping.ip_head = (struct ip *)buf;
	r_ping.icmp_head = (struct icmp *)(buf + (r_ping.ip_head->ip_hl * 4));

	return (r_ping);
}

void setup_ping(t_argv av, t_s_ping *s_ping, int seq)
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
	s_ping->dest_addr.sin_addr.s_addr = inet_addr(av.rhost);
	
	setsockopt(av.sockfd, IPPROTO_IP, IP_TTL, &av.ttl, sizeof(av.ttl));

	struct timeval	tv_out;
	tv_out.tv_sec = 1;
	tv_out.tv_usec = 0;

	setsockopt(av.sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out);
}

void	send_ping(t_argv av, int seq)
{
	int			bytes;
	t_s_ping	s_ping;

	setup_ping(av, &s_ping, seq);
	bytes = sendto(av.sockfd, s_ping.packet, av.payload_size + sizeof(struct icmp), 0,
					(struct sockaddr *)&s_ping.dest_addr, sizeof(s_ping.dest_addr));
	if (bytes < 0)
		ft_exit_message("Fatal Error: Couldn't send icmp packet");
}

void	show_response(t_r_ping r_ping, double time)
{
	int	payload_size;

	payload_size = r_ping.bytes - (r_ping.ip_head->ip_hl * 8);
	if (r_ping.icmp_head->icmp_type == ICMP_ECHOREPLY)
		printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%0.3f ms\n", payload_size, inet_ntoa(r_ping.src_addr.sin_addr),
														htons(r_ping.icmp_head->icmp_seq), r_ping.ip_head->ip_ttl,
														time);
	else if (r_ping.icmp_head->icmp_type == ICMP_TIME_EXCEEDED)
		printf("%d bytes from %s: Time to live exceeded\n", payload_size, inet_ntoa(r_ping.src_addr.sin_addr));
}

void	ping(t_argv av)
{
	int				seq;
	double			s_time;
	double			e_time;
	t_r_ping		r_ping;

	printf("PING %s (%s): %d data bytes\n", av.host, av.rhost, av.payload_size);
	
	seq = 0;
	while (42)
	{
		if (seq != 0)
			sleep(1);
		
		s_time = get_time();
		send_ping(av, seq);

		r_ping = receive_ping(av);
		e_time = get_time();
		
		show_response(r_ping, e_time - s_time);
		seq++;
	}
}

int main(int argc, char **argv)
{
	t_argv				av;

	signal(SIGINT, int_handler);

	av = parse_argv(argc, argv);
	av.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (av.sockfd < 0)
		ft_exit_message("Fatal Error: Socket could not create (are you root ?)");
	
	ping(av);
	return (0);
}