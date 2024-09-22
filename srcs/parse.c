/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 19:41:17 by TheRed            #+#    #+#             */
/*   Updated: 2024/09/05 19:41:17 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

t_argv	parse_argv(int argc, char **argv)
{
	int		i;
	t_argv	av;

	av.tos = 0;
	av.count = 0;
	av.ip_timestamp = -1;
	av.ttl = DEFAULT_TTL;
	av.wait = DEFAULT_WAIT;
	av.preload = DEFAULT_PRELOAD;
	av.interval = DEFAULT_INTERVAL;
	av.payload_size = DEFAULT_PAYLOAD_SIZE;
	av.quiet = false;
	av.force = false;
	av.no_route = false;
	av.reverse_dns = true;

	if (argc == 1)
		ft_exit_message("Usage: %s [host]", argv[0]);

	i = 1;
	while (i < argc && i < argc)
	{
		if (ft_strncmp(argv[i], "--ttl", 5) == 0)
			av.ttl = ft_atoi(argv[i + 1]);
		else if (ft_strncmp(argv[i], "-s", 2) == 0)
			av.payload_size = ft_atoi(argv[i + 1]);
		else if (ft_strncmp(argv[i], "-c", 2) == 0)
			av.count = ft_atoi(argv[i + 1]);
		else if (ft_strncmp(argv[i], "-W", 2) == 0)
			av.wait = ft_atoi(argv[i + 1]);
		else if (ft_strncmp(argv[i], "-i", 2) == 0)
			av.interval = ft_atoi(argv[i + 1]);
		else if (ft_strncmp(argv[i], "-l", 2) == 0)
			av.preload = ft_atoi(argv[i + 1]);
		else if (ft_strncmp(argv[i], "-T", 2) == 0)
			av.tos = ft_atoi(argv[i + 1]);
		else if (ft_strncmp(argv[i], "--ip-timestamp", 14) == 0)
			av.ip_timestamp = ft_strncmp(argv[i + 1], "tsonly", 6) == 0 ? 1 : 0;
		else if (ft_strncmp(argv[i], "-q", 2) == 0)
			av.quiet = true;
		else if (ft_strncmp(argv[i], "-f", 2) == 0)
			av.force = true;
		else if (ft_strncmp(argv[i], "-n", 2) == 0)
			av.reverse_dns = false;
		else if (ft_strncmp(argv[i], "-r", 2) == 0)
			av.no_route = true;

		i++;
	}

	return (av);
}

bool	verify_parsing_value(t_argv av)
{
	if (av.ttl < 1 || av.payload_size < 0 || av.count < 0 || av.wait < 0 || av.interval < 0 || av.preload < 0)
	{
		ft_printf("ft_ping: option value too small.\n");
		return (false);
	}
	if (av.ttl > 255 || av.payload_size > MAX_PAYLOAD_SIZE || av.tos > 255)
	{
		ft_printf("ft_ping: option value too big.\n");
		return (false);
	}

	return (true);
}

bool	verify_parsing(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (is_param_value(argv[i]) && i + 1 >= argc)
		{
			ft_printf("%s: option requires an argument %s\n", argv[0], argv[i]);
			return (false);
		}
		i++;
	}

	return (true);
}