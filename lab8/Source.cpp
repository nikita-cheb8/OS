#include "MonitorQueue.cpp"
#include <math.h>
#include <time.h>
using namespace std;

CRITICAL_SECTION criticalSection;
HANDLE empty1;
HANDLE full;

struct TransmitedParametrs {
	MonitorQueue* queue;
	int number;
	int threadNumber;

	TransmitedParametrs(MonitorQueue*& q, int num, int thNum) {
		queue = q;
		number = num;
		threadNumber = thNum;
	}
};

struct ThreadData {
	HANDLE handle;
	DWORD id;
};

DWORD WINAPI producer(LPVOID v) {
	TransmitedParametrs* parametrs = (TransmitedParametrs*)v;
	
	for (int i = 0; i < parametrs->number; i++) {


		WaitForSingleObject(empty1, INFINITE);

		EnterCriticalSection(&criticalSection);

		unsigned int num = rand() % 250;
		parametrs->queue->AddHead(num);
		cout << "Producer: " << parametrs->threadNumber << " create number: " << num << endl;

		LeaveCriticalSection(&criticalSection);
		Sleep(30);
		while (!ReleaseSemaphore(full, 1, NULL));
	}
	return 0;
}

DWORD WINAPI consumer(LPVOID v) {
	TransmitedParametrs* parametrs = (TransmitedParametrs*)v;
	for (int i = 0; i < parametrs->number; i++) {

		WaitForSingleObject(full, INFINITE);

		EnterCriticalSection(&criticalSection);

		unsigned int num = parametrs->queue->RemoveTail();
		cout << "Consumer: " << parametrs->threadNumber << " used element: " << num << endl;

		LeaveCriticalSection(&criticalSection);
		Sleep(30);
		while (!ReleaseSemaphore(empty1, 1, NULL));
	}
	return 0;
}

int main() {
	srand(time(0));
	int size;
	cout << "Input size of the warehouse: ";
	cin >> size;

	empty1 = CreateSemaphore(NULL, size, size, L"Empty");
	full = CreateSemaphore(NULL, 0, size, L"Full");
	InitializeCriticalSection(&criticalSection);

	MonitorQueue* queue = new MonitorQueue(size);
	cout << "Input number of producers: ";
	int producerThreadCount;
	cin >> producerThreadCount;

	int* producerProductCountList = new int[producerThreadCount];
	for (int i = 0; i < producerThreadCount; i++) {
		cout << "Input num of products for " << i + 1 << " producer: ";
		int producerProductCount;
		cin >> producerProductCount;

		producerProductCountList[i] = producerProductCount;
	}

	cout << "Input number of consumers: ";
	int consumerThreadCount;
	cin >> consumerThreadCount;

	int* consumerProductCountList = new int[consumerThreadCount];
	for (int i = 0; i < consumerThreadCount; i++) {
		cout << "Input num of products for " << i + 1 << " producer: ";
		int consumerProductCount;
		cin >> consumerProductCount;

		consumerProductCountList[i] = consumerProductCount;
	}

	int threadsCount = producerThreadCount + consumerThreadCount;
	HANDLE arr[20];
	int inc = 0;

	ThreadData** producerThreadData = new ThreadData * [producerThreadCount];

	for (int i = 0; i < producerThreadCount; i++) {
		producerThreadData[i] = new ThreadData();
		TransmitedParametrs* tp = new TransmitedParametrs(queue, producerProductCountList[i], i + 1);
		producerThreadData[i]->handle = CreateThread(NULL, 0, producer, (void*&)(tp), 0, &producerThreadData[i]->id);

		arr[inc] = producerThreadData[i]->handle;
		inc++;
	}

	ThreadData** consumerThreadData = new ThreadData * [consumerThreadCount];

	for (int i = 0; i < consumerThreadCount; i++) {
		consumerThreadData[i] = new ThreadData();
		TransmitedParametrs* tp = new TransmitedParametrs(queue, consumerProductCountList[i], i + 1);
		consumerThreadData[i]->handle = CreateThread(NULL, 0, consumer, (void*)(tp), 0, &consumerThreadData[i]->id);

		arr[inc] = consumerThreadData[i]->handle;
		inc++;
	}

	WaitForMultipleObjects(producerThreadCount, arr, TRUE, INFINITE);

	return 0;
}