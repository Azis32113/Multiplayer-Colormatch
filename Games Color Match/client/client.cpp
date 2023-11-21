#include <iostream>
#include <cstring>
#include <winsock2.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialize Winsock" << endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Error creating client socket" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // Use the same port as the server
    serverAddress.sin_addr.s_addr = inet_addr("192.168.1.9"); // Replace with the server's local IP address

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        cerr << "Error connecting to the server" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Connected to the server." << endl;

    system("CLS");

    cout << "Color Match Game" << endl;
    cout << "Rules:" << endl;
    cout << "1. Pemain memilih warna antara (Blue or Red)" << endl;
    cout << "2. Server (Player 1) menang jika warna pemain berbeda" << endl;
    cout << "3. Client (Player 2) menang jika warna pemain sama" << endl << endl;
    cout << "YOU ARE PLAYER 2 || YOU ARE PLAYER 2" << endl;

    char message[1024];
    while (true) {
        cout << endl << "Enter your choice (Blue or Red): ";
        cin.getline(message, sizeof(message));

        // Send the choice to the server
        send(clientSocket, message, strlen(message), 0);

        // Receive the server's choice from the server
        memset(message, 0, sizeof(message));
        int bytesRead = recv(clientSocket, message, sizeof(message), 0);

        if (bytesRead == SOCKET_ERROR) {
            cerr << "Error receiving data" << endl;
            break;
        } else if (bytesRead == 0) {
            cout << "Server disconnected" << endl;
            break;
        }

        cout << "Server's choice: " << message << endl;

        // Receive and display the game result
        memset(message, 0, sizeof(message));
        bytesRead = recv(clientSocket, message, sizeof(message), 0);

        if (bytesRead == SOCKET_ERROR) {
            cerr << "Error receiving data" << endl;
            break;
        } else if (bytesRead == 0) {
            cout << "Server disconnected" << endl;
            break;
        }

        cout << "Game result: " << message << endl;
    }

    // Close the client socket and cleanup Winsock
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}