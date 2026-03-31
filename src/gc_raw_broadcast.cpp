#include <arpa/inet.h>
#include <chrono>
#include <csignal>
#include <iomanip>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

volatile sig_atomic_t running = 1;

void handle_sigint(int) {
    running = 0;
}

std::string now_timestamp() {
    using namespace std::chrono;
    auto t = system_clock::to_time_t(system_clock::now());
    std::tm tm;
    localtime_r(&t, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    return std::string(buf);
}

int main() {
    signal(SIGINT, handle_sigint);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    int on = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        perror("setsockopt(SO_REUSEADDR)");
        close(sock);
        return 1;
    }

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(3838);

    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        return 1;
    }

    std::cout << "Listening for UDP broadcasts on port 3838. Press Ctrl+C to stop.\n";

    const size_t BUFSZ = 65536;
    unsigned char buf[BUFSZ];

    while (running) {
        sockaddr_in from;
        socklen_t fromlen = sizeof(from);
        ssize_t n = recvfrom(sock, buf, (int)BUFSZ, 0, (sockaddr*)&from, &fromlen);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("recvfrom");
            break;
        }

        char addrstr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &from.sin_addr, addrstr, sizeof(addrstr));
        uint16_t port = ntohs(from.sin_port);

        std::cout << "=== " << now_timestamp() << " ===\n";
        std::cout << "From: " << addrstr << ":" << port << "  (" << n << " bytes)\n";

        // Hex dump (16 bytes per row) and ASCII on the right
        const int cols = 16;
        for (ssize_t i = 0; i < n; i += cols) {
            std::cout << std::setw(4) << std::setfill('0') << i << "  ";
            // hex
            for (int j = 0; j < cols; ++j) {
                if (i + j < n) {
                    std::cout << std::hex << std::setw(2) << std::setfill('0')
                                        << (int)buf[i + j] << " ";
                } else {
                    std::cout << "   ";
                }
            }
            std::cout << " ";
            // ascii
            std::cout << std::dec << std::setfill(' ');
            for (int j = 0; j < cols && i + j < n; ++j) {
                unsigned char c = buf[i + j];
                if (c >= 32 && c <= 126)
                    std::cout << (char)c;
                else
                    std::cout << '.';
            }
            std::cout << "\n";
        }
        std::cout << std::flush;
    }

    close(sock);
    std::cout << "\nStopped.\n";
    return 0;
}