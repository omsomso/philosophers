#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

typedef struct	s_data
{
	pthread_mutex_t	lock_id;
} t_data;

void	*thread_test(void *data)
{
	static int i = 0;

	t_data	*dddd;
	dddd =(t_data*)data;
	pthread_mutex_lock(&dddd->lock_id);
	i++;
	printf("waiting in thread %d...\n", i);
	sleep(1);
	printf("and thread %d ends now :(\n", i);
	pthread_mutex_unlock(&dddd->lock_id);
	return (NULL);
}

int	main(void)
{
	pthread_t	thread_id[3];
	int			i = 0;
	t_data		data;

	pthread_mutex_init(&data.lock_id, NULL);
	printf("waiting...\n");
	sleep(1);
	printf("over\n");
	while (i < 2)
	{
		pthread_create(&thread_id[i], NULL, thread_test, &data);
		i++;
	}
	sleep(3);
	printf("program ends now\n");
	//pthread_join(thread_id[0], NULL);
	//pthread_join(thread_id[1], NULL);
	pthread_mutex_destroy(&data.lock_id);
	return (0);
}