#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

#include <string>
#include <memory>

/* A low-level message handler that handles messaging to and from the server */
class MessageHandler{
    public: 
        /* Creates a message handaler for a specific connection */
        MessageHandler(const Connection& conn);

        // Unsure if this is needed: 
        //    /* Deletes the messaga handaler*/
        //    ~MessageHandler();
        //
        //    /* Message handler cannot be copied to assigned*/
        //    MessageHandler(const MessageHandler&) = delete;
        //    MessageHandler& operator=(const MessageHandler&) = delete;
        //    MessageHandler& operator=(MessageHandler&&) = delete;
        
        /* Transmit a code */
        void sendCode(const int code) const;

        /* Transmit an int value */
        void sendInt(const int value) const;

        /* Transmit an int parameter */
        void sendIntParameter(const int param) const;

        /* Transmit a string parameter */
        void sendStringParameter(const std::string& param) const;

        /* Receive a command code or an error code from the server */
        int recvCode() const;

        /* Receive an int value from the server */
        int recvInt() const;

        /* Receive an int parameter from the server */
        int recvIntParameter() const;

        /* Receive a string parameter from the server */
        std::string recvStringParameter() const;


    protected:
        std::shared_ptr<const Connection> conn; // Pointer to the connection in use

        /* Sends a byte using the connection*/
        void sendByte(const int code) const;

        /* Receives a byte using the connection */
        int recvByte() const;
};

#endif