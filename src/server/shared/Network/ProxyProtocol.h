/* Part of Project SkyFire. See LICENSE.md for copyright information. */
#ifndef SKYFIRE_PROXY_PROTOCOL_H
#define SKYFIRE_PROXY_PROTOCOL_H
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace Skyfire::Net
{
    inline bool ParseProxyHeader(std::string const& line, boost::asio::ip::tcp::endpoint& source)
    {
        if (line.size() > 108 || line.size() < 2 || line.substr(line.size()-2) != "\r\n") return false;
        std::istringstream in(line.substr(0, line.size()-2));
        std::string marker, family, src, dst, sport, dport, extra;
        if (!(in >> marker >> family >> src >> dst >> sport >> dport) || in >> extra ||
            marker != "PROXY" || (family != "TCP4" && family != "TCP6")) return false;
        auto port = [](std::string const& value) -> unsigned
        {
            if (value.empty() || value.size() > 5 || value.find_first_not_of("0123456789") != std::string::npos) return 0;
            unsigned n = unsigned(std::stoul(value)); return n <= 65535 ? n : 0;
        };
        boost::system::error_code ec1, ec2;
        auto a = boost::asio::ip::make_address(src, ec1), b = boost::asio::ip::make_address(dst, ec2);
        unsigned const p = port(sport);
        if (ec1 || ec2 || !p || !port(dport) || a.is_v4() != (family == "TCP4") || b.is_v4() != a.is_v4() ||
            a.is_unspecified() || a.is_multicast()) return false;
        source = {a, static_cast<unsigned short>(p)};
        return true;
    }
    inline bool ParseProxyPeers(std::string const& text, std::vector<boost::asio::ip::address>& peers)
    {
        std::istringstream in(text); std::string word;
        while (in >> word)
        {
            boost::system::error_code ec;
            auto address = boost::asio::ip::make_address(word, ec);
            if (ec || address.is_unspecified() || address.is_multicast() || peers.size() >= 16) return false;
            peers.push_back(address);
        }
        return !peers.empty();
    }
    // Call only after validating the transport peer against an explicit allowlist.
    // Read exactly the header: never consume/replay bytes from the client protocol.
    class ProxyHeaderReader : public std::enable_shared_from_this<ProxyHeaderReader>
    {
    public:
        using Socket = boost::asio::ip::tcp::socket;
        using Completion = std::function<void(bool, boost::asio::ip::tcp::endpoint)>;
        static void Start(std::shared_ptr<Socket> socket, Completion done)
        {
            auto reader = std::shared_ptr<ProxyHeaderReader>(new ProxyHeaderReader(std::move(socket), std::move(done)));
            reader->_timer.expires_after(std::chrono::seconds(5));
            reader->_timer.async_wait([reader](boost::system::error_code ec) { if (!ec) reader->Finish(false); });
            reader->Read();
        }
    private:
        ProxyHeaderReader(std::shared_ptr<Socket> socket, Completion done)
            : _socket(std::move(socket)), _timer(_socket->get_executor()), _done(std::move(done)) { }
        void Read()
        {
            auto self = shared_from_this();
            boost::asio::async_read(*_socket, boost::asio::buffer(&_byte, 1), [self](boost::system::error_code ec, std::size_t)
            {
                if (self->_finished) return;
                if (ec) { self->Finish(false); return; }
                self->_line += self->_byte;
                if (self->_byte == '\n') self->Finish(ParseProxyHeader(self->_line, self->_source));
                else if (self->_line.size() >= 108) self->Finish(false);
                else self->Read();
            });
        }
        void Finish(bool success)
        {
            if (_finished) return;
            _finished = true; _timer.cancel();
            if (!success) { boost::system::error_code ec; _socket->close(ec); }
            _done(success, _source);
        }
        std::shared_ptr<Socket> _socket;
        boost::asio::steady_timer _timer;
        Completion _done;
        boost::asio::ip::tcp::endpoint _source;
        std::string _line;
        char _byte = 0;
        bool _finished = false;
    };
}
#endif
