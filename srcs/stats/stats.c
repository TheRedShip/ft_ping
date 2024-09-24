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
	if (stats.recv == 1)
		return (0);
	return (sqrt(stats.mdev / (stats.recv - 1)));
}

void	update_stats(int r_bytes, int response_code, double c_time, t_stats *stats)
{
	static double	mean = 0;

	if (stats->sent == 0)
		mean = 0;

	if (r_bytes < 0 || response_code == ICMP_TIME_EXCEEDED)
	{
		stats->lost++;
		return ;
	}
	else if (response_code == ICMP_ECHOREPLY)
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