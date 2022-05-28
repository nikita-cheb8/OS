#include<iostream>
#include<windows.h>

using namespace std;

int main() {
	int sizeOfArray;
	cout << "Input size of array: ";
	cin >> sizeOfArray;
	int* array = new int[sizeOfArray];
	cout << "Input elements of the array: " << endl;
	for (int i = 0; i < sizeOfArray; i++) {
		cin >> array[i];
	}
	int N;
	cout << "Input N: ";
	cin >> N;
	int* arrayResult = new int[sizeOfArray + 2];
	arrayResult[0] = sizeOfArray;
	arrayResult[1] = N;

	for (int i = 2; i < sizeOfArray + 2; i++) {
		arrayResult[i] = array[i - 2];
	}

	char c;
	HANDLE hNamedPipe;
	
	hNamedPipe = CreateNamedPipe(L"\\\\.\\pipe\\demo_pipe", PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE, (LPSECURITY_ATTRIBUTES)NULL);
	if (hNamedPipe == INVALID_HANDLE_VALUE) {
		cerr << "Creation of the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish server: ";
		cin >> c;
		return 0;
	}

	cout << "The server is waiting for connection with a client." << endl;
	if (!ConnectNamedPipe(hNamedPipe, (LPOVERLAPPED)NULL)) {
		cerr << "The connection failed." << endl

			<< "The last error code: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "Press any char to finish the server: ";
		cin >> c;
		return 0;
	}

	for (int i = 0; i < sizeOfArray + 2; i++)
	{
		DWORD dwBytesWritten;
		if (!WriteFile( hNamedPipe, &arrayResult[i], sizeof(arrayResult[i]), &dwBytesWritten, (LPOVERLAPPED)NULL)) {
			cerr << "Writing to the named pipe failed: " << endl
				<< "The last error code: " << GetLastError() << endl;
			cout << "Press any char to finish the client: ";
			cin >> c;
			CloseHandle(hNamedPipe);
			return 0;
		}
		cout << "The number " << arrayResult[i] << " is written to the named pipe." << endl;
	}

	cout << endl;
	cout << "The data are written by the server." << endl;
	cout << endl;

	DWORD dwBytesRead;
	int size;
	ReadFile(hNamedPipe, &size, sizeof(size), &dwBytesRead, (LPOVERLAPPED)NULL);
	for (int i = 0; i < size; i++) {
		int nData;
		if (!ReadFile(hNamedPipe, &nData, sizeof(nData), &dwBytesRead, (LPOVERLAPPED)NULL)) {
			cerr << "Data reading from the named pipe failed." << endl
				<< "The last error code: " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			cout << "Press any char to finish the server: ";
			cin >> c;
			return 0;
		}
		cout << "The number " << nData << " was read by the server" << endl;
	}

	cout << endl;
	cout << "The data are read by the server." << endl;
	cout << endl;

	cout << "Press any char to finish the server: ";
	cin >> c;

	CloseHandle(hNamedPipe);
	return 0;
}