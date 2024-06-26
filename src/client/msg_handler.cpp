#include "./client.h"

// std::string request_msg =
//             "request: %s\r\n"
//             "id: %s\r\n"
//             "session: %s\r\n"
//             "\r\n"
//             "%s";

int Client::msg_handler(int opt, std::string request, std::string id, std::string session, std::string body)
{
    char buf[BUFSIZ];
    memset(buf, 0, BUFSIZ);
    snprintf(buf, BUFSIZ, request_msg.c_str(), request.c_str(), id.c_str(), session.c_str(), body.c_str());
    
    if(send(client_socks[opt], buf, strlen(buf), 0) < 0)
    {
        std::string dest = (opt == 0) ? "log-in" : "chat";
        printf("error: (msg_handler/send) Failed to send request to %s server.\n", dest.c_str());
        return -1;
    }
    return 0;
}