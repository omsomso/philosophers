/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:35:35 by kpawlows          #+#    #+#             */
/*   Updated: 2023/04/02 17:53:29 by kpawlows         ###   ########.fr       */
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

struct	s_data;

typedef struct s_philo
{
	unsigned long	death_time;
	pthread_t		thread;
	int				id;
	int				lo;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	end_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	check_dth_lock;
	pthread_mutex_t	forks[210];
	int				meals_had[210];
	int				nb_phil;
	unsigned long	start_msec;
	int				time_sleep;
	int				time_eat;
	int				time_death;
	int				max_meals;
	int				end;
	t_philo			philo[210];
}	t_data;

int				write_error(char **s, int argnb);
char			check_input(char **s, int argnb, int i, int j);
int				init_mutex(t_data *data);
int				init_data(t_data *data, char **s, int argnb);
void			destroy_everything(t_data *data);

int				start_sim(t_data *data);
void			monitor(void *tmp);
int				monitor_food(t_data *data);

void			*be_born(void *tmp);
void			philosophise(t_data *data, t_philo *philo);
void			take_fork(t_data *data, t_philo *philo);
void			sleep_well(t_data *data, unsigned long time);
int				philo_log(t_data *data, t_philo *philo, char *s);

void			find_forks(t_data *data, t_philo *philo);
int				check_meals(t_data *data);
int				add_meal(t_data *data, int thread_id);
int				check_death(t_data *data);
int				check_end(t_data *data);

unsigned long	get_time(void);
unsigned long	get_sim_time(t_data *data);
void			set_death_hour(t_data *data, t_philo *philo);

int				ft_isdigit(int c);
size_t			ft_strlen(const char *s);
long			ft_atol(const char *s);
void			ft_putendl_fd(char *s, int fd);

#endif