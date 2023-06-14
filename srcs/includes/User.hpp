/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:16:16 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:12:58 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <map>
#include "Channels.hpp"

class User {
public:
	User();
	User(int gFd, const std::string & gIp, bool s_pass = 0);
	User(const User& src);

	~User();

	User &operator=(const User &src);

	void enterChannel(Channels* chan, bool op);
	int leaveChannel(Channels* chan);
	void leaveAllChannels();

	void initModes();

public:
	std::string user;
	std::string realName;
	std::string nick;
	bool pass;
	std::string ip;
	std::string buffer;
	int fd;
	std::map<Channels*, bool> channels;
	std::map<char, bool> modes;
};


#endif
