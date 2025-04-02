#ifndef PROTOCOL_VIOLATION_EXCEPTION
#define PROTOCOL_VIOLATION_EXCEPTION

#include <string>

/* An exception to be cast when protocol is voilated. 
 * Violation specifications are specified in msg*/
struct ProtocolViolationException {
    ProtocolViolationException(const std::string& msg) {
        std::string m = msg;
    };
};
#endif