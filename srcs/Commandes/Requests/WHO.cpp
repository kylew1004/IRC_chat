/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:10:20 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:13:33 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/SockServer.hpp"

typedef std::vector<User *>(*listFunc)(SockServer &, const std::string &);

static void sendMessageByUserMatch(SockServer &srv, const User& sender, listFunc fc, const std::string& arg, const std::string& match) {
	std::vector<User *> userList = fc(srv, arg);
	if (userList.empty())
		return;
	SockServer::sendMessage(sender.fd, "WHO matches by " + match + "\n", std::cout);
	for (std::vector<User *>::iterator uit = userList.begin(); uit != userList.end(); uit++) {
		if (*uit == NULL)
			continue ;
		if ((*uit)->modes['i'])
			continue;
		SockServer::sendMessage(sender.fd, WHOREPLY(sender.nick, "*", (*uit)->user, (*uit)->nick, "", (*uit)->realName) + "\n", std::cout);
	}
	SockServer::sendMessage(sender.fd, ENDOFWHO(sender.nick, arg, match) + "\n", std::cout);
}

void SockServer::who(SockServer &srv, std::vector<std::string> & args, User& user) {
	if (args.size() < 2 || (args.size() < 3 && args[1] == "0"))
	{
		SockServer::sendMessage(user.fd, "WHO matches by global\n", std::cout);
		for (fdIterator fd = srv._fds.begin(); fd != srv._fds.end(); fd++) {
			if (fd == srv._fds.begin())
				continue;
			User &dude = srv._users[fd->fd];
			if (dude.modes['i'])
				continue;
			SockServer::sendMessage(user.fd, WHOREPLY(user.nick, "*", dude.user, dude.nick, "", dude.realName) + "\n", std::cout);
		}
		SockServer::sendMessage(user.fd, ENDOFWHO(user.nick, args[1], "global") + "\n", std::cout);
	} else {
		sendMessageByUserMatch(srv, user, getUsersByNick, args[1], "nick");
		sendMessageByUserMatch(srv, user, getUsersByUsername, args[1], "username");
		sendMessageByUserMatch(srv, user, getUsersByRealName, args[1], "realname");

		bool operatorMode = false;
		if (args.size() > 1 && *(args.end() - 1) == "o")
			operatorMode = true;
		for (channelsMap::iterator chan = srv._chans.begin(); chan != srv._chans.end(); chan++) {
			if (chan->first != args[1])
				continue;
			SockServer::sendMessage(user.fd, "WHO matches by channel\n", std::cout);
			std::vector<int> userList = chan->second.getUsers();
			for (std::vector<int>::iterator userFd = userList.begin(); userFd != userList.end(); userFd++) {
				User &dude = srv._users[*userFd];
				if (dude.modes['i'])
					continue;
				if (chan->second.isOper(*userFd))
					SockServer::sendMessage(user.fd, WHOREPLY(user.nick, chan->first, dude.user, dude.nick, "@", dude.realName) + "\n", std::cout);
				else if (!operatorMode)
					SockServer::sendMessage(user.fd, WHOREPLY(user.nick, chan->first, dude.user, dude.nick, "", dude.realName) + "\n", std::cout);
			}
			SockServer::sendMessage(user.fd, ENDOFWHO(user.nick, args[1], "by channel") + "\n", std::cout);
		}
	}
}
