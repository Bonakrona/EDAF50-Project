#include "messageHandler.h"
#include "connection.h"
#include "connectionclosedexception.h"

#include <memory>

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