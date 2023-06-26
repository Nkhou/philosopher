#include "philosopher.h"

unsigned long	get_time()
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	ft_clear(t_data *data)
{
	int i;

	i = 0;
	while (i < data->num_fork)
		pthread_mutex_destroy(&data->forks[i++]);
	pthread_mutex_destroy(&data->lock);
	pthread_mutex_unlock(&data->write);
	pthread_mutex_destroy(&data->write);
	while(i < data->num_fork)
	{
		free(&data->forks[i]);
		i++;
	}
	free(data->forks);
	free(data->philo);
	free(data);
	return (0);
}

int	eating(t_philosopher	*philo)
{
	long long	tmp;
	t_data *data;
	
	data = philo->data;
	tmp = get_time() - data->start;
	philo->time_to_eat_meal =  get_time();
	pthread_mutex_lock(&data->write);
	printf("%lld %d is eating \n", tmp, philo->id);
	pthread_mutex_unlock(&data->write);
	ft_usleep(data->time_to_eat);
	return (0);
}

void	take_fork(t_philosopher	*philo)
{
	long long	tmp;

	pthread_mutex_lock(&philo->data->write);
	tmp = get_time() - philo->data->start;
	printf("%lld %d  has taken a fork \n", tmp, philo->id);
	pthread_mutex_unlock(&philo->data->write);
}

void	sleeping(t_philosopher	*philo)
{
	long long	tmp;

	pthread_mutex_lock(&philo->data->write);
	tmp = get_time() - philo->data->start;
	printf("%lld %d is sleeping \n", tmp, philo->id);
	pthread_mutex_unlock(&philo->data->write);
	ft_usleep(philo->data->time_to_sleep);
}
void	*routune_philo(void *tred)
{
	t_philosopher			*philo;
	t_data *data;

	philo = (t_philosopher *)tred;

	data = philo->data;
	while (data->stop)
	{
		if (!data && !(philo->num_time_was_eat < data->num_time_to_eat || ( data->num_time_to_eat == -1)))
			return (0);

		if (philo->id % 2 == 0 && data->start )
		pthread_mutex_lock(philo->left_fork);
		if (data && data->stop)
			take_fork(philo);
		else
		{
			pthread_mutex_unlock(philo->left_fork);
			return (0);
		}
		pthread_mutex_lock(philo->right_fork);
		if (data && data->stop)
		{
			take_fork(philo);
			eating(philo);
		}
		else
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return (0);
		}
		if (data->num_time_to_eat && data->stop)
		{
			philo->num_time_was_eat = philo->num_time_was_eat + 1;
		}
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		if (data && data->stop)
			sleeping(philo);
	}
	return (0);
}
int	check_arguments(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				ft_error("Error in number of arguments\n");
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}
int check_intial(t_data *data)
{
	if (data->num_time_to_eat == 0)
		return (0);
	if (!data->num_philo)
		return (0);
	if (!data->num_fork)
		return (0);
	if (!data->philo)
		return (0);
	if (!data->forks)
		return (0);
	if (!data->time_to_die)
		return (0);
	if (!data->time_to_eat)
		return (0);
	if (!data->time_to_sleep)
		return (0);
	return (1);
}
int	ft_initial(char **argv, int ac, t_data *data)
{
	if (ac == 6)
		data->num_time_to_eat = ft_atoi(argv[ac - 1]);
	else
		data->num_time_to_eat = -1;
	data->num_philo = ft_atoi(argv[1]);
	data->num_fork = ft_atoi(argv[1]);
	data->philo = (t_philosopher *) malloc(sizeof(t_philosopher) * data->num_philo);
	if (!data->philo)
		return (0);
	data->forks = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * data->num_fork);
	if (!data->forks)
		return(0);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (!check_intial(data))
		return (0);
	return (1);
}

void	insial_fork(t_data *data)
{
	int i;

	i = 0;
	data->start = get_time();
	while (i < data->num_fork)
	{
		data->philo[i].data = data;
		data->philo[i].num_time_was_eat = 0;
		data->philo[i].time_to_eat_meal = get_time();
		data->philo[i].left_fork = &data->forks[i];
		data->philo[i].right_fork = &data->forks[(i + 1) % data->num_philo];
		data->philo[i].id = i + 1;
		data->stop = 1;
		i++;
	}
}

void	*check_philo(void *tread)
{
	int	i;
	t_data	*data;
	unsigned long	tmp;

	i = 0;
	data = (t_data *) tread;
	while (1)
	{
		if (!(data->philo->num_time_was_eat < data->num_time_to_eat || ( data->num_time_to_eat == -1)))
			return (0);
		tmp = get_time();
		pthread_mutex_lock(&data->lock);
		if ((get_time() - data->philo[i].time_to_eat_meal) > (unsigned long)data->time_to_die)
		{
			pthread_mutex_lock(&data->write);
			printf("%lu %d is die \n", tmp - data->start, data->philo[i].id);
			pthread_mutex_unlock(&data->write);
			data->stop = 0;
			i = 0;

			while (i < data->num_philo)
			{
				data->philo[i].data->stop = data->stop;
				i++;
			}
			pthread_mutex_unlock(&data->lock);
			return(NULL);
		}
		i++;
		i = i % data->num_philo;
		usleep(100);
	}
	return (0);
}

int philos(t_data *data)
{
	int i;

	i = 0;
	insial_fork(data);
	while (i < data->num_philo)
	{
		if (pthread_create(&data->philo[i].tread, NULL, &routune_philo, &data->philo[i]) != 0)
			return (0);
		pthread_detach(data->philo[i].tread);
		i++;
	}
	if (check_philo(data) == 0)
		return (0);
	return (1);
}

int	main(int ac, char **argv)
{

	t_data *data;

	if (ac != 5 && ac != 6)
		ft_error("Error in number of arguments\n");
	if (!check_arguments(argv))
		return (0);
	data = (t_data *)malloc(sizeof(t_data) );
	if (!data)
		return (0);
	memset(data,0,sizeof(t_data ));
	ft_initial_mutex(data);
	if (!ft_initial(argv, ac, data))
	{
		free(data->philo);
		free(data->forks);
		free(data);
		return (0);
	}
	if (!philos(data))
	{
		printf("ha ana\n");
		if (!ft_clear(data))
			return (0);
	}
	return (0);
}
