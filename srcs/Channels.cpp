/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:08:55 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 18:17:28 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Channels.hpp"

Channels::Channels() {
}

Channels::Channels(int creator, const std::string& name, const std::string& key) : _name(name), _creator(creator), _topic(""), _key(key) {
	std::cout << "new Channel : " + _name << std::endl;
	_members = std::map<int, bool>();
	_modes = std::map<char, bool>();
	_members[creator] = true;
	_nbop = 1;
	initModes();
	if (!key.empty()) {
		kMode('+', key);
	}
}

Channels::Channels(const Channels& src) {
	*this = src;
}

Channels::~Channels() {
	_members.clear();
}

void Channels::initModes() {
	_modes['i'] = false;
	_modes['t'] = false;
	// _modes['n'] = false;
	_modes['l'] = false;
	_modes['k'] = false;

}

std::vector<int> Channels::getUsers() {
	std::vector<int> ret;
	for (std::map<int, bool>::iterator it = _members.begin(); it != _members.end(); it++) {
		ret.push_back(it->first);
	}
	return (ret);
}

std::string Channels::getKey() {
	return (_key);
}

size_t Channels::getMaxMembers() {
	return (_maxMembers);
}

void Channels::setTopic(std::string &mess) {
	_topic = mess;
}

std::string Channels::getTopic() {
	return _topic;
}

bool Channels::isOper(int fd) {
	return (_members[fd]);
}

std::string Channels::getName() {
	return (_name);
}

void Channels::joinChannel(int fd) {
	_members[fd] = false;
}

int Channels::joinChannel(int fd, const std::string& key) {
	if (_modes['k'] == true && key != _key)
		return (1);
	if (_modes['l'] == true && _members.size() >= _maxMembers)
		return (2);
	_members[fd] = false;
	return (0);
}

int Channels::leaveChannel(int fd) {
	if (_members[fd] == true) {
		if (_nbop == 1 && _members.size() > 1) {
			std::map<int, bool>::iterator it = _members.begin();
			if (it->first == fd) {
				it++;
				it->second = true;
			}
			else
				it->second = true;
			_members.erase(fd);
			return (it->first);
		}
		_nbop--;
	}
	_members.erase(fd);
	return (-1);
}

Channels& Channels::operator=(const Channels& src) {
	_name = src._name;
	_topic = src._topic;
	_creator = src._creator;
	_members = src._members;
	_topic = src._topic;
	_key = src._key;
	_nbop = src._nbop;
	_maxMembers = src._maxMembers;
	_modes = src._modes;

	return *this;
}

int Channels::oMode(char ar, User *user) {
	if (user == NULL || _members.find(user->fd) == _members.end())
		return NO_MEMBER;
	if (ar == '+') {
		_members[user->fd] = true;
		user->channels[this] = true;
		_nbop++;
		return ALL_GOOD;
	}
	else if (ar == '-') {
		if (_nbop == 1 && isOper(user->fd)) {
			return LAST_OPER;
		}
		_members[user->fd] = false;
		user->channels[this] = false;
		_nbop--;
		return ALL_GOOD;
	}
	return WRONG_DECLARATION;
}

int Channels::lMode(char ar, int nb, const std::string&) {
	if (ar == '+') {
		_modes['l'] = true;
		_maxMembers = nb;
		return ALL_GOOD;
	}
	else if (ar == '-') {
		_modes['l'] = false;
		return ALL_GOOD;
	}
	return WRONG_DECLARATION;
}

int Channels::kMode(char ar, const std::string& key) {
	if (ar == '+') {
		_modes['k'] = true;
		_key = key;
		return ALL_GOOD;
	}
	else if (ar == '-') {
		_modes['k'] = false;
		return ALL_GOOD;
	}
	return WRONG_DECLARATION;
}


void Channels::allModes(char ar, char mode) {
	if (ar == '+')
		_modes[mode] = true;
	else if (ar == '-')
		_modes[mode] = false;
}

bool Channels::isMode(char mode) {
	return (_modes[mode]);
}

bool Channels::isEmpty() {
	return (_members.empty());
}
