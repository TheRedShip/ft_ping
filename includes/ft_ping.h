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
# include <math.h> 

# include "libft.h"

# define DEFAULT_TTL 64
# define DEFAULT_WAIT 10
# define DEFAULT_PRELOAD 0
# define DEFAULT_INTERVAL 1
# define DEFAULT_PAYLOAD_SIZE 56
# define MAX_PAYLOAD_SIZE 9999

// extern bool	interrupted;

typedef struct	s_argv
{
	int		ttl;
	int		wait;
	int		count;
	int		preload;
	int		interval;
	int		payload_size;
	bool	force;
}				t_argv;

typedef struct	s_host
{
	char	*host;
	char	*rhost;
	int		sockfd;
}				t_host;

typedef struct	s_stats
{
	int		sent;
	int		recv;
	int		lost;
	double	mdev;
	double	min;
	double	max;
	double	avg;
}				t_stats;

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

t_argv			parse_argv(int argc, char **argv);


// SOCKETS //

void			send_ping(t_host host, t_argv av, int seq);

t_r_ping		receive_ping(t_host host);
t_r_ping		receive_ping_force(t_host host);

unsigned short	in_cksum(unsigned short *addr, int len);
char			*dns_lookup(char *addr);

// UTILS //

void			ft_exit_message(char *message, ...);
double			get_time(void);
bool			is_param_value(char *param);

// STATS //

void			show_response(t_r_ping r_ping, double time);
void			show_stats(t_host host, t_stats stats);

void			init_stats(t_stats *stats);
void			update_stats(t_r_ping r_ping, double c_time, t_stats *stats);
double			calculate_standard_deviation(t_stats stats);

#endif