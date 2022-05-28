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

	HANDLE hSmallEnableRead, hServerEnableRead;
	hSmallEnableRead = CreateEvent(NULL, FALSE, FALSE, L"SmallRead");
	hServerEnableRead = CreateEvent(NULL, FALSE, FALSE, L"ServerRead");


	HANDLE hWritePipe, hReadPipe;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);

	STARTUPINFO stp;
	PROCESS_INFORMATION pi;
	ZeroMemory(&stp, sizeof(STARTUPINFO));
	stp.cb = sizeof(STARTUPINFO);
	wchar_t commandLine[80];
	wsprintf(commandLine, L"Small.exe %d %d", (int)hWritePipe, (int)hReadPipe);
	CreateProcess(NULL, commandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &stp, &pi);

	DWORD dwBytesWritten, dwBytesRead;
	WriteFile(hWritePipe, &sizeOfArray, sizeof(int), &dwBytesWritten, NULL);
	WriteFile(hWritePipe, &N, sizeof(int), &dwBytesWritten, NULL);
	for (int i = 0; i < sizeOfArray; ++i)
		WriteFile(hWritePipe, &array[i], sizeof(int), &dwBytesWritten, NULL);
	SetEvent(hSmallEnableRead);

	WaitForSingleObject(hServerEnableRead, INFINITE);
	int sizeOfNewArray;
	ReadFile(hReadPipe, &sizeOfNewArray, sizeof(int), &dwBytesRead, NULL);
	int* resultArr = new int[sizeOfNewArray];
	for (int i = 0; i < sizeOfNewArray; ++i)
		ReadFile(hReadPipe, &resultArr[i], sizeof(int), &dwBytesRead, NULL);

	cout << "Number of elements is " << sizeOfNewArray << endl;
	cout << "Result : ";
	for (int i = 0; i < sizeOfNewArray; ++i)
		cout << resultArr[i] << " ";

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(hServerEnableRead);
	CloseHandle(hSmallEnableRead);
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}