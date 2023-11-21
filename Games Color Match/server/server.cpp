#include <iostream>
#include <cstring>
#include <cstdlib>
#include <winsock2.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

int ChoiceToInt(const string& choice) {
    if (choice == "Red" || choice == "red") {
        return 0;
    } 
    
    else if (choice == "Blue" || choice == "blue") {
        return 1;
    } 
    
    else {
        // Handle invalid choices
        return -1; // You can choose a different value to represent an invalid choice
    }
}

int DetermineWinner(int clientChoiceInt, int serverChoiceInt) {
    if (clientChoiceInt == serverChoiceInt) {
        return 1; // server wins
    } 
    
    else {
        return -1; // client wins
    }
}

void PrintWinner(int result, SOCKET clientSocket)
{
    if (result == 0)
    {
        send(clientSocket, "It's a tie!", 12, 0);
        cout << "Game result: It's a tie!" << endl;
    }
    else if (result == 1)
    {
        send(clientSocket, "Player 2 Win!", 14, 0);    // Client Win
        cout << "Game result: Player 1 Lose!" << endl; // Server Lose
    }
    else
    {
        send(clientSocket, "Player 2 Lose!", 15, 0);  // Client Lose
        cout << "Game result: Player 1 Win!" << endl; // Server Win
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialize Winsock" << endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error creating server socket" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // Choose a port, e.g., 12345
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        cerr << "Error binding socket" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        cerr << "Error listening on socket" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Server listening on port 12345..." << endl;

    sockaddr_in clientAddress;
    int clientAddrSize = sizeof(clientAddress);
    SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Error accepting connection" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Client connected" << endl;

    char buffer[1024];
    int bytesRead;

    system("CLS");

    cout << "Color Match Game" << endl;
    cout << "Rules:" << endl;
    cout << "1. Pemain memilih warna antara (Blue or Red)" << endl;
    cout << "2. Server (Player 1) menang jika warna pemain berbeda" << endl;
    cout << "3. Client (Player 2) menang jika warna pemain sama" << endl << endl;
    cout << "YOU ARE PLAYER 1 || YOU ARE PLAYER 1" << endl;

    while (true) {
        string serverChoice; // 0 for Red, 1 for Blue
        
        cout << endl << "Enter your choice (Blue or Red): ";
        cin >> serverChoice;

        memset(buffer, 0, sizeof(buffer));
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead == SOCKET_ERROR) {
            cerr << "Error receiving data" << endl;
            break;
        } else if (bytesRead == 0) {
            cout << "Client disconnected" << endl;
            break;
        }

        string clientChoice(buffer);

        int clientChoiceInt = ChoiceToInt(clientChoice);

        if (clientChoiceInt == -1) {
            cerr << "Invalid choice received" << endl;
        } else {
            srand(static_cast<unsigned>(time(nullptr)));
            
            cout << "Client's choice : " << clientChoice << endl;

            // Send server choice to the client
            send(clientSocket, serverChoice.c_str(), serverChoice.length(), 0);

            int result = DetermineWinner(clientChoiceInt, ChoiceToInt(serverChoice));
            PrintWinner(result, clientSocket);
        }
    }

    // Close the sockets and cleanup Winsock
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
