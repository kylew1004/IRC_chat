/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:09:43 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:13:38 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/SockServer.hpp"

void SockServer::nick(SockServer& srv, std::vector<std::string>& args, User& user) {
	if (args.size() != 2 || args[0] != "NICK") {
		if (user.nick.empty())
			sendMessage(user.fd, std::string(ERR_NONICKNAMEGIVEN_NONICK) + "\n", std::cerr);
		else
			sendMessage(user.fd, std::string(ERR_NONICKNAMEGIVEN(user.nick)) + "\n", std::cerr);
		return;
	}
	std::string oldNick;
	stringVector::iterator itu = srv.getNicks().end();
	for (stringVector::iterator it = srv.getNicks().begin(); it != srv.getNicks().end(); it++) {
		if (user.nick == *it) {
			itu = it;
			oldNick = user.nick;
		}
		if (args[1] == *it) {
			sendMessage(user.fd, ERR_NICKNAMEINUSE(args[1]) + "\n", std::cerr);
			return;
		}
	}
	user.nick = args[1];
	if (itu != srv.getNicks().end()) {
		sendMessage(user.fd, NICK(oldNick, user.user) + args[1] + "\n", std::cout);
		srv.getNicks().erase(itu);
	}
	else
		welcome(srv, args, user);
	srv.getNicks().push_back(args[1]);
	std::cout << "Nick is set" << std::endl;
}
