/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:11:55 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:13:08 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/SockServer.hpp"

void parseChan(std::map<std::string, std::string>& mChans, std::vector<std::string>& args) {
	std::string::iterator it = args[1].begin();
	std::string::iterator ite = it;
	std::string::iterator itk;
	std::string::iterator itek;
	if (args.size() == 3) {
		itk = args[2].begin();
		itek = itk;
	}

	while (it != args[1].end()) {
		std::string chan;
		while (ite != args[1].end() && *ite != ',') {
			ite++;
		}
		chan.assign(it, ite);
		mChans[chan] = "";
		if (args.size() == 3) {
			std::string key;
			while (itek != args[2].end() && *itek != ',')
				itek++;
			key.assign(itk, itek);
			if (key != "x")
				mChans[chan] = key;
			if (*itek == ',')
				itek++;
			itk = itek;
		}
		if (*ite == ',')
			ite++;
		it = ite;
	}
}

void SockServer::join(SockServer &srv, std::vector<std::string>& args, User& user) {
	if (args[0] != "JOIN" || args.size() <= 1) {
		sendMessage(user.fd, std::string(ERR_NEEDMOREPARAMS(user.nick, args[0])) + "\n", std::cerr);
		return;
	}
	std::map<std::string, std::string> mChans;

	parseChan(mChans, args);

	for (std::map<std::string, std::string>::iterator it = mChans.begin(); it != mChans.end() ; it++) {
		channelsMap::iterator itc = srv._chans.find(it->first);
		std::string mess = "Hello " + user.nick + "! Welcome in the channel " + it->first + "!\n";
		if (itc == srv._chans.end()) {
			Channels newC(user.fd, it->first, it->second);
			srv._chans[it->first] = newC;
			user.enterChannel(&srv._chans[it->first], true);
			transmitToChannelFromServ(srv._chans[it->first], JOIN(user.nick, user.user, it->first) + "\n");
			if (!srv._chans[it->first].getTopic().empty()) {
				sendMessage(user.fd, TOPIC(user.nick, srv._chans[it->first].getName()) + srv._chans[it->first].getTopic() + "\n", std::cout);
			}
			transmitToChannelFromServ(srv._chans[it->first], CHANOPER(srv._chans[it->first].getName(), user.nick));
			std::vector<std::string> a;
			a.push_back("NAMES"); a.push_back(srv._chans[it->first].getName());
			names(srv, a, user);
		}
		else {
			if (itc->second.isMode('i') == true) {
				sendMessage(user.fd, std::string(ERR_INVITEONLYCHAN(user.nick, itc->first)) + "\n", std::cerr);
				return ;
			}
			int ret = itc->second.joinChannel(user.fd, it->second);
			if (!ret) {
				user.enterChannel(&itc->second, false);
				transmitToChannelFromServ(itc->second, JOIN(user.nick, user.user, itc->first) + "\n");
				if (!itc->second.getTopic().empty()) {
					sendMessage(user.fd, TOPIC(user.nick, itc->second.getName()) + itc->second.getTopic() + "\n", std::cout);
				}
				std::vector<std::string> a;
				a.push_back("NAMES"); a.push_back(itc->second.getName());
				names(srv, a, user);
			}
			else {
				if (ret == 1) {
					sendMessage(user.fd, std::string(ERR_BADCHANNELKEY(user.nick, itc->first)) + "\n", std::cerr);
					return;
				}
				if (ret == 2) {
					sendMessage(user.fd, std::string(ERR_CHANNELISFULL(user.nick, itc->first)) + "\n", std::cerr);
					return;
				}
			}
		}
	}
}
