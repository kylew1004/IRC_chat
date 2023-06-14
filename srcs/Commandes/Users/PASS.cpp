/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:09:33 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:13:45 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/SockServer.hpp"

void SockServer::pass(SockServer& srv, std::vector<std::string>& args, User& user) {
	if (args.size() != 2 || args[0] != "PASS") {
		if (user.nick.empty())
			sendMessage(user.fd, std::string(ERR_NEEDMOREPARAMS_NONICK(args[0])) + "\n", std::cerr);
		else
			sendMessage(user.fd, std::string(ERR_NEEDMOREPARAMS(user.nick, args[0])) + "\n", std::cerr);
		return;
	}
	if (!user.nick.empty() && !user.realName.empty()){
		sendMessage(user.fd, ERR_ALREADYREGISTERED(user.nick) + "\n", std::cout);
		return;
	}
	if (args[1] == srv.password)
		user.pass = 1;
	else
		user.pass = 0;
}
