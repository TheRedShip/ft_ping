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

void print_hex_dump(const unsigned char *buf, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        printf("%02x", buf[i]);
		if (i % 2 == 1)
			printf(" ");
    }
    printf("\n");
}

void print_ip_header(struct ip *ip)
{
    printf("IP Hdr Dump:\n ");
    print_hex_dump((unsigned char *)ip, ip->ip_hl * 4);
    printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst     Data\n");
    printf(" %1x  %1x  %02x %04x %04x   %1x %04x  %02x  %02x %04x ",
           ip->ip_v, ip->ip_hl, ip->ip_tos, ntohs(ip->ip_len), ntohs(ip->ip_id),
           ntohs(ip->ip_off) >> 13, ntohs(ip->ip_off) & 0x1FFF, ip->ip_ttl, ip->ip_p, ntohs(ip->ip_sum));
    printf("%s  ", inet_ntoa(ip->ip_src));
    printf("%s\n", inet_ntoa(ip->ip_dst));
}

void print_icmp_packet(struct icmp *icmp, int size)
{
    printf("ICMP: type %d, code %d, size %d, id 0x%04x, seq 0x%04x\n",
           icmp->icmp_type, icmp->icmp_code, size,
           ntohs(icmp->icmp_id), htons(icmp->icmp_seq));
}

void	print_verbose_error(t_r_ping r_ping)
{
	print_ip_header(r_ping.ip_head);
    print_icmp_packet(r_ping.icmp_head, r_ping.bytes - (r_ping.ip_head->ip_hl * 4));
	printf("\n");
}

void	show_response(t_argv av, t_r_ping r_ping, double time)
{
	char	*host;
	char	*reverse_host;
	int		payload_size;

	if (r_ping.bytes < 0)
		return ;

	payload_size = r_ping.bytes - (r_ping.ip_head->ip_hl * 4);
	if (r_ping.icmp_head->icmp_type == ICMP_ECHOREPLY && !av.quiet)
		printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%0.3f ms\n", payload_size, inet_ntoa(r_ping.src_addr.sin_addr),
														htons(r_ping.icmp_head->icmp_seq), r_ping.ip_head->ip_ttl, time);
	else if (r_ping.icmp_head->icmp_type == ICMP_TIME_EXCEEDED)
	{
		host = inet_ntoa(r_ping.src_addr.sin_addr);

		if (av.reverse_dns)
		{
			reverse_host = reverse_dns_lookup(host);
			printf("%d bytes from %s (%s): Time to live exceeded\n", payload_size, reverse_host, host);
			free(reverse_host);
		}
		else
			printf("%d bytes from %s: Time to live exceeded\n", payload_size, host);

		print_verbose_error(r_ping);
	}
}

void	show_stats(t_host host, t_stats stats)
{
	if (stats.sent == 0)
		stats.sent++;

	printf("--- %s ft_ping statistics ---\n", host.host);
	printf("%d packets transmitted, %d packets received, %d%% packet loss\n",
			stats.sent, stats.recv, 100 - (100 * stats.recv / stats.sent));
	
	if (stats.recv != 0)
		printf("round-trip min/avg/max/stddev = %0.3f/%0.3f/%0.3f/%0.3f ms\n",
				stats.min, stats.avg / stats.recv, stats.max, calculate_standard_deviation(stats));
	printf("\n");	
}