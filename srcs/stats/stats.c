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

void	update_standard_deviation(double c_time, t_stats *stats, double *mean)
{
	double	delta;
	double	delta2;
	
	delta = c_time - *mean;
	*mean += delta / stats->recv;

	delta2 = c_time - *mean;
	stats->mdev += delta * delta2;
}

double	calculate_standard_deviation(t_stats stats)
{
	return (sqrt(stats.mdev / (stats.recv - 1)));
}

void	update_stats(t_r_ping r_ping, double c_time, t_stats *stats)
{
	static double	mean = 0;

	if (stats->sent == 0)
		mean = 0;

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

		update_standard_deviation(c_time, stats, &mean);
	}
}