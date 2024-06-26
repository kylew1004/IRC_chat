/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SockAddress.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:08:24 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:12:46 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/SockAddress.hpp"

SockAddress::SockAddress() : _addr(), size(sizeof(_addr)) {}

SockAddress::SockAddress(const SockAddress &src): _addr(), size(sizeof(_addr)) {
	*this = src;
}

SockAddress::SockAddress(sa_family_t family, in_addr_t address, const char *port)
	: _addr(), size(sizeof(_addr)) {
	_addr.sin_family = family;
	_addr.sin_addr.s_addr = address;
	_addr.sin_port = htons(std::strtol(port, NULL, 0));
}

SockAddress::~SockAddress() {}

SockAddress &SockAddress::operator=(const SockAddress &src) {
	if (this != &src)
		_addr = src._addr;
	return *this;
}

struct sockaddr *SockAddress::getAddr() {
	return reinterpret_cast<sockaddr *>(&_addr);
}

socklen_t *SockAddress::getSize() {
	return reinterpret_cast<socklen_t *>(&size);
}

std::string SockAddress::getIP() {
	return inet_ntoa(_addr.sin_addr);
}
