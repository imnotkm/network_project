#include "./client.h"

bool Client::log_in()
{
    char buf[BUFSIZ];
    std::string id;

    bool authenticated= false;
    int attempt = 0;
    int fin = 0;

    while(attempt++ < 5)
    {
        printf("Enter your ID(%d/5): ", attempt);
        std::cin >> id;

        if(attempt == 5)
            fin = 1;
        snprintf(buf, BUFSIZ, request_form.c_str(), "login", id.c_str(), fin);

        if(send(sock_fd[0], buf, strlen(buf), 0) < 0)
        {
            printf("[client/login]: Failed to send request msg.\n");
            continue;
        }
        if(recv(sock_fd[0], buf, BUFSIZ, 0) < 0)
        {
            printf("[client/login]: Failed to receive response msg.\n");
            continue;
        }

        std::map<std::string, std::string> headers = parseHeaders(buf);
        std::string login_status = headers["authentication"];
        authenticated = (login_status == "success") ? true : false;

        if(authenticated)
        {
            printf("Hello %s.\n", id.c_str());
            this->id = id;

            return true;
        }

        printf( "ID already taken. Please choose a different ID.\n"
                "Continue to log in? [Y/N]: ");
        char ans[10];
        scanf("%s", ans);
        if(strcasecmp(ans, "y"))
            break;
    }
    snprintf(buf, BUFSIZ, request_form.c_str(), "login", id.c_str(), 1);
    send(sock_fd[0], buf, strlen(buf), 0);
    printf("Too many login attempts. Terminating program.");

    return false;
}