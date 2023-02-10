#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void	timer(int sec)
{
	struct	timeval tv;
	struct	timezone tz;
	long	old_time;

	old_time = 0;
	while (sec > 0)
	{
		gettimeofday(&tv, &tz);
		if (old_time != tv.tv_sec)
		{
			printf("time remaining : %d seconds\n", sec);
			sec--;
		}
		old_time = tv.tv_sec;
	}
	printf("TIMER END\n");
}
int	main(int argc, char **argv)
{
	timer(atoi(argv[1]));
	return (0);
}