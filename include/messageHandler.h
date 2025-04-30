#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "connection.h"
#include "protocol.h"

#include <string>
#include <memory>

/* A low-level message handler that handles messaging to and from the server */
class MessageHandler{
    public: 
        /* Creates a message handaler */
        MessageHandler() = default;
        
        /* Transmit a code */
        void sendCode(const std::shared_ptr<Connection>& conn, const int code) const;

        /* Transmit an int value */
        void sendInt(const std::shared_ptr<Connection>& conn, const int value) const;

        /* Transmit an int parameter */
        void sendIntParameter(const std::shared_ptr<Connection>& conn, const int param) const;

        /* Transmit a string parameter */
        void sendStringParameter(const std::shared_ptr<Connection>& conn, const std::string& param) const;

        /* Receive a command code or an error code */
        int recvCode(const std::shared_ptr<Connection>& conn) const;

        /* Receive an int value */
        int recvInt(const std::shared_ptr<Connection>& conn) const;

        /* Receive an int parameter */
        int recvIntParameter(const std::shared_ptr<Connection>& conn) const;

        /* Receive a string parameter */
        std::string recvStringParameter(const std::shared_ptr<Connection>& conn) const;


    private:
        /* Sends a byte using the connection*/
        void sendByte(const std::shared_ptr<Connection>& conn, const int code) const;

        /* Receives a byte using the connection */
        int recvByte(const std::shared_ptr<Connection>& conn) const;
};

#endif