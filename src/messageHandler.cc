#include "messageHandler.h"
#include "connection.h"
#include "protocolviolationexception.h"
#include "connectionclosedexception.h"
#include "protocol.h"

#include <memory>
#include <string>
#include <sstream>

using std::string;

MessageHandler::MessageHandler(const Connection& conn) {
    this -> conn = std::shared_ptr<const Connection>(&conn); 
}

void MessageHandler::sendByte(const int code) const{
    try {
        (*conn).write(static_cast<unsigned char>(code));
    }   catch (ConnectionClosedException e) { // This is most likely not the correct exception to look for.
        throw new ConnectionClosedException();
    }
};

void MessageHandler::sendCode(const int code) const{
    sendByte(code);
    // Somehow write to log
}

void MessageHandler::sendInt(const int value) const{
    sendByte((value >> 24) & 0xFF);
    sendByte((value >> 16) & 0xFF);
    sendByte((value >> 8) & 0xFF);
    sendByte(value & 0xFF);
    // Somehow log the intermediate steps to know what's going on?
}

void MessageHandler::sendIntParameter(const int param) const{
    sendCode(static_cast<int>(Protocol::PAR_NUM));
    sendInt(param);
    // Log here?
}

void MessageHandler::sendStringParameter(const string& param) const {
    sendCode(static_cast<int>(Protocol::PAR_STRING));
    sendInt(param.length());
    for (string::const_iterator it = param.begin(); it != param.end(); ++it) {
        sendByte(*it);
        // Log here?
    }
}

int MessageHandler::recvByte() {
    unsigned char code = (*conn).read();
    if (code == Connection::CONNECTION_CLOSED){ // Change after seeing how this is done in connection.
        throw ConnectionClosedException();
    }
    return static_cast<int>(code);
}

int MessageHandler::recvCode() {
    int code = recvByte();
    // Log here?
    return code;
}

int MessageHandler::recvInt() {
    int b1 = recvByte();
    int b2 = recvByte();
    int b3 = recvByte();
    int b4 = recvByte();
    // Log here?

    return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4; 
}

int MessageHandler::recvIntParameter() {
    int code = recvCode();
    if (code != static_cast<int>(Protocol::PAR_NUM)) {
        string msg = "Receive numeric parameter: code does not match Protocol::PAR_NUM: " + 
        std::to_string(code) + "!=" + std::to_string(static_cast<int>(Protocol::PAR_NUM));
        throw ProtocolViolationException(msg);
    }
    return recvInt();
}

string MessageHandler::recvStringParameter() {
    int code = recvCode();
    if (code != static_cast<int>(Protocol::PAR_STRING)) {
        string msg = "Receive string parameter: code does not match Protocol::PAR_STRING: " + 
        std::to_string(code) + "!=" + std::to_string(static_cast<int>(Protocol::PAR_STRING));
        throw ProtocolViolationException(msg);
    }
    int n = recvInt();
    if (n < 0) {
        throw ProtocolViolationException("Receive string parameter: Number of characters < 0");
    }
    std::stringstream result;
    for (int i = 0; i < n; i++) {
        unsigned char ch = (*conn).read();
        result << ch;
        // Log here?
    }
    return result.str();
}