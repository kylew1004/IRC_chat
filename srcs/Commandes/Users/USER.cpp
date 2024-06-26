/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:09:17 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:13:43 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/SockServer.hpp"

void SockServer::user(SockServer &srv, std::vector<std::string>& args, User& user) {
	if (args[0] != "USER" || args.size() < 5) {
		if (user.nick.empty())
			sendMessage(user.fd, std::string(ERR_NEEDMOREPARAMS_NONICK(args[0])) + "\n", std::cerr);
		else
			sendMessage(user.fd, std::string(ERR_NEEDMOREPARAMS(user.nick, args[0])) + "\n", std::cerr);
		return;
	}
	if (!user.nick.empty() && !user.realName.empty()){
		sendMessage(user.fd, ERR_ALREADYREGISTERED(user.nick) + "\n", std::cerr);
		return;
	}
	if (*args[4].begin() == ':') {
		user.user = args[1];
		args[4].erase(0,1);
		user.realName = args[4];
	}
	else
		return ;
	for (size_t i = 5; i < args.size(); i++) {
		user.realName += " " + args[i];
	}
	std::cout << "Name is set" << std::endl;
	welcome(srv, args, user);
}
