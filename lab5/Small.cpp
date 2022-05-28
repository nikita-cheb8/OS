#include<iostream>
#include<windows.h>

using namespace std;

int main(int argc, char* argv[]) {
	HANDLE hWritePipe = (HANDLE)atoi(argv[1]), hReadPipe = (HANDLE)atoi(argv[2]);

	HANDLE hSmallEnableRead, hServerEnableRead;
	hSmallEnableRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"SmallRead");
	hServerEnableRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"ServerRead");

	WaitForSingleObject(hSmallEnableRead, INFINITE);

	int size, number;
	DWORD dwBytesRead, dwBytesWrite;
	ReadFile(hReadPipe, &size, sizeof(int), &dwBytesRead, NULL);
	ReadFile(hReadPipe, &number, sizeof(int), &dwBytesRead, NULL);
	int* arr = new int[size];
	for (int i = 0; i < size; i++)
		ReadFile(hReadPipe, &arr[i], sizeof(int), &dwBytesRead, NULL);

	int counter = 0;
	cout << "My array with numbers, that > 0 and < N: ";
	for (int i = 0; i < size; i++) {
		if (arr[i] > 0 && arr[i] < number) {
			cout << arr[i] << " ";
			counter++;
		}
	}
	cout << endl;
	int* resultArr = new int[counter];
	int index = 0;
	for (int i = 0; i < size; i++) {
		if (arr[i] > 0 && arr[i] < number) {
			resultArr[index] = arr[i];
			index++;
		}
	}

	WriteFile(hWritePipe, &counter, sizeof(int), &dwBytesWrite, NULL);
	for (int i = 0; i < counter; ++i)
		WriteFile(hWritePipe, &resultArr[i], sizeof(int), &dwBytesWrite, NULL);
	SetEvent(hServerEnableRead);

	delete[]arr;
	delete[]resultArr;
	system("pause");
	return 0;
}