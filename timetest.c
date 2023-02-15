#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define	U_SEC 1000000UL
#define	M_SEC 1000UL
#define	INT_MAX 2147483647
#define	INT_MIN -2147483648

typedef struct	s_data
{
	pthread_mutex_t	lock;
	unsigned long	start_time;
	unsigned long	start_sec;
	int				nb_phil;
	int				table_status[100]; //n
	int				meals_had[100]; //n
	long			death_hour[100]; //n
	int				time_sleep;
	int				time_eat;
	int				time_death;
	int				max_meals;
	int				end;
} t_data;


void	get_start_time(t_data *data)
{
	struct	timeval tv;
	struct	timezone tz;

	gettimeofday(&tv, &tz);
	data->start_time = 0;
	data->start_sec = tv.tv_sec;
	//return (tv.tv_usec);
}

unsigned long	get_msec(t_data *data)
{
	struct	timeval tv;
	struct	timezone tz;
	unsigned long	milisec;
	unsigned long	seconds;

	gettimeofday(&tv, &tz);

	seconds = tv.tv_sec - data->start_sec;
	milisec = (tv.tv_usec / M_SEC) + (seconds * M_SEC);
	//printf("%lu\n", tv.tv_usec / M_SEC);
	printf("%lu\n", milisec);
	//need smth to add the usecs, goes to 999 999 i guess
	
	return (milisec);
}

int	main(void)
{
	t_data	*data;

	data = malloc(sizeof(data));
	get_start_time(data);
	while (1)
	get_msec(data);
	return (0);
}