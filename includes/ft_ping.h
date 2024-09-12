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
# include <sys/time.h>
# include <time.h>
# include <signal.h>

# include "libft.h"

#define DEFAULT_TTL 64
#define DEFAULT_PAYLOAD_SIZE 56
#define MAX_PAYLOAD_SIZE 9999

typedef struct	s_host
{
	char	*host;
	char	*rhost;


	struct s_host *next;
}				t_host;

typedef struct	s_stats
{
	
}

typedef struct	s_argv
{
	int		ttl;
	int		payload_size;

	char	*host;
	char	*rhost;
	int		sockfd;
}				t_argv;

typedef struct	s_s_ping
{
	char				packet[MAX_PAYLOAD_SIZE];
	struct sockaddr_in	dest_addr;
}				t_s_ping;

typedef struct	s_r_ping
{
	int					bytes;
	struct ip			*ip_head;
	struct icmp			*icmp_head;
	struct sockaddr_in	src_addr;
}				t_r_ping;



// PARSING //

t_argv	parse_argv(int argc, char **argv);

// UTILS //

void	ft_exit_message(char *message, ...);
char	*dns_lookup(char *addr);
double	get_time(void);


#endif