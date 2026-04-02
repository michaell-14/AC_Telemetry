#include "include/AC_data.h"
#include <sys/socket.h>
#include <arpa/inet.h>

class ACTelemetry {
    private:
        int sock;
        sockaddr_in server_addr;
    
    public:
        bool init(const char* ip, int port){
            sock = socket(AF_INET, SOCK_DGRAM, 0); //create udp socket
            if (sock < 0){
                std::cerr << "Socket creation failed" << std::endl;
                return false;
            }
            server_addr.sin_family = AF_INET; //set addres family to IPv4
            server_addr.sin_port = htons(port); //network byte order (big-endian)
            inet_pton(AF_INET, ip, &server_addr.sin_addr); //convert IP to binary
            return true;
        }
        
        bool subscribe(){

            //Handshake with AC to start receiving telem data
            handshaker hs;
            hs.operationid = 0;
            sendto(sock, &hs, sizeof(hs), 0, (sockaddr*) &server_addr, sizeof(server_addr));

            //Request 400Hz data
            RTLap response;
            char buffer[sizeof(RTLap)]; //buffer wating for track and car info
            int len = recv(sock, buffer, sizeof(buffer), 0); //may need a timeout to avoid blocking
            if (len < 0){
                std::cerr << "Failed to receive handshake response" << std::endl;
                return false;
            }
            hs.operationid = 1; // Start streaming 400Hz data
            sendto(sock, &hs, sizeof(hs), 0, (sockaddr*) &server_addr, sizeof(server_addr)); //offload to board

            std::cout << "Telemetry subscription successful\n\n";
            return true;
        }

        int getPacket(CarData& data){
            //Receive telemetry data packet; may need a timeout to avoid blocking here too
            return recv(sock, &data, sizeof(CarData), 0);
        }
};
