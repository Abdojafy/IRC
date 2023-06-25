/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozahid- <ozahid-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 11:57:39 by junik             #+#    #+#             */
/*   Updated: 2023/06/25 11:56:59 by ozahid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <exception>
#include <stdexcept>
#include <sys/socket.h>

int	handle_port(std::string port_);
bool	handle_password(std::string password_);

using namespace std;