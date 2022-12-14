#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "get_next_line.h"
#include "libft.h"
#include "structs.h"

void	add_valve_to_list(t_visited **list_visited, char *str)
{
	t_visited	*new;
	new = malloc(sizeof(t_visited));
	new->valvename = ft_strdup(str);
	new->next = *list_visited;
	*list_visited = new;
}

int	check_if_valid(t_valve *temp, t_visited *list_visited)
{
	t_visited *temporary = list_visited;

	while (temporary != NULL)
	{
		if (ft_strncmp(temp->valvename, temporary->valvename, 2) == 0)
			return (1);
		temporary = temporary->next;
	}
	return (0);
}

t_route	*new_loc(char *str, int steps)
{
	t_route	*new_loc = malloc(sizeof(t_route));
	new_loc->valvevisited = ft_strdup(str);
	new_loc->steps = steps;
	new_loc->next = NULL;
	return (new_loc);
}

void	put_loc_back(t_route **head, t_route *new)
{
	t_route *temp;

	temp = *head;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

int	compare(char *str, t_route *end)
{
	while (end != NULL)
	{
		if (ft_strncmp(str, end->valvevisited, 2) == 0)
			return (0);
		end = end->next;
	}
	return (1);
}

void	make_new_locations(t_valve *firstvalve, char *str, int steps, t_route *end)
{
	int	i = 0;
	t_route	*new;
	while (ft_strncmp(firstvalve->valvename, str, 2) != 0)
		firstvalve = firstvalve->next;
	while (*(firstvalve->neigbors + i) != NULL)
	{
		if (compare(*(firstvalve->neigbors + i), end))
			put_loc_back(&end, new_loc(*(firstvalve->neigbors + i), steps));
		i++;
	}
}

int	find_shortest_route(t_valve *firstvalve, t_valve *current, t_valve *next_valve)
{
	int	steps = 0;
	t_valve	*temp;
	t_route	*end;
	t_route *temproute;

	end = new_loc(next_valve->valvename, steps);
	temproute = end;
	temp = firstvalve;
	printf("strings %s\n", current->valvename);
	printf("strings %s\n", next_valve->valvename);

	while (1)
	{
		while (temproute != NULL && temproute->steps == steps)
		{
			if (ft_strncmp(temproute->valvevisited, current->valvename, 2) == 0)
				return (steps);
			make_new_locations(firstvalve, temproute->valvevisited, steps + 1, end);
			temproute = temproute->next;
		}
		steps++;
	}
}

void get_steps_and_go_next(t_valve *first_valve,  t_valve *current, t_valve *next_valve, int remaining_steps, int *maxpressure, t_visited *list_visited, int pressure)
{
	int	steps = find_shortest_route(first_valve, current, next_valve);
	remaining_steps = remaining_steps - steps - 1;
	add_valve_to_list(&list_visited, next_valve->valvename);
	do_somthing(first_valve, next_valve, remaining_steps, maxpressure, list_visited, pressure);
}

void printlist(t_visited *list)
{
	while (list != NULL)
	{
		printf("%s\n", list->valvename);
		list = list->next;
	}
}

void	do_somthing(t_valve *first_valve,  t_valve *current, int remaining_steps, int *maxpressure, t_visited *list_visited, int pressure)
{
	t_valve *temp;

	printf("hi\n %d\n", remaining_steps);
	if (remaining_steps > 0)
		pressure = pressure + remaining_steps * current->flowrate;
	else
	{
		if (pressure > *maxpressure)
			*maxpressure = pressure;
		return ;
	}
	temp = first_valve;
	while (temp != NULL)
	{
		if (temp->flowrate == 0)
		{
			temp = temp->next;		
			continue ;
		}
		if (check_if_valid(temp, list_visited))
		{
			temp = temp->next;		
			continue ;
		}
		get_steps_and_go_next(first_valve, current, temp, remaining_steps, maxpressure, list_visited, pressure);
		temp = temp->next;
	}
	if (temp == NULL)
	{
		printf ("\n\npressure here %d %d\n", pressure, *maxpressure);
		if (pressure > *maxpressure)
			*maxpressure = pressure;
		return ;
	}
}

void	get_pressure(t_valve *first_valve)
{
	int		steps = 30;
	int		maxpressure = 0;
	int		pressure = 0;
	t_valve	*temp;
	t_visited	*list_visited = NULL;

	temp = first_valve;
	while (ft_strncmp(temp->valvename, "AA", 2) != 0)
		temp = temp->next;
	do_somthing(first_valve, temp, steps, &maxpressure, list_visited, pressure);
	printf("max pressure : %d\n", maxpressure);
}

int	main()
{
	t_valve	*first_valve;
	first_valve = get_valve_list();
	// printvalves(first_valve);
	get_pressure(first_valve);
}
