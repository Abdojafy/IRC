/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junik <abderrachidyassir@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 11:52:54 by junik             #+#    #+#             */
/*   Updated: 2023/06/15 12:46:38 by junik            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"

void	start(string port, string password)
{
	
}

int	main(int ac, char **av)
{
	if (ac == 3)
		start(av[1], av[2]);
	else
		throw(runtime_error("Error\n"));
}