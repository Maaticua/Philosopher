/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:28:35 by macaruan          #+#    #+#             */
/*   Updated: 2025/07/23 14:55:05 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "philo.h"

typedef struct s_philo	t_philo;

long					get_time(void);
void					ft_usleep(int duration);
void					print_state(t_philo *philo, const char *msg);
void					take_forks(t_philo *philo);
void					release_forks(t_philo *philo);

#endif
