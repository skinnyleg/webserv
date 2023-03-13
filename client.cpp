#include "client.hpp"

void client::extractheader(char *buff)
{
	size_t res = 0;
	size_t pos = 0;
	std::string str(buff, this->bytes_read);
	buffer = str;

	while (pos < buffer.size())
	{
		res = buffer.find("\r\n", pos);
		if (res != std::string::npos)
		{
			if (buffer[res] == '\r' && buffer[res + 1] == '\n')
			{
				res += 2;
				if (buffer[res] == '\r' && buffer[res + 1] == '\n')
				{
					header_request = buffer.substr(0, res + 1);
					buffer.erase(0, res + 2);
					break ;
				}
				else
					pos = res;
			}
		}
		else
			pos += 1;
	}
}

void client::openfile()
{
	size_t res = 0;
	if (!file.is_open())
	{
		res = header_request.find("pdf");
		if (res != std::string::npos)
			file.open("file.pdf");
		res = header_request.find("mp4");
		if (res != std::string::npos)
			file.open("file.mp4");
		res = header_request.find("png");
		if (res != std::string::npos)
			file.open("file.png");
		res = header_request.find("text");
		if (res != std::string::npos)
			file.open("file");
	}
	if (!file.is_open())
		file.open("data");
}

void client::response()
{
	if (file.is_open())
	{
		file << buffer;
		file.flush();
	}
}

client::client()
{

}

client::client(const client &obj)
{
	*this = obj;
}

client& client::operator=(const client& obj)
{
	if (this != &obj)
	{
		this->client_socket = obj.client_socket;
		this->buffer = obj.buffer;
		this->header_request = obj.header_request;
		this->bytes_read = obj.bytes_read;
	}
	return (*this);
}

client::~client()
{

}