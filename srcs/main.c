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

void receive_ping(int sockfd)
{
	char buf[4096];
	struct sockaddr_in src_addr;
	socklen_t src_addr_len = sizeof(src_addr);
	struct ip *ip_header;
	struct icmp *icmp_header;
	int bytes_received;

	bytes_received = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&src_addr, &src_addr_len);
	if (bytes_received < 0)
		ft_exit_message("Error: Failed to receive ping response");

	ip_header = (struct ip *)buf;
	icmp_header = (struct icmp *)(buf + (ip_header->ip_hl * 4));

	if (icmp_header->icmp_type == ICMP_ECHOREPLY)
	{
		printf("Received ICMP echo reply from %s\n", inet_ntoa(src_addr.sin_addr));
		printf("ICMP header:\n");
		printf("  Type: %d\n", icmp_header->icmp_type);
		printf("  Code: %d\n", icmp_header->icmp_code);
		printf("  ID: %d\n", icmp_header->icmp_id);
		printf("  Sequence: %d\n", icmp_header->icmp_seq);
	}
	else
	{
		printf("Received ICMP packet, but not an echo reply\n");
	}
}

void send_ping(t_argv av)
{
	struct sockaddr_in dest_addr;
	struct icmp *icmp_header;
	char *packet;
	int packet_size, sent_bytes;

	packet_size = sizeof(struct icmp);
	packet = malloc(packet_size);
	memset(packet, 0, packet_size);

	icmp_header = (struct icmp *)packet;
	icmp_header->icmp_type = ICMP_ECHO;
	icmp_header->icmp_code = 0;
	icmp_header->icmp_id = getpid();
	icmp_header->icmp_seq = 0;
	icmp_header->icmp_cksum = 0;

	icmp_header->icmp_cksum = in_cksum((unsigned short *)icmp_header, packet_size);

	memset(&dest_addr, 0, sizeof(struct sockaddr_in));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_addr.s_addr = inet_addr(dns_lookup(av.host));

	sent_bytes = sendto(av.sockfd, packet, packet_size, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
	free(packet);

	if (sent_bytes < 0)
		ft_exit_message("Error: Failed to send ping packet");

	printf("Packet sent\n");

	receive_ping(av.sockfd);
}

int main(int argc, char **argv)
{
	t_argv				av;

	signal(SIGINT, int_handler);

	av = parse_argv(argc, argv);
	av.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (av.sockfd < 0)
		ft_exit_message("Fatal Error: Socket could not create (are you root ?)");
	
	send_ping(av);
	return (0);
}