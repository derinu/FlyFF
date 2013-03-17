#include "StdAfx.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "HttpRequest.h"

using boost::asio::ip::tcp;

HttpRequest::HttpRequest(boost::asio::io_service& io_service, const std::string& server, const std::string& path, const char* data, void(*OnComplete)(char*)) : resolver_(io_service), socket_(io_service)
{
  // Form the request. We specify the "Connection: close" header so that the
  // server will close the socket after transmitting the response. This will
  // allow us to treat all data up until the EOF as the content.
  this->OnComplete = OnComplete;

  std::ostream request_stream(&request_);
  request_stream << "POST " << path << " HTTP/1.0\r\n";
  request_stream << "Host: " << server << "\r\n";
  request_stream << "Accept: */*\r\n";
  request_stream << "Accept-Encoding:\r\n";
  request_stream << "Connection: close\r\n";
  request_stream << "Content-Type: application/x-www-form-urlencoded\r\n";
  request_stream << "Content-Length: " << strlen(data) << "\r\n\r\n";
  request_stream << data;

  //::Error("Strlen: %d Data: %s", strlen(data), data);

  // Start an asynchronous resolve to translate the server and service names
  // into a list of endpoints.
  tcp::resolver::query query(server, "http");
  resolver_.async_resolve(query,
      boost::bind(&HttpRequest::handle_resolve, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::iterator));
}

void HttpRequest::New(const std::string& server, const char* path, const char* data, void(*OnComplete)(char*))
{
	boost::asio::io_service io_service;
    HttpRequest* c = new HttpRequest(io_service, server, path, data, OnComplete);
    io_service.run();
}

void HttpRequest::handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
  if (!err)
  {
    // Attempt a connection to each endpoint in the list until we
    // successfully establish a connection.
    boost::asio::async_connect(socket_, endpoint_iterator,
        boost::bind(&HttpRequest::handle_connect, this,
          boost::asio::placeholders::error));
  }
  else
  {
    std::cout << "Error: " << err.message() << "\n";
  }
}

void HttpRequest::handle_connect(const boost::system::error_code& err)
{
  if (!err)
  {
    // The connection was successful. Send the request.
    boost::asio::async_write(socket_, request_,
        boost::bind(&HttpRequest::handle_write_request, this,
          boost::asio::placeholders::error));
  }
  else
  {
    std::cout << "Error: " << err.message() << "\n";
  }
}

void HttpRequest::handle_write_request(const boost::system::error_code& err)
{
  if (!err)
  {
    // Read the response status line. The response_ streambuf will
    // automatically grow to accommodate the entire line. The growth may be
    // limited by passing a maximum size to the streambuf constructor.
    boost::asio::async_read_until(socket_, response_, "\r\n",
        boost::bind(&HttpRequest::handle_read_status_line, this,
          boost::asio::placeholders::error));
  }
  else
  {
    std::cout << "Error: " << err.message() << "\n";
  }
}

void HttpRequest::handle_read_status_line(const boost::system::error_code& err)
{
  if (!err)
  {
    // Check that response is OK.
    std::istream response_stream(&response_);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
      std::cout << "Invalid response\n";
      return;
    }
    if (status_code != 200)
    {
      std::cout << "Response returned with status code ";
      std::cout << status_code << "\n";
      return;
    }

    // Read the response headers, which are terminated by a blank line.
    boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
        boost::bind(&HttpRequest::handle_read_headers, this,
          boost::asio::placeholders::error));
  }
  else
  {
    std::cout << "Error: " << err << "\n";
  }
}

void HttpRequest::handle_read_headers(const boost::system::error_code& err)
{
  if (!err)
  {
    // Process the response headers.
    std::istream response_stream(&response_);
    std::string header;
	while (std::getline(response_stream, header) && header != "\r"){  }

	std::string body;
    response_stream >> body;

	if(OnComplete)
		this->OnComplete((char*)body.c_str());
  }
  else
  {
    std::cout << "Error: " << err << "\n";
	this->OnComplete("Err");
  }
}