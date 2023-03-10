//Padrao do C++ :DDDDD
#include <iostream>
#include <vector>

//Threads 
#include <windows.h>
#include <process.h>

using namespace std;

HANDLE hMutex;
HANDLE hSemaforo;
CRITICAL_SECTION hSessaoCritica;

int main(){
    
    hMutex = CreateMutex(NULL, FALSE, NULL);
    hSemaforo = CreateSemaphore(NULL, FALSE, 8, NULL);
    InitializeCriticalSection(&hSessaoCritica);

    return 0;
}

