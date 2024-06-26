/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:09:05 by klew              #+#    #+#             */
/*   Updated: 2023/06/15 12:26:14 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_irc.hpp"

static int startSocket() {
	int serverFd;
	if ((serverFd = socket(IPV4, TCP, IP_PROTOCOL)) <= 0) {
		std::cerr << std::strerror(errno) << std::endl;
		throw std::runtime_error("socket creation failed");
	}

	int opt = 1;
	if (setsockopt(serverFd, SOL_SOCKET, OPTS, &opt, sizeof(opt))) { // 커널에서 소켓을 바인딩하고 있기 때문에 재사용 가능하게 해야함.
		std::cerr << std::strerror(errno) << std::endl;
		throw std::runtime_error("socket was not properly created");
	}

	return serverFd;
}

static void bindSocket(int serverFd, const char *port) {
	struct sockaddr_in address = {};
	static int addrLen = sizeof(address);
	address.sin_family = IPV4;
	address.sin_addr.s_addr = ANY_CLIENT;
	address.sin_port = htons(std::strtol(port, NULL, 0));

	if (bind(serverFd, (struct sockaddr *)&address,addrLen) < 0) {
		std::cerr << std::strerror(errno) << std::endl;
		throw std::runtime_error("socket bind failed");
	}
}

int socketConf(const char *port) {
	int serverFd = startSocket();
	bindSocket(serverFd, port);
	if (listen(serverFd, MAX_CLIENTS)) {
		std::cerr << std::strerror(errno) << std::endl;
		throw std::runtime_error("socket listen failed");
	}
	std::cout << "Socket up" << std::endl;
	return serverFd;
}
