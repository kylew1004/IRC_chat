/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:08:31 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:12:36 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/SockServer.hpp"

struct pollfd generatePollFd(int fd, short events) {
	struct pollfd ret = {};
	ret.fd = fd;
	ret.events = events;
	return ret;
}

std::vector<std::string> parseMessage(std::string msg) {
	std::vector<std::string> args;
	size_t pos;
	std::string token;
	while ((pos = msg.find(' ')) != std::string::npos) {	// token은 명령어라 생각
		token = msg.substr(0, pos);
		args.push_back(token);
		msg.erase(0, pos + 1);
	}
	args.push_back(msg);
	return args;
}

void SockServer::welcome(SockServer& srv, const std::vector<std::string>&, User& user) {
	if (user.nick.empty() || user.user.empty())
		return;
	sendMessage(user.fd, RPL_WELCOME(user.nick, user.user) + "\n", std::cout);
	std::vector<std::string> args;
	args.push_back("NAMES");
	names(srv, args, user);
}

std::string getCurrentTime() {
	std::time_t t = std::time(NULL);
	std::tm* now = std::localtime(&t);
	std::ostringstream ss("");

	ss << now->tm_mday << '-'
			<< (now->tm_mon + 1) << '-'
			<< (now->tm_year + 1900) << " - "
			<< now->tm_hour << ':'
			<< now->tm_min << ':'
			<< now->tm_sec;
	return ss.str();
}

bool cInSet(const char c, const std::string &set) {
	for (std::string::const_iterator it = set.begin(); it != set.end(); it++) {
		if (*it == c)
			return true;
	}
	return false;
}
