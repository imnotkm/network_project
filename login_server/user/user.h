#include <string>
#include <vector>

typedef struct
{
    std::string id;
    int sock_fd;
    struct sockaddr_in addr;
}User;