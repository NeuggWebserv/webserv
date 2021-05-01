#include "../ConfigServer.hpp"

/*
* *		ConfigServer 테스트 결과
**		location 에서 부모 server의 index와 server_name이 두번씩 입력되는 문제가 있음
*/

int			main(void)
{
	fileVector				file;
	file.push_back("listen");
	file.push_back("80");
	file.push_back("server_name");
	file.push_back("domain2.com");
	file.push_back("domain1.com");
	file.push_back("root");
	file.push_back("html");
	file.push_back("error_page");
	file.push_back("404");
	file.push_back("400");
	file.push_back("500");
	file.push_back("/404.html");
	file.push_back("client_body_buffer_size");
	file.push_back("1024");
	file.push_back("cgi_param");
	file.push_back("param");
	file.push_back("value");
	file.push_back("cgi_pass");
	file.push_back("pass_sample");
	file.push_back("allow_methods");
	file.push_back("GET");
	file.push_back("PUSH");
	file.push_back("PULL?");
	//		location
	file.push_back("location");
	file.push_back("var/www/");
	file.push_back("{");
	file.push_back("root");
	file.push_back("htm");
	file.push_back("error_page");
	file.push_back("304");
	file.push_back("300");
	file.push_back("300");
	file.push_back("/304.html");
	file.push_back("client_body_buffer_size");
	file.push_back("2048");
	file.push_back("cgi_param");
	file.push_back("params");
	file.push_back("values");
	file.push_back("cgi_pass");
	file.push_back("pass_sample2");
	file.push_back("allow_methods");
	file.push_back("RESET");
	file.push_back("RETURN");
	file.push_back("ADD?");
	file.push_back("index");
	file.push_back("index2.html");
	file.push_back("index2.htm");
	file.push_back("autoindex");
	file.push_back("off");
	file.push_back("alias");
	file.push_back("off");
	file.push_back("}");

	file.push_back("index");
	file.push_back("index.html");
	file.push_back("index.htm");
	file.push_back("autoindex");
	file.push_back("on");
	file.push_back("}");

	ConfigServer			server;
	unsigned int			i = 0;
	server.parse_server(i, file);

	// std::cout << server;
	return 0;
}