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
        MessageHandler(Connection& conn);

        // Unsure if this is needed: 
        //    /* Deletes the messaga handaler*/
        //    ~MessageHandler();
        //
        //    /* Message handler cannot be copied to assigned*/
        //    MessageHandler(const MessageHandler&) = delete;
        //    MessageHandler& operator=(const MessageHandler&) = delete;
        //    MessageHandler& operator=(MessageHandler&&) = delete;
        
        /* Transmit a code */
        void sendCode(int code);

        /* Transmit an int value */
        void sendInt(int value);

        /* Transmit an int parameter */
        void sendIntParameter(int param);

        /* Transmit a string parameter */
        void sendStringParameter(std::string param);

        /* Receive a command code or an error code from the server */
        int recvCode();

        /* Receive an int value from the server */
        int recvInt();

        /* Receive an int parameter from the server */
        int recvIntParameter();

        /* Receive a string parameter from the server */
        std::string recvStringParameter();


    protected:
        std::shared_ptr<Connection> conn; // Pointer to the connection in use

        /* Sends a byte using the connection*/
        void sendByte(int code);

        /* Receives a byte using the connection */
        int recvByte();
};

#endif