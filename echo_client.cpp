#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

void print_error_and_exit(const string& message) {
    cerr << message << endl;
    exit(EXIT_FAILURE);
}

int main() {
    string server_ip;
    int server_port;
    string message;
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Prompt user for server details
    cout << "Enter the server IP address):";
    cin >> server_ip;
    cout << "Enter the server port number:";
    cin >> server_port;
    cin.ignore();  // To ignore the newline left by cin

    // Prompt for message to send
    cout << "Enter the message to send to the server:";
    getline(cin, message);

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        print_error_and_exit("Error creating socket!");
    }

    // Set up the server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        print_error_and_exit("Connection to the server failed!");
    }

    cout << "Connected to the server at " << server_ip << ":" << server_port << endl;

    // Send the message to the server
    if (send(sockfd, message.c_str(), message.length(), 0) < 0) {
        print_error_and_exit("Error sending data to server!");
    }
    cout << "Message sent to server: " << message << endl;

    // Receive the echoed message from the server
    ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
        print_error_and_exit("Error receiving data from server!");
    }
    else if (bytes_received == 0) {
        cout << "Server closed the connection." << endl;
    } else {
        buffer[bytes_received] = '\0';  // Null-terminate the received string
        cout << "Received echoed message from server: " << buffer << endl;
    }

    // Close the socket
    close(sockfd);
    cout << "Connection closed." << endl;

    return 0;
}
