/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 01:26:01 by TheRed            #+#    #+#             */
/*   Updated: 2024/09/13 01:26:01 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	init_stats(t_stats *stats)
{
	stats->sent = 0;
	stats->recv = 0;
	stats->lost = 0;
	stats->mdev = 0;
	stats->min = 0;
	stats->max = 0;
	stats->avg = 0;
}

void	update_stats(t_r_ping r_ping, double c_time, t_stats *stats)
{
	stats->sent++;
	
	if (r_ping.bytes < 0 || r_ping.icmp_head->icmp_type == ICMP_TIME_EXCEEDED)
	{
		stats->lost++;
		return ;
	}
	else if (r_ping.icmp_head->icmp_type == ICMP_ECHOREPLY)
	{
		stats->recv++;
		if (stats->min == 0 || c_time < stats->min)
			stats->min = c_time;
		if (c_time > stats->max)
			stats->max = c_time;
		stats->avg += c_time;
	}
}