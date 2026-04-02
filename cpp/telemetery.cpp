#include "include/AC_data.h"
#include <sys/socket.h>
#include <arpa/inet.h>

class ACTelemetry {
    private:
        int sock;
        sockaddr_in server_addr;
    
    public:
        bool init(const char* ip, int port){
            sock = socket(AF_INET, SOCK_DGRAM, 0);
            if (sock < 0){
                std::cerr << "Socket creation failed" << std::endl;
                return false;
            }
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(port);
            inet_pton(AF_INET, ip, &server_addr.sin_addr);
            return true;
        }
        
        bool subscribe(){

            //Handshake with AC to start receiving telem data
            handshaker hs;
            hs.operationid = 0;
            sendto(sock, &hs, sizeof(hs), 0, (sockaddr*) &server_addr, sizeof(server_addr));

            //Request 400Hz data
            RTLap response;
            char buffer[sizeof(RTLap)];
            int len = recv(sock, buffer, sizeof(buffer), 0);
            if (len < 0){
                std::cerr << "Failed to receive handshake response" << std::endl;
                return false;
            }
            hs.operationid = 1; // Request 400Hz data
            sendto(sock, &hs, sizeof(hs), 0, (sockaddr*) &server_addr, sizeof(server_addr));

            std::cout << "Telemetry subscription successful\n\n";
            return true;
        }

        int getPacket(CarData& data){
            return recv(sock, &data, sizeof(CarData), 0);
        }
};
