NAME = philo
CC = cc -Wall -Wextra -Werror #-fsanitize=thread
SRC = \
		main.c\
		ft_error.c\
		ft_putstr.c\
		ft_putchar.c\
		ft_atoi.c\
		ft_usleep.c\
		philo_utils.c\
		routine_utils.c\
		right_left.c\

OBJCTS = $(SRC:.c=.o)
all : $(NAME)
$(NAME) : $(OBJCTS) 
		$(CC) $(CFLAGS) $^ -o $@
%.o: %.c philosopher.h
	$(CC) $(CFLAGS) -c $< -o $@ 
	
clean:
	rm -f $(OBJCTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: fclean clean all re bonus