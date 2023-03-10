// Aula Threads 2022.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//criação de Thread recebe os seguintes parâmetros 
/**
HANDLE CreateThread(
    LPSECURITY_ATTRIBUTES lpThreadAttributes, // access privileges

   DWORD dwStackSize,                        // initial stack size

   LPTHREAD_START_ROUTINE lpStartAddress,    // thread function

   LPVOID lpParameter,                       // thread argument

   DWORD dwCreationFlags,                    // creation option

   LPDWORD lpThreadId                        // thread identifier
);**/


//Se a Thread for criada como CREATE_SUSPENDED, é necessário resumí-la 
//DWORD ResumeThread(HANDLE hThread); 
//Se deseja suspendê-la:
//DWORD SuspendThread(HANDLE hThread); 

//Threads com maior prioridade têm um tempo maior de uso no processador
//É possível alterar a prioridade de uma thread com a função SetThreadPriority
/**BOOL SetThreadPriority(

    HANDLE hThread, // handle to the thread

    int nPriority // thread priority level

);**/

//Valores de prioridade:
/**
THREAD_PRIORITY_LOWEST 	                    Two levels below process
THREAD_PRIORITY_BELOW_NORMAL 	            One level below process
THREAD_PRIORITY_NORMAL 	                    Normal priority
THREAD_PRIORITY_ABOVE_NORMAL 	            One level above process
THREAD_PRIORITY_HIGHEST 	                Two levels above process
THREAD_PRIORITY_TIME_CRITICAL 	            Priority of 15
THREAD_PRIORITY_IDLE 	                    Priority of 1
**/

//Para pegar o nível de prioridade: getThreadPriority
//int GetThreadPriority(HANDLE hThread);
/*
    Depois que terminar de usar a thread, é possível que você deseje terminá-la, para isso, chamar a função
    ExitThread(), seguido de CloseHandle como uma forma adequada de fechar uma thread. Para terminar de uma vez uma thread,
    chamar TerminateThread. Os parâmetros dessa função é:

    VOID ExitThread( DWORD dwExitCode);

    BOOL CloseHandle(HANDLE hThread);

    BOOL TerminateThread(HANDLE hThread, DWORD dwExitCode);
*/

/*
    O Windows costuma chamar a função ExitThread quando uma thread termina a sua função. dwExitCode é ó código de saída 
    que você deseja passar. O código de saída pode ser obtido depois com GetExitCodeThread

BOOL GetExitCodeThread(

        HANDLE hThread,

        LPDWORD lpExitCode // pointer to a DWORD to hold the exit code

);

Abaixo um exemplo de como utilizar threads 
*/

#include <process.h>
#include <windows.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>

using namespace std;

DWORD ThreadID;
HANDLE Tname;

/**typedef struct {
    int id;
    int cont;
} PARAM;**/

void funcaoExemplo() {
    int x = 0;
    for (x; x < 10; x++) {
        cout << "Fazendo coisas" << endl;

    }

}

int main()
{
    Tname = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)funcaoExemplo, NULL, CREATE_SUSPENDED, &ThreadID);
    if (Tname == NULL) 
    {
        cout << "Não foi possível criar uma thread" << endl;
        return 1;
    }

    if (!SetThreadPriority(Tname, THREAD_PRIORITY_BELOW_NORMAL)) {
        cout << "falhou em chamar a função SetPriority" << endl;
        return 1;
    }

    if (ResumeThread(Tname) == -1) {
        return 1;
    }

    WaitForSingleObject(Tname, INFINITE); //discutido mais tarde 
    CloseHandle(Tname);

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
