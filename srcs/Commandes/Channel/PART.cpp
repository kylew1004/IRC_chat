/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:10:54 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 18:15:28 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/SockServer.hpp"

void SockServer::part(SockServer &srv, std::vector<std::string> &args, User& user) {
	if (args.size() < 2) {
		sendMessage(user.fd, std::string(ERR_NEEDMOREPARAMS(user.nick, args[0])) + "\n", std::cerr);
		return;
	}
	std::string mess = "Reason";
	for (size_t i = 2; i < args.size(); i++) {
		if (i == 2) {
			mess += ":";
			args[i].erase(0,1);
		}
		mess += " " + args[i];
	}
	if (mess == "Reason")
		mess += " not provided";
	mess += "\n";

	for (size_t i = 0; i < args[1].size(); i++) {
		std::string chans = "";
		for (; i < args[1].size() && args[1][i] != ','; i++)
			chans.push_back(args[1][i]);
		std::map<std::basic_string<char>, Channels>::iterator chan = srv._chans.find(chans);
		if (chan == srv._chans.end()) {
			sendMessage(user.fd, std::string(ERR_NOSUCHCHANNEL(user.nick, chans)) + "\n", std::cout);
			continue;
		}
		if (!user.channels.count(&chan->second)) {
			sendMessage(user.fd, std::string(ERR_NOTONCHANNEL(user.nick, chan->first)) + "\n", std::cout);
			continue;
		}
		transmitToChannelFromServ(chan->second, PART(user.nick, user.user, chan->second.getName()) + mess);
		int fd_op = user.leaveChannel(&chan->second);
		if (fd_op != -1) {
			srv._users[fd_op].channels[&chan->second] = true;
			transmitToChannelFromServ(chan->second, CHANOPER(chan->first, srv._users[fd_op].nick));
		}
		if (chan->second.isEmpty())
			srv._chans.erase(chan->second.getName());
	}
}
