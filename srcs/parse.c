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

	av.count = 0;
	av.ttl = DEFAULT_TTL;
	av.payload_size = DEFAULT_PAYLOAD_SIZE;

	if (argc == 1)
		ft_exit_message("Usage: %s [host]", argv[0]);

	i = 1;
	while (i < argc)
	{
		if (ft_strncmp(argv[i], "--ttl", 5) == 0)
			av.ttl = ft_atoi(argv[i + 1]);
		else if (ft_strncmp(argv[i], "-s", 2) == 0)
			av.payload_size = ft_atoi(argv[i + 1]);
		else if (ft_strncmp(argv[i], "-c", 2) == 0)
			av.count = ft_atoi(argv[i + 1]);
		i++;
	}

	return (av);
}