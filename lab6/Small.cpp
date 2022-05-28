#include<iostream>
#include<windows.h>

using namespace std;

int main() {
	char c;
	cout << "The client is started." << endl;
	cout << "Press any char to start the work: ";
	cin >> c;

	HANDLE hNamedPipe;
	wchar_t pipeName[] = L"\\\\DESKTOP-OQ6I1Q5\\pipe\\demo_pipe";
	hNamedPipe = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING, 0, (HANDLE)NULL);
	
	if (hNamedPipe == INVALID_HANDLE_VALUE) {
		cerr << "Connection with the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish the client: ";
		cin >> c;
		return 0;
	}
	
	DWORD dwBytesRead;
	int size;
	ReadFile(hNamedPipe, &size, sizeof(size), &dwBytesRead, (LPOVERLAPPED)NULL);
	int number;
	ReadFile(hNamedPipe, &number, sizeof(number), &dwBytesRead, (LPOVERLAPPED)NULL);

	int* array = new int[size];
	for (int i = 0; i < size; i++)
	{
		if (!ReadFile(hNamedPipe, &array[i], sizeof(array[i]), &dwBytesRead, (LPOVERLAPPED)NULL)) {
			cerr << "Data reading from the named pipe failed." << endl
				<< "The last error code: " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			cout << "Press any char to finish the server: ";
			cin >> c;
			return 0;
		}
		cout << "The number " << array[i] << " was read by the client" << endl;
	}
	
	cout << endl;
	cout << "The data are read by the client." << endl;
	cout << endl;

	int counter = 0;
	cout << "My array with numbers, that > 0 and < N: ";
	for (int i = 0; i < size; i++) {
		if (array[i] > 0 && array[i] < number) {
			cout << array[i] << " ";
			counter++;
		}
	}
	cout << endl;
	int* resultArr = new int[counter];
	int index = 0;
	for (int i = 0; i < size; i++) {
		if (array[i] > 0 && array[i] < number) {
			resultArr[index] = array[i];
			index++;
		}
	}

	cout << endl;
	DWORD dwBytesWritten;
	WriteFile(hNamedPipe, &counter, sizeof(counter), &dwBytesWritten, (LPOVERLAPPED)NULL);
	for (int i = 0; i < counter; i++) {
		if (!WriteFile(hNamedPipe, &resultArr[i], sizeof(resultArr[i]), &dwBytesWritten, (LPOVERLAPPED)NULL)) {
			cerr << "Writing to the named pipe failed: " << endl
				<< "The last error code: " << GetLastError() << endl;
			cout << "Press any char to finish the client: ";
			cin >> c;
			CloseHandle(hNamedPipe);
			return 0;
		}
		cout << "The number " << resultArr[i] << " is written to the named pipe." << endl;
	}

	cout << endl;
	cout << "The data are read by the client." << endl;
	cout << endl;

	CloseHandle(hNamedPipe);
	cout << "Press any char to finish the client: ";
	cin >> c;
	return 0;
}