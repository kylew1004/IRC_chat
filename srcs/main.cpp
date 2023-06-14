/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:08:14 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:12:33 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/SockServer.hpp"
#include "includes/SockAddress.hpp"

void catchSig(int sig) {
	std::cout << "Caught signal " << sig << " Exiting..." << std::endl;
}

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << getCurrentTime() << std::endl;
	SockServer server = SockServer(argv[1]);
	server.password = argv[2];
	signal(SIGTERM, catchSig);
	signal(SIGINT, catchSig);
	signal(SIGKILL, catchSig);
	signal(SIGHUP, catchSig);

	while (true) {
		int acNum = poll(server.getFds(), server.getSize(), -1);

		if (acNum == -1) {
			if (errno == EINTR)
				break;
			std::cerr << "Error on poll" << std::endl;
			break;
		}

		if (server.begin()->revents)
			acNum -= server.check();

		for (fdIterator it = server.begin(); it != server.end(); it++) {
			if (acNum <= 0)
				break;
			if (!it->revents || it->fd == server.getFd())
				continue;

			if (it->revents == DATA_IN) {
				acNum--;
				bool err;
				std::string msg = server.readMessage(it->fd, err);
				std::cout << "\tRECV -> " << msg << std::endl;
				if (!msg.empty())
					server.messageRouter(it->fd, msg);
				if (!err)
					continue;
			}

			fdVector::iterator nxt = it - 1;
			server.deleteClient(it);
			it = nxt;
		}
	}
	std::cout << "Bye!" << std::endl;
}
