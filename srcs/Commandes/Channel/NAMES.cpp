/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NAMES.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:10:58 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:13:16 by klew             ###   ########.fr       */
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

void SockServer::names(SockServer &srv, std::vector<std::string> & args, User& user)
{
    if (args.size() < 2)
    {
        for (channelsMap::iterator it = srv._chans.begin(); it != srv._chans.end(); it++)
        {
            std::vector<int> user_list = it->second.getUsers();
			std::string list;
            for (std::vector<int>::iterator it2 = user_list.begin(); it2 != user_list.end(); it2++) {
				if (srv._users[*it2].modes['i'])
					continue;
				if (it->second.isOper(*it2))
					list += "@";
				list += srv._users[*it2].nick + " ";
			}
			sendMessage(user.fd, NAMES(user.nick, it->first) + list + "\n", std::cout);
			sendMessage(user.fd, ENDOFNAMES(user.nick, it->first) + "\n", std::cout);
        }
		std::string list;
		for (userMap::iterator it = srv._users.begin(); it != srv._users.end(); it++)
			if (it->second.channels.empty() && !it->second.modes['i'])
				list += it->second.nick + " ";
		sendMessage(user.fd, NAMES(user.nick, "*") + list + "\n", std::cout);
		sendMessage(user.fd, ENDOFNAMES(user.nick, "*") + "\n", std::cout);
    }
    else
    {
        std::vector<std::string> chan_list = parseMess(args[1]);
        for (std::vector<std::string>::iterator i = chan_list.begin(); i != chan_list.end(); i++)
        {
            std::map<std::basic_string<char>, Channels >::iterator chan = srv._chans.find(*i);
            if (chan == srv._chans.end())
		        return ;
            std::vector<int> user_list = chan->second.getUsers();
			std::string list;
            for (std::vector<int>::iterator it2 = user_list.begin(); it2 != user_list.end(); it2++) {
				if (srv._users[*it2].modes['i'])
					continue;
				if (chan->second.isOper(*it2))
					list += "@";
				list += srv._users[*it2].nick + " ";
			}
			sendMessage(user.fd, NAMES(user.nick, *i) + list + "\n", std::cout);
			sendMessage(user.fd, ENDOFNAMES(user.nick, *i) + "\n", std::cout);
        }
    }
}
