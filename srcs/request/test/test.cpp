#include "test.hpp"

int			main(void)
{
	try {
		Request			req(REQUEST);
		Config			conf;
		t_listen		addr;
		ConfigRequest	conf_req;

		addr.host = 0;
		addr.port = 80;

		// std::cout << req;
		// std::cout << "\n\n[[======conf======]]" << std::endl;
		conf.pull(CONF_PATH);
		// std::cout << conf << std::endl;
		conf_req = conf.get_config_for_request(addr, req.get_path(), req.get_headers().at("Host"), req.get_method(), req);
		std::cout << conf_req << std::endl;
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}