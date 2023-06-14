/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TIME.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:10:11 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:13:37 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/SockServer.hpp"

void SockServer::time(SockServer &, std::vector<std::string> &, User& user) {
	sendMessage(user.fd, TIME(user.nick, getCurrentTime()) + "\n", std::cout);
}
