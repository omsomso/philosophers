/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:35:35 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/20 13:12:32 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H

# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>
# include <string.h>

# define U_SEC 1000000UL
# define M_SEC 1000UL
# define INT_MAX 2147483647
# define INT_MIN -2147483648

typedef struct	s_data
{
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	init_lock;
	pthread_mutex_t	printf_lock;
	pthread_mutex_t	hour_lock;
	pthread_mutex_t	time_lock;
	pthread_mutex_t	end_lock;
	pthread_mutex_t	*forks;
	pthread_t		*thread;
	int				nb_phil;
	int				nb_forks;
	int				*meals_had;
	unsigned long	*death_hour;
	unsigned long	start_sec;
	unsigned long	start_msec;
	int				time_sleep;
	int				time_eat;
	int				time_death;
	int				max_meals;
	int				end;
	int				end_log;
}	t_data;

typedef struct	s_philo
{
	int				nb_phil;
	int				nb_forks;
	int				thread_id;
	int				lo;
	int				end_status;
	unsigned long	start_sec;
	unsigned long	ms_to_die;
}	t_philo;

int	handle_error(char **s, int argnb);
char	check_input(char **s, int argnb);
int	init_mutex(t_data *data, int i);
int	init_data(t_data *data, char **s, int argnb);
void	destroy_everything(t_data *data, int i);

void	*be_born(void *tmp);
void	find_lock_values(t_philo *philo);
unsigned long	get_sim_msec(t_data *data, int start);

void	set_death_hour(t_data *data, t_philo *philo);
int	check_meals_had(t_data *data, int thread_id);
int	check_death(t_data *data, t_philo *philo);
int	philo_log(t_data *data, t_philo *philo, char *s);

int	philosophise(t_data *data, t_philo *philo);
int	take_fork_odd(t_data *data, t_philo *philo);
int	take_fork_even(t_data *data, t_philo *philo);
int	eat_n_sleep(t_data *data, t_philo *philo);
void	*handle_end(t_data *data, t_philo *philo);

int	ft_isdigit(int c);
size_t	ft_strlen(const char *s);
long	ft_atol(const char *s);
void	ft_putendl_fd(char *s, int fd);
void	*ft_calloc(size_t nmemb, size_t size);

#endif