# Threads 
## Criando uma Thread:

```c++
HANDLE CreateThread(
	LPSECURITY_ATTRIBUTES lpThreadAttributes, // access privileges
	DWORD dwStackSize, // initial stack size
	LPTHREAD_START_ROUTINE lpStartAddress, // thread function
	LPVOID lpParameter, // thread argument
	DWORD dwCreationFlags, // creation option
	LPDWORD lpThreadId // thread identifier
);
```  
Se a Thread for criada como ```CREATE_SUSPENDED```, é necessário resumi-la:

```DWORD ResumeThread(HANDLE hThread);```

Se deseja suspendê-la:

```DWORD SuspendThread(HANDLE hThread);```


**Threads com maior prioridade têm um tempo maior de uso no processador**

É possível alterar a prioridade de uma thread com a função ``SetThreadPriority``
```c++
BOOL SetThreadPriority(
	HANDLE hThread, // handle to the thread
	int nPriority // thread priority level
);
```
Valores de prioridade:


``THREAD_PRIORITY_LOWEST`` Dois níveis abaixo do processo

``THREAD_PRIORITY_BELOW_NORMAL`` Um nível abaixo do processo

``THREAD_PRIORITY_NORMAL `` Prioridade Normal 

``THREAD_PRIORITY_ABOVE_NORMAL`` Um nível acima do processo

``THREAD_PRIORITY_HIGHEST`` Dois níveis acima do processo

``THREAD_PRIORITY_TIME_CRITICAL`` Prioridade de 15

``THREAD_PRIORITY_IDLE`` Prioridade de 1 


  Para pegar o nível de prioridade: ``getThreadPriority``

```cpp 
int GetThreadPriority(HANDLE hThread);
```

Depois que terminar de usar a thread, é possível que você deseje terminá-la, para isso, chamar a função

``ExitThread()``, seguido de ``CloseHandle`` como uma forma adequada de fechar uma thread. Para terminar de uma vez uma thread, chamar ``TerminateThread``. Os parâmetros dessa função é:

  
```cpp
VOID ExitThread( DWORD dwExitCode);
BOOL CloseHandle(HANDLE hThread);
BOOL TerminateThread(HANDLE hThread, DWORD dwExitCode);
```

O Windows costuma chamar a função ``ExitThread`` quando uma thread termina a sua função. ``dwExitCode`` é ó código de saída que você deseja passar. O código de saída pode ser obtido depois com ``GetExitCodeThread``

  
```cpp
BOOL GetExitCodeThread(
	HANDLE hThread,
	LPDWORD lpExitCode // pointer to a DWORD to hold the exit code
);
```
Abaixo um exemplo de como utilizar threads:
```cpp
#include  <process.h>
#include  <windows.h>

#include  <iostream>
#include  <stdio.h>
#include  <stdlib.h>
#include  <time.h>
#include  <math.h>
#include  <vector>
using  namespace  std;

DWORD  ThreadID;
HANDLE  Tname;

void  funcaoExemplo()  {
	int x = 0;
	for  (x; x<10;x++)  {
		cout  <<  "Fazendo coisas"  <<  endl;
	}
}

int  main()
{

	Tname = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)funcaoExemplo,  NULL,CREATE_SUSPENDED,&ThreadID);

	if  (Tname  ==  NULL)
	{
		cout << "Não foi possível criar uma thread"  <<  endl;
		return  1;
	}
	if  (!SetThreadPriority(Tname, THREAD_PRIORITY_BELOW_NORMAL))  
	{
		cout  <<  "falhou em chamar a função SetPriority"  <<  endl;
		return  1;
	}
	if  (ResumeThread(Tname)==-1)
	{
		return  1;
	}
	WaitForSingleObject(Tname,  INFINITE); //discutido mais tarde
	CloseHandle(Tname);
	return  0;
}
```
## Sincronismo 
Sincronização: Um programa cria duas threads, uma para ler um arquivo e outro que escreve esse arquivo;

A primeira thread espera a segunda terminar a sua escrita e então ler seu conteúdo. As duas threads trabalham felizes :D, tudo funciona bem até que em um dia sombrio, a Thread de leitura termina a sua tarefa antes da Thread de escrita terminar o seu trabalho. Isso pode acarretar problemas que levam a uma falha do programa. Esse evento é conhecido como **condição de corrida**, isso porque duas Threads estão competindo pelo seu tempo na CPU.

O programa sempre irá falhar caso a Thread de leitura vencer essa corrida, esse tipo de situação é consequência de má sincronização. Outro problema é o **Deadlock**, onde ambas as threads são incapazes de terminarem suas funções por esperaram uma terminar outra thread terminar a sua.

Para ganhar a posse de qualquer objeto de sincronização, a thread pode usar ``WaitForSingleObject``,`` WaitForSingleObjectEx``, ``WaitForMultipleObjects``, ou ``WaitForMultipleObjectsEx``. 

`WaitForSingleObject` permite que uma thread entre em suspensão afim de esperar por uma posse de um dos objetos de sincronização.

```cpp
DWORD WaitForSingleObject
(
	HANDLE hHandle, // handle to object
	DWORD dwMilliseconds // time-out interval
);
```
O primeiro parâmetro é a quantidade de tempo em milissegundos, a thread irá esperar para que o objeto de antes retorne;
O segundo parâmetro é definido como **INFINITO** caso você não deseje que ele tenha esse *timeout*, se for definido como 0;
a função irá tentar ganhar posse de um objeto e retornar imediatamente, mesmo se não ganhar a dita posse.

`WaitForSingleObjectEx` é o mesmo que `WaitForSingleObject`, com a opção de alerta no módulo de E/S.
```cpp
DWORD WaitForSingleObjectEx(
	HANDLE hHandle, // handle to object
	DWORD dwMilliseconds, // time-out interval
	BOOL bAlertable // alertable option
);
```

