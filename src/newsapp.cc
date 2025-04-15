
#include "newsapp.h"
// #include "database.h"
#include "connection.h"

#include <iostream>
#include <string>
// NewsApp::NewsApp(Server* serv, Database* db) {
//     server = serv;
//     database = db;
// }

// NewsApp::NewsApp(MessageHandler mh, Database* db) {
//     mh = mh;
//     database = db;
// }

NewsApp::NewsApp(std::unique_ptr<Database> db, MessageHandler mh) : database(std::move(db)), messageHandler(mh) {
}

int readNumber(const std::shared_ptr<Connection>& conn)
{
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}



void NewsApp::process_request(std::shared_ptr<Connection>& conn) {
    // MOCK IMPL: read and return integer from client.
    int nbr = readNumber(conn);
    std::string s = std::to_string(nbr);
    for (char c : s) {
        conn->write(c);
    }
    conn->write('$');
}