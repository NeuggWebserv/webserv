#ifndef TEST_HPP
#define TEST_HPP

#include "Request.hpp"
#include "Config.hpp"
#include "ConfigRequest.hpp"
#include <iostream>

#define CONF_PATH "./test.conf"

#define REQUEST "GET http://goddaehee.tistory.com/168 HTTP/1.1\r\n\
Host: goddaehee.tistory.com\r\n\
Connection: keep-alive\r\n\
Upgrade-Insecure-Requests: 1\r\n\
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n\
Referer: http://goddaehee.tistory.com/\r\n\
Accept-Encoding: gzip, deflate\r\n\
Accept-Charsets: utf-8\r\n\
Allow: GET POST\r\n\
Accept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7\r\n\
Cookie: menuEnabled=1;\r\n\r\n\
hello this is test\r\n\
this is next line\r\n\
\r\n\
another line\r\n\
"
/* 안 되는 것
** Accept-Charset
** 
*/
// #define REQUEST "GET http://goddaehee.tistory.com/168 HTTP/1.1\r\nHost: goddaehee.tistory.com\r\n"

#endif