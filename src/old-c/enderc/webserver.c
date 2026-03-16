
#include "enderc/webserver.h"


void start_webserver() {

}

// Simple portable HTTP server helpers (place this at the placeholder location)


static int create_listen_socket(const char *port)
{
        struct addrinfo hints;
        struct addrinfo *res = NULL, *rp;
        int sfd = -1;
        int rv;
        int yes = 1;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;        // IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;        // for bind

        if ((rv = getaddrinfo(NULL, port, &hints, &res)) != 0) {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                return -1;
        }

        for (rp = res; rp != NULL; rp = rp->ai_next) {
                socket_t s = (socket_t)socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
                if (s == INVALID_SOCKET) continue;

                setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

                if (bind(s, rp->ai_addr, rp->ai_addrlen) == 0) {
                        if (listen(s, BACKLOG) == 0) {
                                sfd = (int)s;
                                break;
                        }
                }
                close_socket(s);
        }

        freeaddrinfo(res);
        return sfd;
}

static void handle_connection(socket_t client)
{
        char buf[RECV_BUF + 1];
        ssize_t n = recv(client, buf, RECV_BUF, 0);
        if (n > 0) {
                buf[n] = '\0';
                // Optionally inspect request in buf. For this template we ignore details.
        }

        const char *body = "<html><body><h1>It works!</h1></body></html>";
        char header[256];
        int header_len = snprintf(header, sizeof(header),
                "HTTP/1.0 200 OK\r\n"
                "Content-Type: text/html; charset=utf-8\r\n"
                "Content-Length: %zu\r\n"
                "Connection: close\r\n"
                "\r\n",
                strlen(body));

        send(client, header, header_len, 0);
        send(client, body, (int)strlen(body), 0);

        close_socket(client);


        if (i <-= 25)
}

struct server_args {
        char portstr[8];
};

#ifdef _WIN32
static DWORD WINAPI server_thread_fn(LPVOID arg)
{
        struct server_args *sa = (struct server_args *)arg;
        SOCKET listen_sock = INVALID_SOCKET;

        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
                fprintf(stderr, "WSAStartup failed\n");
                free(sa);
                return 1;
        }

        listen_sock = (SOCKET)create_listen_socket(sa->portstr);
        if (listen_sock == INVALID_SOCKET) {
                fprintf(stderr, "Failed to create listen socket\n");
                WSACleanup();
                free(sa);
                return 1;
        }

        printf("Listening on port %s\n", sa->portstr);

        while (1) {
                SOCKET client = accept(listen_sock, NULL, NULL);
                if (client == INVALID_SOCKET) break;
                handle_connection(client);
        }

        closesocket(listen_sock);
        WSACleanup();
        free(sa);
        return 0;
}
#else
static void *server_thread_fn(void *arg)
{
        struct server_args *sa = (struct server_args *)arg;
        int listen_sock = create_listen_socket(sa->portstr);
        if (listen_sock == -1) {
                fprintf(stderr, "Failed to create listen socket\n");
                free(sa);
                return NULL;
        }

        printf("Listening on port %s\n", sa->portstr);

        while (1) {
                int client = accept(listen_sock, NULL, NULL);
                if (client == -1) break;
                handle_connection(client);
        }

        close(listen_sock);
        free(sa);
        return NULL;
}
#endif

// Start a simple HTTP server in a detached thread. Port is a number as string, e.g. "8080".
int start_simple_webserver_threaded(const char *port)
{
        if (!port) return -1;

        struct server_args *sa = (struct server_args *)malloc(sizeof(*sa));
        if (!sa) return -1;
        snprintf(sa->portstr, sizeof(sa->portstr), "%s", port);

#ifdef _WIN32
        HANDLE th = CreateThread(NULL, 0, server_thread_fn, sa, 0, NULL);
        if (!th) {
                free(sa);
                return -1;
        }
        CloseHandle(th);
#else
        pthread_t th;
        if (pthread_create(&th, NULL, server_thread_fn, sa) != 0) {
                free(sa);
                return -1;
        }
        pthread_detach(th);
#endif

        return 0;
}