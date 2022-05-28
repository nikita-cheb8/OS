#include <iostream>
#include <windows.h>
#include <algorithm>
using namespace std;

CRITICAL_SECTION cs;
int counterX;
HANDLE hOutEvent, hContinueEvent;

struct dataArray {
	int size;
	int* array;
	int X;
};

DWORD WINAPI work(void* obj) {
	EnterCriticalSection(&cs);
	dataArray* dAr = (dataArray*)obj;
	int* mas = dAr->array;

	cout << "Thread work is started" << endl;
	int time;
	cout << "Input time interval to sleep: ";
	cin >> time;

	int swapCounter = 0;
	for (int i = 0; i < dAr->size; i++) {
		if (mas[i] == dAr->X) {
			swap(mas[i], mas[swapCounter]);
			swapCounter += 1;
		}
	}
	
	for (int i = 0; i < dAr->size; i++) {
		cout << mas[i] << " ";
		Sleep(time);
	}
	cout << endl << "Thread work is finished" << endl;
	LeaveCriticalSection(&cs);
	return 0;
}

DWORD WINAPI CountElement(void* obj) {
	EnterCriticalSection(&cs);
	dataArray* dAr = (dataArray*)obj;
	int* mas = dAr->array;
	cout << "Thread CountElement is started" << endl;

	int sum = 0;
	for (int i = 0; i < dAr->size; i++) {
		sum += mas[i];
	}
	cout << "Sum of array is " << sum << endl;
	LeaveCriticalSection(&cs);
	counterX = 0;
	for (int i = 0; i < dAr->size; i++) {
		if (mas[i] == dAr->X) {
			counterX++;
		}
	}

	SetEvent(hOutEvent);
	WaitForSingleObject(hContinueEvent, INFINITE);

	cout << endl << "Thread CountElement is finished" << endl;
	return 0;
}

int main() {
	int size;
	cout << "Input size of array: ";
	cin >> size;
	int* array = new int[size];
	for (int i = 0; i < size; i++) {
		cin >> array[i];
	}

	cout << "The size of array is " << size << endl;
	cout << "Array: " << endl;
	for (int i = 0; i < size; i++) {
		cout << array[i] << " ";
	}

	dataArray* d = new dataArray();
	d->size = size;
	d->array = array;

	InitializeCriticalSection(&cs);
	hOutEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hContinueEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE hThread;
	DWORD IDThread;
	DWORD dwCount;
	hThread = CreateThread(NULL, 0, work, (void*)d, CREATE_SUSPENDED, &IDThread);

	HANDLE hThreadCount;
	DWORD IDThreadCount;
	DWORD dwCountCount;
	hThreadCount = CreateThread(NULL, 0, CountElement, (void*)d, CREATE_SUSPENDED, &IDThreadCount);

	int numbertoFind;
	cout << endl << "Input number to find: ";
	cin >> numbertoFind;
	d->X = numbertoFind;

	dwCount = ResumeThread(hThread);
	dwCountCount = ResumeThread(hThreadCount);

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	DeleteCriticalSection(&cs);

	WaitForSingleObject(hOutEvent, INFINITE);
	cout << "Number " << d->X << " repeats " << counterX << " times" << endl;
	SetEvent(hContinueEvent);

	WaitForSingleObject(hThreadCount, INFINITE);
	CloseHandle(hThreadCount);

	delete[] array;

	return 0;
}