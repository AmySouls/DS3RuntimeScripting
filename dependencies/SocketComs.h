#pragma once
/*
(c) Church Guard 2021
*/

#include <WS2tcpip.h>
#include <thread>
#include <deque>

#pragma comment (lib, "ws2_32.lib")

#define MAXBUFFER 4096


class socketCommunication
{
public:
	socketCommunication(WORD port) {
		stop = false;
		active = false;
		socketPort = port;
		if (setup()) {
			//printf("READY\n");
			conLoop = std::thread(&socketCommunication::waitForConnection,this);
		}
		//else {
			//printf("FAILED\n");
		//}
	}
	~socketCommunication() {
		stop = true;
		Sleep(50);
		WSACleanup();
	}
	void printf(
		_In_z_ _Printf_format_string_    char const* const _Format,
		...) {
		char buf[MAXBUFFER];
		va_list args;
		va_start(args, _Format);
		if (active && (printBuffer.size() < (printBuffer.max_size() - 1000))) {
			if(vsprintf_s(buf, _Format, args) != -1)
				printBuffer.push_back(buf);
		}
		va_end(args);
	}
	void wprintf(
		_In_z_ _Printf_format_string_    wchar_t const* const _Format,
		...) {
		wchar_t buf[MAXBUFFER];
		va_list args;
		va_start(args, _Format);
		if (active && (printBuffer.size() < (printBuffer.max_size() - 1000))) {
			if (vswprintf_s(buf, _Format, args) != -1) {
				std::wstring ws(buf);
				std::string str(ws.begin(), ws.end());
				printBuffer.push_back(str);
			}
		}
		va_end(args);
	}
	void alert(
		_In_z_ _Printf_format_string_    char const* const _Format,
		...) {
		char buf[MAXBUFFER];
		va_list args;
		va_start(args, _Format);
		if (vsprintf_s(buf, _Format, args) != -1)
			alertBuffer.push_back(buf);
		va_end(args);
	}

	std::string getUserMessage() {
		std::string temp = "";
		if (!cmdBuffer.empty()) {
			temp = cmdBuffer[0];
			cmdBuffer.pop_front();
		}
		return temp;
	}

private:
	bool stop;
	bool active;
	WSADATA wsData;
	WORD ver;
	int wsOK;
	SOCKET listening;
	SOCKET clientsocket;
	sockaddr_in hint;
	sockaddr_in client;
	std::thread conLoop;
	std::deque<std::string>printBuffer;
	std::deque<std::string>cmdBuffer;
	std::deque<std::string>alertBuffer;
	WORD socketPort;

	bool setup(){
		ver = MAKEWORD(2, 2);
		wsOK = WSAStartup(ver, &wsData);
		if (wsOK != 0) {
			//printf("Can't Start winsock\n");
			return false;
		}

		listening = socket(AF_INET, SOCK_STREAM, 0);
		if (listening == INVALID_SOCKET) {
			//printf("INVALID_SOCKET\n");
			return false;
		}

		hint.sin_family = AF_INET;
		hint.sin_port = htons(socketPort);
		hint.sin_addr.S_un.S_addr = INADDR_ANY;

		bind(listening, (sockaddr*)&hint, sizeof(hint));

		listen(listening, SOMAXCONN);

		return true;
	}
	void waitForConnection() {
		std::string strTemp;
		int clientSize;
		char buf[MAXBUFFER];
		do {
			clientSize = sizeof(client);
			clientsocket = accept(listening, (sockaddr*)&client, &clientSize);
			active = true;
			printBuffer.push_front("||CONNECTED||\r\n");
			while (!stop) {
				//Check For Input
				if (isDataAvailable(clientsocket)) {
					ZeroMemory(buf, MAXCHAR);
					int bytesRecived = recv(clientsocket, buf, MAXBUFFER, 0);
					if (bytesRecived == SOCKET_ERROR) {
						//rut ro raggy
						break;
					}
					if (bytesRecived == 0) {
						//Client Disconnected
						break;
					}
					//echo
					strTemp = buf;
					if (strTemp != "\r\n") {
						cmdBuffer.push_back(strTemp);
						printBuffer.push_back("CMD[" + strTemp + "]\r\n");
					}
				}
				//Send Pending Prints
				while(!printBuffer.empty()) {
					ZeroMemory(buf, MAXCHAR);
					strTemp = printBuffer[0];
					printBuffer.pop_front();
					strcpy_s(buf, strTemp.c_str());
					send(clientsocket, buf, strTemp.length(), 0);
				}
				//print alerts
				while (!alertBuffer.empty()) {
					ZeroMemory(buf, MAXCHAR);
					strTemp = alertBuffer[0];
					alertBuffer.pop_front();
					strcpy_s(buf, strTemp.c_str());
					send(clientsocket, "\u001b[41m\u001b[30;1m", 13, 0);
					send(clientsocket, buf, strTemp.length(), 0);
					send(clientsocket, "\u001b[0m\u001b[0m", 9, 0);
				}

			}
			active = false;
			closesocket(clientsocket);
		} while (!stop);
	}
	//https://stackoverflow.com/questions/55384591/how-to-check-if-there-are-available-data-to-read-before-to-call-recv-in-a-blocki
	static bool isDataAvailable(int socket)
	{
		fd_set sready;
		struct timeval nowait;

		FD_ZERO(&sready);
		FD_SET((unsigned int)socket, &sready);
		memset((char*)&nowait, 0, sizeof(nowait));

		bool res = select(socket + 1, &sready, NULL, NULL, &nowait);
		if (FD_ISSET(socket, &sready))
			res = true;
		else
			res = false;
		return res;
	}
};


socketCommunication* gSock;