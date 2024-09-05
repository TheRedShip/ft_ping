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

	av.host = NULL;

	if (argc == 1)
		ft_exit_message("Usage: %s [host]", argv[0]);

	i = 1;
	while (i < argc)
	{
		if (ft_strncmp(argv[i], "-", 1) == 0)
			continue ; //params
		else if (av.host == NULL)
			av.host = argv[i];
		i++;
	}

	return (av);
}