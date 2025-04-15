#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "connection.h"
#include "protocol.h"

#include <string>
#include <memory>

/* A low-level message handler that handles messaging to and from the server */
class MessageHandler{
    public: 
        /* Creates a message handaler for a specific connection */
        MessageHandler() = default;

        // Unsure if this is needed: 
        //    /* Deletes the messaga handaler*/
        //    ~MessageHandler();
        //
        //    /* Message handler cannot be copied to assigned*/
        //    MessageHandler(const MessageHandler&) = delete;
        //    MessageHandler& operator=(const MessageHandler&) = delete;
        //    MessageHandler& operator=(MessageHandler&&) = delete;
        
        /* Transmit a code */
        void sendCode(const Connection& conn, const int code) const;

        /* Transmit an int value */
        void sendInt(const Connection& conn, const int value) const;

        /* Transmit an int parameter */
        void sendIntParameter(const Connection& conn, const int param) const;

        /* Transmit a string parameter */
        void sendStringParameter(const Connection& conn, const std::string& param) const;

        /* Receive a command code or an error code from the server */
        int recvCode(const Connection& conn) const;

        /* Receive an int value from the server */
        int recvInt(const Connection& conn) const;

        /* Receive an int parameter from the server */
        int recvIntParameter(const Connection& conn) const;

        /* Receive a string parameter from the server */
        std::string recvStringParameter(const Connection& conn) const;


    protected:
        /* Sends a byte using the connection*/
        void sendByte(const Connection& conn,const int code) const;

        /* Receives a byte using the connection */
        int recvByte(const Connection& conn) const;
};

#endif