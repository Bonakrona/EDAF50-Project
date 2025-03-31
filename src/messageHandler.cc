#include "messageHandler.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

#include <memory>
#include <string>

using std::string;

MessageHandler::MessageHandler(Connection& conn) {
    this -> conn = std::shared_ptr<Connection>(&conn); 
}

void MessageHandler::sendByte(int code) {
    try {
        (*conn).write(static_cast<unsigned char>(code));
    }   catch (ConnectionClosedException e) { // This is most likely not the correct exception to look for.
        throw new ConnectionClosedException();
    }
};

void MessageHandler::sendCode(int code) {
    sendByte(code);
    // Somehow write to log
}

void MessageHandler::sendInt(int value) {
    sendByte((value >> 24) & 0xFF);
    sendByte((value >> 16) & 0xFF);
    sendByte((value >> 8) & 0xFF);
    sendByte(value & 0xFF);
    // Somehow log the intermediate steps to know what's going on?
}

void MessageHandler::sendIntParameter(int param) {
    sendCode(static_cast<int>(Protocol::PAR_NUM));
    sendInt(param);
    // Log here?
}

void MessageHandler::sendStringParameter(string param) {
    sendCode(static_cast<int>(Protocol::PAR_STRING));
    sendInt(param.length());
    for (string::iterator it = param.begin(); it != param.end(); ++it) {
        sendByte(*it);
        // Log here?
    }
}

int MessageHandler::recvByte() {
    unsigned char code = (*conn).read();
    if (code == Connection::CONNECTION_CLOSED){ // Change after seeing how this is done in connection.
        throw new ConnectionClosedException();
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