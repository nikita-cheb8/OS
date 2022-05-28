#include <iostream>
#include <windows.h>
using namespace std;

wchar_t const* mes[3]{
	L"P_Exit",L"P_A",L"P_B"
};

int main() {
	HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"Semaphore");
	WaitForSingleObject(hSemaphore, INFINITE);
	cout << "Process parent is started" << endl;
	cout << "Process is active" << endl;
	HANDLE mesEv[3];
	for (int i = 0; i < 3; ++i)
		mesEv[i] = OpenEvent(EVENT_MODIFY_STATE, FALSE, mes[i]);
	while (true) {

		string line;
		cin >> line;
		line = "P_" + line;
		wstring wline(line.begin(), line.end());
		if (wline._Equal(mes[0])) {
			SetEvent(mesEv[0]);
			break;
		}
		if (wline._Equal(mes[1])) {
			SetEvent(mesEv[1]);
		}
		if (wline._Equal(mes[2])) {
			SetEvent(mesEv[2]);
		}

	}
	ReleaseSemaphore(hSemaphore, 1, NULL);
	return 0;
}