/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:10:41 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:26:44 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/SockServer.hpp"

void SockServer::privmsg(SockServer &srv, std::vector<std::string> &args, User &user) {
	if (args.size() < 3) {
		sendMessage(user.fd, std::string(ERR_NEEDMOREPARAMS(user.nick, args[0])) + "\n", std::cerr);
		return;
	}

	if (*args[2].begin() != ':') {
		sendMessage(user.fd, std::string(ERR_NOTEXTTOSEND(user.nick)) + "\n", std::cerr);
		return;
	}
	args[2].erase(0,1);
	std::string message = args[2];
	for (size_t i = 3; i < args.size(); i++) {
		message += " " + args[i];
	}

	if (cInSet(args[1][0], "#&+!")) {
		std::map<std::basic_string<char>, Channels >::iterator chan = srv._chans.find(args[1]);
		if (chan == srv._chans.end()) {
			sendMessage(user.fd, std::string(ERR_NOSUCHCHANNEL(user.nick, args[1])) + "\n", std::cerr);
			return;
		}
		/*
		if (chan->second.isMode('n') && !user.channels.count(&chan->second)) {
			sendMessage(user.fd, std::string(ERR_NOTONCHANNEL(user.nick, chan->first)) + "\n", std::cerr);
			return;
		}
		*/
		transmitToChannel(chan->second, user, PRIVMSG(user.nick, user.user, args[1]) + message + "\n");
	} 
	else {
		User *target = srv.getUserByNick(args[1]);
		if (!target) {
			sendMessage(user.fd, std::string(ERR_NOSUCHNICK(user.nick, args[1])) + "\n", std::cerr);
			return;
		}
		sendMessage(target->fd, PRIVMSG(user.nick, user.user, target->nick) + message + "\n", std::cout);
	}
}
