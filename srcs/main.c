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
	t_stats			stats;
	double			s_time;
	double			c_time;
	t_r_ping		r_ping;
	int				response_code;

	printf("PING %s (%s): %d data bytes", host.host, host.rhost, av.payload_size);
	if (av.verbose)
		printf(", id 0x%04x = %d\n", getpid(), getpid());
	printf("\n");

	memset(&stats, 0, sizeof(t_stats));
	memset(&r_ping, 0, sizeof(t_r_ping));

	do
	{
		if (!av.force && stats.sent && !av.preload)
			sleep(av.interval);
		else
			usleep(1000 * 3);

		if (av.preload > 0)
			av.preload--;
		
		s_time = get_time();
		send_ping(host, av, stats.sent);

		if (interrupted)
			break;

		stats.sent++;

		r_ping = receive_ping(host);
		c_time = get_time() - s_time;

		if (r_ping.bytes < 0)
			continue ;

		response_code = r_ping.icmp_head->icmp_type;
		
		if (!av.force)
			show_response(av, r_ping, c_time);
		
		free(r_ping.ip_head);
		update_stats(r_ping.bytes, response_code, c_time, &stats);
	}
	while (!interrupted && (stats.sent < av.count || av.count == 0));

	show_stats(host, stats);
}

int main(int argc, char **argv)
{
	int		i;
	t_argv	av;
	t_host	host;

	memset(&av, 0, sizeof(t_argv));
	memset(&host, 0, sizeof(t_host));
	signal(SIGINT, int_handler);
	
	if (!verify_parsing(argc, argv))
		ft_exit_message("Try '%s -?' for more information.", argv[0]);
	
	av = parse_argv(argc, argv);
	if (av.help)
		ft_exit_message(HELP_MESSAGE, argv[0]);

	if (!verify_parsing_value(av))
		return (1);

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

	if (host.host == NULL)
		ft_exit_message("%s: missing host operand\nTry '%s -?' for more information.", argv[0], argv[0]);

	return (0);
}
