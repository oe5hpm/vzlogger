/***********************************************************************/
/** @file hpm_test.hpp
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

#ifndef _hpm_test_hpp_
#define _hpm_test_hpp_

#include <ApiIF.hpp>
#include <Options.hpp>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>

struct amis_mtr_r {
	uint32_t	cnt;
	int64_t		ts;
	float		val;
};

namespace vz {
	namespace api {
		class hpm_test : public ApiIF {
			public:
				typedef vz::shared_ptr<ApiIF> Ptr;

				hpm_test(Channel::Ptr ch, std::list<Option> options);
				~hpm_test();
				void send();
				void register_device();

			private:
				const char			*ip;
				int					dport;
				struct addrinfo		hints, *res;
				struct sockaddr_in	peer;
				int					sock;

				struct amis_mtr_r	payload;
		};
	}
}
#endif
