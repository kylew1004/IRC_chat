/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:12:00 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:13:04 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/SockServer.hpp"

void SockServer::invite(SockServer &srv, std::vector<std::string> &args, User& user) {
	if (args[0] != "INVITE" || args.size() <= 2) {
		sendMessage(user.fd, std::string(ERR_NEEDMOREPARAMS(user.nick, args[0])) + "\n", std::cerr);
		return;
	}

	if (!cInSet(args[2][0], "#&+!")) {
		std::cerr << "Not a channel";
		return ;
	}
	std::map<std::basic_string<char>, Channels >::iterator chan = srv._chans.find(args[2]);
	if (chan == srv._chans.end()) {
		sendMessage(user.fd, std::string(ERR_NOSUCHCHANNEL(user.nick, args[2])) + "\n", std::cerr);
		return;
	}
	if (!user.channels.count(&chan->second)) {
		sendMessage(user.fd, std::string(ERR_NOTONCHANNEL(user.nick, chan->first)) + "\n", std::cerr);
		return;
	}
	if (chan->second.isOper(user.fd) == false && chan->second.isMode('i') == true) {
		sendMessage(user.fd, std::string(ERR_CHANOPRIVSNEEDED(user.nick, chan->first)) + "\n", std::cerr);
		return;
	}

	User *u_invit = srv.getUserByNick(args[1]);
	if (u_invit == NULL) {
		sendMessage(user.fd, std::string(ERR_NOSUCHNICK(user.nick, args[1])) + "\n", std::cerr);
		return;
	}
	if (u_invit->channels.count(&chan->second)) {
		sendMessage(user.fd, std::string(ERR_USERONCHANNEL(user.nick, u_invit->nick , chan->first)) + "\n", std::cerr);
		return;
	}
	chan->second.joinChannel(u_invit->fd);
	u_invit->enterChannel(&chan->second, false);
	transmitToChannelFromServ(chan->second, JOIN(u_invit->nick, u_invit->user, chan->first) + "\n");
	if (!chan->second.getTopic().empty()) {
		sendMessage(u_invit->fd, TOPIC(u_invit->nick, chan->second.getName()) + chan->second.getTopic() + "\n", std::cout);
	}
	std::vector<std::string> a;
	a.push_back("NAMES"); a.push_back(chan->second.getName());
	names(srv, a, *u_invit);
}
