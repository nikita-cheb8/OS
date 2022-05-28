#include <windows.h>
#include <iostream>

using namespace std;

class MonitorQueue {
private:
	unsigned int* arr;
	int size;
	int pushPos;
	int popPos;
public:
	MonitorQueue(int size) {
		this->size = size;
		arr = new unsigned int[size];
		pushPos = 0;
		popPos = 0;
	}

	~MonitorQueue() {
		delete[] arr;
	}

	void AddHead(unsigned int nElement) {
		arr[pushPos++] = nElement;
		if (pushPos == size) {
			pushPos = 0;
		}
	}

	int RemoveTail() {
		int element = arr[popPos++];
		if (popPos == size) {
			popPos = 0;
		}
		return element;
	}

};