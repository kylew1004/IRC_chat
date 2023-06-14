/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:11:15 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:13:10 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/SockServer.hpp"

void SockServer::kick(SockServer &srv, std::vector<std::string>& args, User& user)
{
	if (args.size() < 3 && args[0] != "KICK") {
		sendMessage(user.fd, std::string(ERR_NEEDMOREPARAMS(user.nick, args[0]	)) + "\n", std::cout);
		return;
	}
	if (!cInSet(args[1][0], "#&+!")) {
		std::cerr << "Not a channel";
		return ;
	}
	std::map<std::basic_string<char>, Channels >::iterator chan = srv._chans.find(args[1]);
	if (chan == srv._chans.end()) {
		sendMessage(user.fd, std::string(ERR_NOSUCHCHANNEL(user.nick, args[1])) + "\n", std::cerr);
		return;
	}
	if (!user.channels.count(&chan->second)) {
		sendMessage(user.fd, std::string(ERR_NOTONCHANNEL(user.nick, chan->first)) + "\n", std::cerr);
		return ;
	}
	if (chan->second.isOper(user.fd) == false) {
		sendMessage(user.fd, std::string(ERR_CHANOPRIVSNEEDED(user.nick, chan->first)) + "\n", std::cerr);
		return ;
	}
	User *u_kick = srv.getUserByNick(args[2]);
	if (u_kick == NULL) {
		sendMessage(user.fd, std::string(ERR_NOSUCHNICK(user.nick, args[2])) + "\n", std::cerr);
		return;
	}
	if (!u_kick->channels.count(&chan->second)) {
		sendMessage(user.fd, std::string(ERR_USERNOTINCHANNEL(user.nick, u_kick->nick , chan->first)) + "\n", std::cerr);
		return ;
	}

	std::string mess = "Was kicked. Reason";
	if (args[3] != ":")
		for (size_t i = 3; i < args.size(); i++) {
			if (i == 3) {
				mess += ": ";
				args[i].erase(0,1);
			}
			mess += " " + args[i];
		}
	if (mess == "Was kicked. Reason")
		mess += " not provided";
	mess += "\n";
	std::string rep;
	for (std::vector<std::string>::iterator arg = args.begin() + 1; arg != args.end(); arg++)
		rep += *arg + " ";

	transmitToChannelFromServ(chan->second, KICK(user.nick, user.user) + rep + "\n");
	transmitToChannelFromServ(chan->second, PART(u_kick->nick, u_kick->user, chan->second.getName()) + mess);

	int fd_op = u_kick->leaveChannel(&chan->second);
	if (fd_op != -1) {
		srv._users[fd_op].channels[&chan->second] = true;
		transmitToChannelFromServ(chan->second, CHANOPER(chan->first, srv._users[fd_op].nick));
	}
	if (chan->second.isEmpty())
		srv._chans.erase(chan->second.getName());
}
