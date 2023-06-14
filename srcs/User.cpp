/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:08:38 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:12:44 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/User.hpp"

User::User() : user(), realName(), nick(), pass(0), ip(), buffer(), fd(0), channels() {
	initModes();
}

User::User(int gFd, const std::string &gIp, bool s_pass)
	: user(), realName(), nick(), pass(s_pass), ip(gIp), buffer(), fd(gFd), channels(std::map<Channels*, bool>()) {
	initModes();
}

User::User(const User &src) {
	*this = src;
}

User::~User() {
	modes.clear();
	channels.clear();
}

User &User::operator=(const User &src) {
	if (this != &src) {
		user = src.user;
		nick = src.nick;
		pass = src.pass;
		ip = src.ip;
		buffer = src.buffer;
		fd = src.fd;
		realName = src.realName;
		modes = src.modes;
		channels = src.channels;
	}
	return *this;
}

void User::initModes() {
	modes['i'] = false;
	modes['o'] = false;
}

void User::enterChannel(Channels *chan, bool op) {
	channels[chan] = op;
}

void User::leaveAllChannels() {
	channels.clear();
}

int User::leaveChannel(Channels *chan) {
	channels.erase(chan);
	return(chan->leaveChannel(fd));
}
