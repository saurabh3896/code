#ifndef USBMUX_H
#define USBMUX_H

#include "usbmux.h"

using namespace std;
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
BinaryProtocol::BinaryProtocol(tcp::socket& s)
{
}

PlistProtocol::PlistProtocol(tcp::socket& s)
{
}

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
MuxConnection::MuxConnection(std::string const& socket_path)
    : _socket_path(socket_path)
{
    if (1) 
    {
    }
}

void MuxConnection::listen()
{
}

void MuxConnection::process(int timeout)
{
}

void MuxConnection::connect(MuxDevice device, int port)
{
}

void MuxConnection::close()
{
}

void MuxConnection::get_replay()
{
}

void MuxConnection::process_packet()
{
}

void MuxConnection::exchange()
{
}

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
USBMux::USBMux(string const& socket_path)
{
    if (socket_path.empty())
    {
    }

    _socket_path = socket_path;
    _listener = new MuxConnection(_socket_path);
}

USBMux::~USBMux()
{
    if (_listener)
    {
        delete _listener;
    }
}

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#endif
