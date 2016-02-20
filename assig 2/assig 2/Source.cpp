#include <iostream>
#include "httpServer.h"
#include <string>
#include <vector>
#include "userCommand.h"

using namespace std;

void main() {
	//Start server
	cout << "Loading server..." << endl;
	httpServer server;
	server.startListening();
	cout << "Server Started, access the server at http://localhost" << endl;

	//TODO: Listen for errors
	

	//Process user input
	cout << endl << "Command can now be entered, type 'help' for information on the command you can use" << endl;
	string userInput = "";
	while (userInput != "close")
	{
		cout << "server: ";

		//Get input
		getline(cin, userInput);

		//Process input
		if (userInput == "stopServer")
		{//StopServer command
			cout << "Stopping server..." << endl;
			server.stopListening();
			cout << "Server successfully stopped" << endl;
		}
		else if (userInput == "startServer")
		{//StartServer command
			cout << "Starting server..." << endl;
			server.startListening();
			cout << "Server successfully started" << endl;
		}
		else if (userInput != "close")
		{//Unknown command
			cout << "The command you entered could not be found" << endl;
		}
	}

	//Stop server and close
	cout << "Stopping server" << endl;
	server.stopListening();
	cout << "Closing down, goodbye!";
}