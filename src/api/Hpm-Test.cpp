/***********************************************************************/
/** @file hpm_test.cpp
 * Header file for hpm_test API calls
 *
 * @author Hannes Schmelzer
 * @date   2017-11-14
 * @email  oe5hpm@oevsv.at
 * @copyright Copyright (c) 2017, Hannes Schmelzer
 * @package vzlogger
 * @license http://opensource.org/licenses/gpl-license.php GNU Public License
 **/
/*---------------------------------------------------------------------*/

/*
 * This file is part of volkzaehler.org
 *
 * volkzaehler.org is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * volkzaehler.org is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with volkszaehler.org. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Config_Options.hpp"
#include <api/Hpm-Test.hpp>

vz::api::hpm_test::hpm_test(
	Channel::Ptr ch,
	std::list<Option> options
	)
	: ApiIF(ch)
{
	int rc;
	char tmp[64];

	print(log_debug,
		  "create.",
		  channel()->name()
		  );


	for (OptionList::const_iterator it = options.cbegin();
		 it != options.cend(); it++) {
		print(log_debug,
			  "option key: %s (type %d), val %s",
			  channel()->name(),
			  it->key().c_str(),
			  (uint32_t)it->type(),
			  "nix"
			  );

		if (it->key() == "ip") {
			ip = it->operator const char *();
		} else if (it->key() == "port") {
			dport = it->operator int();
		}
	}

	if (dport == 0)
		dport = 10020;
	if (ip == NULL)
		ip = "10.44.20.10";

	peer.sin_family			= AF_INET;
	peer.sin_port			= htons(dport);
	peer.sin_addr.s_addr	= inet_addr(ip);

	hints.ai_family			= AF_INET;
	hints.ai_socktype		= SOCK_DGRAM;
	hints.ai_flags			= AI_PASSIVE; /* fill in my IP for me */
	hints.ai_protocol		= IPPROTO_UDP;
	sprintf(tmp, "%d", dport);
	getaddrinfo(NULL, tmp, &hints, &res);

	/* create socket and bind */
	sock = socket(res->ai_family,
				  res->ai_socktype,
				  res->ai_protocol);

	if(sock == -1) {
		print(log_error, "cannot create ip socket !",  channel()->name());
		return;
	} else {
		rc = bind(sock, res->ai_addr, res->ai_addrlen);
		if (rc != 0) {
			print(log_error, "annot bind socket!",  channel()->name());
			return;
		}
	}

	print(log_info,
		  "sock: %d, ip: %s, dport: %d",
		  channel()->name(),
		  sock, ip, dport
		  );
}

vz::api::hpm_test::~hpm_test()
{
	print(log_debug,
		  "destroy.",
		  channel()->name()
		  );

	close(sock);
}

void vz::api::hpm_test::send()
{
	Buffer::Ptr buf = channel()->buffer();
	int rc;
	/*
	 * we need to mark all elements as transmitted/deleted,
	 * otherwise the Channel::Buffer keeps on growing
	 */
	buf->lock();
	for (Buffer::iterator it = buf->begin(); it != buf->end(); it++) {
		print(log_debug,
			  "inst: %p, buffer %p, %03.3f @ %ld (ip: %s, port: %d)",
			  channel()->name(),
			  this,
			  channel()->buffer(),
			  it->value(),
			  it->time_ms(),
			  ip, dport
			  );

		/* transmitt data to PLC */
		payload.cnt++;
		payload.ts = it->time_ms();
		payload.val = it->value();

		rc = sendto(sock, &payload, sizeof(payload), 0,
					(struct sockaddr *)&peer,
					sizeof(struct sockaddr));
		if (rc != sizeof(payload))
			print(log_error, "cannot transmitt!",  channel()->name());
		it->mark_delete();
	}
	buf->unlock();
	buf->clean();
}

void vz::api::hpm_test::register_device()
{
	print(log_debug,
		  "register.",
		  channel()->name()
		  );
}
