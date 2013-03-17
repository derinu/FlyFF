#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

class HttpRequest
{
public:
	HttpRequest(boost::asio::io_service& io_service, const std::string& server, const std::string& path, const char* data, void(*OnComplete)(char*));
	static void New(const std::string& server, const char* path, const char* data, void(*OnComplete)(char*) = NULL);

private:
	void handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
	void handle_connect(const boost::system::error_code& err);
	void handle_write_request(const boost::system::error_code& err);
	void handle_read_status_line(const boost::system::error_code& err);
	void handle_read_headers(const boost::system::error_code& err);

	tcp::resolver resolver_;
	tcp::socket socket_;
	boost::asio::streambuf request_;
	boost::asio::streambuf response_;

	void(*OnComplete)(char*);
};
