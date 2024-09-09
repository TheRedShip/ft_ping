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

void receive_ping(t_argv av, int seq)
{
	char buf[4096];
	struct sockaddr_in src_addr;
	socklen_t src_addr_len = sizeof(src_addr);
	struct ip *ip_header;
	struct icmp *icmp_header;
	int bytes_received;

	bytes_received = recvfrom(av.sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&src_addr, &src_addr_len);
	if (bytes_received < 0)
		ft_exit_message("Error: Failed to receive icmp reply");

	ip_header = (struct ip *)buf;
	icmp_header = (struct icmp *)(buf + (ip_header->ip_hl * 4));
	printf("%d\n", icmp_header->icmp_cksum);

	if (icmp_header->icmp_type == ICMP_ECHOREPLY)
	{
		printf("%d bytes from %s: icmp_seq=%d\n", bytes_received, av.rhost, seq);
		// printf("Received ICMP echo reply from %s\n", inet_ntoa(src_addr.sin_addr));
		// printf("ICMP header:\n");
		// printf("  Type: %d\n", icmp_header->icmp_type);
		// printf("  Code: %d\n", icmp_header->icmp_code);
		// printf("  ID: %d\n", icmp_header->icmp_id);
		// printf("  Sequence: %d\n", icmp_header->icmp_seq);
	}
}

void setup_ping(t_argv av, t_ping *ping)
{
	struct icmp *icmp_header;
	int			packet_size;

	packet_size = sizeof(struct icmp);
	ping->packet = malloc(packet_size);
	memset(ping->packet, 0, packet_size);

	icmp_header = (struct icmp *)ping->packet;
	icmp_header->icmp_type = ICMP_ECHO;
	icmp_header->icmp_code = 0;
	icmp_header->icmp_id = getpid();
	icmp_header->icmp_seq = 0;
	icmp_header->icmp_cksum = 0;

	icmp_header->icmp_cksum = in_cksum((unsigned short *)icmp_header, packet_size);

	memset(&ping->dest_addr, 0, sizeof(struct sockaddr_in));
	ping->dest_addr.sin_family = AF_INET;
	ping->dest_addr.sin_addr.s_addr = inet_addr(av.rhost);

	int	ttl_val = 5;
	setsockopt(av.sockfd, IPPROTO_IP, IP_TTL, &ttl_val, sizeof(ttl_val));
}

void	send_ping(t_argv av, t_ping ping)
{
	if (sendto(av.sockfd, ping.packet, sizeof(struct icmp), 0, (struct sockaddr *)&ping.dest_addr, sizeof(ping.dest_addr)) < 0)
	{
		free(ping.packet);
		ft_exit_message("Fatal Error: Couldn't send icmp packet");
	}
}

void	ping(t_argv av)
{
	int		seq;
	t_ping	ping;

	setup_ping(av, &ping);
	printf("PING %s (%s): (unknown data bytes)\n", av.host, av.rhost);
	
	seq = 0;
	while (seq < 4)
	{
		if (seq != 0)
			sleep(1);

		send_ping(av, ping);
		receive_ping(av, seq);

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