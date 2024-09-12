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

void	ping(t_host host, t_argv av)
{
	int				seq;
	t_stats			stats;
	double			s_time;
	double			c_time;
	t_r_ping		r_ping;

	init_stats(&stats);
	
	printf("PING %s (%s): %d data bytes\n", host.host, host.rhost, av.payload_size);

	seq = 0;
	while (seq < av.count || av.count == 0)
	{
		if (seq != 0)
			sleep(1);
		
		s_time = get_time();
		send_ping(host, av, seq);

		r_ping = receive_ping(host);
		c_time = get_time() - s_time;
		
		show_response(r_ping, c_time);
		update_stats(r_ping, c_time, &stats);
		seq++;
	}
	show_stats(host, stats);
}

int main(int argc, char **argv)
{
	int		i;
	t_argv	av;
	t_host	host;

	signal(SIGINT, int_handler);
	
	av = parse_argv(argc, argv);
	
	i = 0;
	while (argv[++i])
	{
		if (argv[i][0] != '-')
		{
			host.host = argv[i];
			host.rhost = dns_lookup(host.host);
			host.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
			
			if (host.sockfd < 0)
				ft_exit_message("Fatal Error: Socket could not create (are you root ?)");

			ping(host, av);
		}
		else
			i++;
	}
	return (0);
}