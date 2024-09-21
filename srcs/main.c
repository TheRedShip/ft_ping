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

bool	interrupted = false;

void int_handler(int sig)
{
	(void) sig;
	interrupted = true;
}

void	ping(t_host host, t_argv av)
{
	int				seq;
	t_stats			stats;
	double			s_time;
	double			c_time;
	t_r_ping		r_ping;
	int				response_code;

	printf("PING %s (%s): %d data bytes\n", host.host, host.rhost, av.payload_size);
	init_stats(&stats);

	seq = 0;
	do
	{
		if (!av.force && seq && !av.preload)
			sleep(av.interval);
		else
			usleep(1000 * 3);

		if (av.preload > 0)
			av.preload--;
		
		s_time = get_time();
		send_ping(host, av, seq);

		stats.sent++;
		if (interrupted)
			break;

		r_ping = receive_ping(host);
		c_time = get_time() - s_time;

		response_code = r_ping.icmp_head->icmp_type;
		
		if (!av.force)
			show_response(av, r_ping, c_time);

		update_stats(r_ping.bytes, response_code, c_time, &stats);
		seq++;
	}
	while (!interrupted && (seq < av.count || av.count == 0));

	show_stats(host, stats);
}

int main(int argc, char **argv)
{
	int		i;
	t_argv	av;
	t_host	host;

	signal(SIGINT, int_handler);
	
	av = parse_argv(argc, argv);
	
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] != '-')
		{
			host.host = argv[i];
			host.rhost = dns_lookup(host.host);
			host.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
			
			if (host.sockfd < 0)
				ft_exit_message("Fatal Error: Socket could not create (are you root ?)");

			set_option(av, host.sockfd);
			
			ping(host, av);
			free(host.rhost);
		}
		else if (is_param_value(argv[i]))
			i++;
		i++;
	}
	return (0);
}