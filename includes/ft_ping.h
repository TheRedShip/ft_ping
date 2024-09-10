/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 19:36:15 by TheRed            #+#    #+#             */
/*   Updated: 2024/09/05 19:36:15 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <string.h>
# include <netinet/ip_icmp.h>
# include <time.h>
# include <fcntl.h>
# include <signal.h>

# include "libft.h"

#define DEFAULT_TTL 64
#define DEFAULT_PAYLOAD_SIZE 56
#define MAX_PAYLOAD_SIZE 9999

typedef struct	s_argv
{
	int		ttl;
	int		payload_size;

	char	*host;
	char	*rhost;
	int		sockfd;
}				t_argv;

typedef struct	s_ping
{
	char				packet[MAX_PAYLOAD_SIZE];
	struct sockaddr_in	dest_addr;
}				t_ping;

// PARSING //

t_argv	parse_argv(int argc, char **argv);

// UTILS //

void	ft_exit_message(char *message, ...);
char	*dns_lookup(char *addr);


#endif