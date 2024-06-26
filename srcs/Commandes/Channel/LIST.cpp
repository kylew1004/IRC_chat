/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LIST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:11:07 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:13:13 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/SockServer.hpp"

static std::vector<std::string> parseMess(std::string msg)
{
	std::vector<std::string> args;
	size_t pos;
	std::string token;
	while ((pos = msg.find(',')) != std::string::npos)
	{
		token = msg.substr(0, pos);
		args.push_back(token);
		msg.erase(0, pos + 1);
	}
	args.push_back(msg);
	return args;
}

void SockServer::list(SockServer &srv, std::vector<std::string> & args, User& user)
{
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++) {
		if (it->empty()) {
			args.erase(it);
			break;
		}
	}
	if (args.size() < 2)
	{
		for (channelsMap::iterator it = srv._chans.begin(); it != srv._chans.end(); it++) {
			std::vector<int> user_list = it->second.getUsers();
			int i = 0;
			for (std::vector<int>::iterator fd = user_list.begin(); fd != user_list.end(); fd++) {
				if (srv._users[*fd].modes['i'])
					continue;
				i++;
			}
			std::ostringstream ss(""); ss << i;
			sendMessage(user.fd, LIST(user.nick, it->first, ss.str()) + it->second.getTopic() + "\n", std::cout);
		}
	}
	else
	{
		std::vector<std::string> chan_list = parseMess(args[1]);
		for (std::vector<std::string>::iterator it = chan_list.begin(); it != chan_list.end(); it++) {
			std::map<std::basic_string<char>, Channels >::iterator chan = srv._chans.find(*it);
			if (chan == srv._chans.end())
				return ;
			std::vector<int> user_list = chan->second.getUsers();
			int i = 0;
			for (std::vector<int>::iterator fd = user_list.begin(); fd != user_list.end(); fd++) {
				if (srv._users[*fd].modes['i'])
					continue;
				i++;
			}
			std::ostringstream ss(""); ss << i;
			sendMessage(user.fd, LIST(user.nick, chan->first, ss.str()) + chan->second.getTopic() + "\n", std::cout);
		}
	}
	sendMessage(user.fd, LISTEND(user.nick) + "\n", std::cout);
}
