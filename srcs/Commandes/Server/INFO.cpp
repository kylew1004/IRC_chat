/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INFO.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:10:06 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:13:35 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/SockServer.hpp"

void SockServer::info(SockServer &srv, std::vector<std::string> &, User& user) {
	sendMessage(user.fd, INFO(user.nick, std::string("Time: ") + getCurrentTime()) + "\n", std::cout);
	std::ostringstream ss(""); ss << "Members: " << srv._fds.size();
 	sendMessage(user.fd, INFO(user.nick, ss.str()) + "\n", std::cout);
	ss.str(""); ss << "Port: " << srv._port;
	sendMessage(user.fd, INFO(user.nick, ss.str()) + "\n", std::cout);
	sendMessage(user.fd, std::string(ENDOFINFO(user.nick)) + "\n", std::cout);
}
