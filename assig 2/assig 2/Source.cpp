#include <iostream>
#include "httpServer.h"
#include <string>
#include <vector>

using namespace std;

void main() {
	//Start server
	cout << "Loading server..." << endl;
	httpServer server;
	server.startListening();
	cout << "Server Started, access the server at http://localhost" << endl;
	

	//Process user input
	cout << endl << "Commands can now be entered, type 'help' for information on the commands you can use" << endl;
	string userInput = "";
	while (userInput != "exit")
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
		else if (userInput == "help") 
		{//help command
			cout << "startServer >> No parameters, Starts the server, restarts it if it is already running" << endl
				<< "StopServer >> No parameters, Stops the server if it is running" << endl
				<< "exit >> No parameters, Stops the server if its running and closes the application" << endl;
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