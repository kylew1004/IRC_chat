/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ERROR.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klew <klew@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:10:36 by klew              #+#    #+#             */
/*   Updated: 2023/06/13 17:13:30 by klew             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/SockServer.hpp"

void SockServer::error(const std::string& reason, User& user) {
	sendMessage(user.fd, ERROR_QUIT(user.nick, user.ip, reason) + "\n", std::cout);
}
