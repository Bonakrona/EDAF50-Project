#include "messageHandler.h"
#include "connection.h"
#include "protocolviolationexception.h"
#include "protocol.h"

#include <memory>
#include <string>
#include <sstream>

using std::string;

void MessageHandler::sendByte(const Connection& conn,const int code) const{
    conn.write(static_cast<unsigned char>(code));
};

void MessageHandler::sendCode(const Connection& conn,const int code) const{
    sendByte(conn,code);
    // Somehow write to log
}

void MessageHandler::sendInt(const Connection& conn,const int value) const{
    sendByte(conn,(value >> 24) & 0xFF);
    sendByte(conn,(value >> 16) & 0xFF);
    sendByte(conn,(value >> 8) & 0xFF);
    sendByte(conn,value & 0xFF);
    // Somehow log the intermediate steps to know what's going on?
}

void MessageHandler::sendIntParameter(const Connection& conn,const int param) const{
    sendCode(conn,static_cast<int>(Protocol::PAR_NUM));
    sendInt(conn,param);
    // Log here?
}

void MessageHandler::sendStringParameter(const Connection& conn,const string& param) const {
    sendCode(conn,static_cast<int>(Protocol::PAR_STRING));
    sendInt(conn,param.length());
    for (string::const_iterator it = param.begin(); it != param.end(); ++it) {
        sendByte(conn,*it);
        // Log here?
    }
}

int MessageHandler::recvByte(const Connection& conn) const {
    unsigned char code = conn.read();
    return static_cast<int>(code);
}

int MessageHandler::recvCode(const Connection& conn) const {
    int code = recvByte(conn);
    // Log here?
    return code;
}

int MessageHandler::recvInt(const Connection& conn) const {
    int b1 = recvByte(conn);
    int b2 = recvByte(conn);
    int b3 = recvByte(conn);
    int b4 = recvByte(conn);
    // Log here?

    return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4; 
}

int MessageHandler::recvIntParameter(const Connection& conn) const {
    int code = recvCode(conn);
    if (code != static_cast<int>(Protocol::PAR_NUM)) {
        string msg = "Receive numeric parameter: code does not match Protocol::PAR_NUM: " + 
        std::to_string(code) + "!=" + std::to_string(static_cast<int>(Protocol::PAR_NUM));
        throw ProtocolViolationException(msg);
    }
    return recvInt(conn);
}

string MessageHandler::recvStringParameter(const Connection& conn) const {
    int code = recvCode(conn);
    if (code != static_cast<int>(Protocol::PAR_STRING)) {
        string msg = "Receive string parameter: code does not match Protocol::PAR_STRING: " + 
        std::to_string(code) + "!=" + std::to_string(static_cast<int>(Protocol::PAR_STRING));
        throw ProtocolViolationException(msg);
    }
    int n = recvInt(conn);
    if (n < 0) {
        throw ProtocolViolationException("Receive string parameter: Number of characters < 0");
    }
    std::stringstream result;
    for (int i = 0; i < n; i++) {
        unsigned char ch = conn.read();
        result << ch;
        // Log here?
    }
    return result.str();
}