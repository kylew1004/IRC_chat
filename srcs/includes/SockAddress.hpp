/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SockAddress.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:15:55 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:13:00 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKADDRESS_HPP
#define SOCKADDRESS_HPP

#include "ft_irc.hpp"

class SockAddress {
public:
	SockAddress();
	SockAddress(const SockAddress & src);
	SockAddress(sa_family_t family, in_addr_t address, const char *port);

	~SockAddress();

	SockAddress & operator=(const SockAddress & src);

	struct sockaddr *getAddr();
	std::string getIP();

	socklen_t *getSize();


public:

private:
	struct sockaddr_in _addr;
	size_t size;
};


#endif
