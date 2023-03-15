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

```c++
DWORD WaitForSingleObject(
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
Se o último parâmetro for colocado como Verdade (ou 1), a função irá quando uma operação de I/O ser concluída; 
WaitForMultipleObjects permite que a thread espere por múltiplos objetos de sincronização de uma vez. Pode ser definido como retorno quando qualquer um desses objetos estiverem disponíveis. 
```cpp
DWORD WaitForMultipleObjects(
        DWORD nCount, // number of handles in array
        CONST HANDLE *lpHandles, // object-handle array
        BOOL bWaitAll, // wait option
        DWORD dwMilliseconds // time-out interval
    );
```
Se o terceiro parâmetro for definido como VERDADEIRO, a função irá esperar **todos** os objetos ficarem disponíveis WaitForMultipleObjectsEx = mesma coisa só que com a opção de alerta:

```cpp
   DWORD WaitForMultipleObjectsEx(
        DWORD nCount, // number of handles in array
        CONST HANDLE *lpHandles, // object-handle array
        BOOL bWaitAll, // wait option
        DWORD dwMilliseconds, // time-out interval
        BOOL bAlertable // alertable option
    );
```
### Mutexes 
Criados pelo Windows Object Manager, A.K.A Mutual Exclusion, significando que apenas UMA thread pode possuir outra por vez. Entendível como um ticket de acesso para acessar seja lá o que o Mutex esteja protegendo. Uma vez que a thread termina o que faz, é dado um ticket para a próxima thread fazer o que necessário enquanto as demais esperam:

```c++
CreateMutex(
    LPSECURITY_ATTRIBUTES lpMutexAttributes,

    BOOL bInitialOwner, // initial owner

    LPCTSTR lpName // object's name
);
```
O primeiro parâmetro pode ser nulo para ter os direitos de acesso. Se o segundo parâmetro é definido como TRUE, o criador tem a posse primeiro. Enquanto o terceiro parâmetro é opcional, facilita a identificação do mutex;

Para liberar o Mutex, chama ReleaseMutex

```cpp
BOOL ReleaseMutex(
    HANDLE hMutex // handle to mutex
);
```
Apenas a thread que possui o mutex. Para destruir o mutex, chamar CloseHandle usando o mutex como parâmetro 

Semáforos: 
Um semáforo é util para a permitir um número limitado de Threads acessar um recurso protegido por vez. Com o mutex, apenas uma thread pode possuir as permissões por vez. Com um semáforo, multiplas threads podem possuir com o tempo. Consequentemente, qualquer thread pode "destruir" o semaforo. 

```cpp
    HANDLE CreateSemaphore(

        LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,

        LONG lInitialCount, // initial count

        LONG lMaximumCount, // maximum count

        LPCTSTR lpName // object's name

    ); 
```
Primeiro parâmetro é identico como lidar com a criação do CreateMutex, O segundo conjunto de parâmetros define o contador inicial do semáforo, tipicamente, é o mesmo valor que o contador máximo. O terceiro parâmetro define o contador máximo que limita o número de threads que pode possuir o semáforo. O último parâmetro é lidado da mesma forma que o CreateMutex. Após a thread ganhar a possessão do semáforo, o contador inicial é decrementado por um. Se o contador chegar a 0, significa que não há nenhuma thread usando o semáforo

E para liberar um semaforo:

```cpp
    BOOL ReleaseSemaphore(

        HANDLE hSemaphore, // handle to semaphore

        LONG lReleaseCount, // count increment amount

        LPLONG lpPreviousCount // previous count

    );
```
O segundo parâmetro define a quantidade de incrementos, por padrão, é apenas 1. 

Secções críticas:

(vou chamar de Critical Section) é bem similar com um Mutex, uma critical section só pode ser possuída por uma thread por vez. Entretanto, uma CriticalSection não pode ser compartilhada entre processos, uma Mutex pode. Por causa disso, a secção crítica trabalha de forma mais rápida. Para criar uma Critical Section, chamar InitializeCriticalSection; para possuir uma, chamar EnterCriticalSection; e para tirar a permissão: LeaveCriticalSection e para "destruir" DeleteCriticalSection.

```cpp

    VOID InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection); 

    VOID EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection); 

    VOID LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection); 

    VOID DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection); 
```

Todas as funções que requer um ponteiro para a Critical Section, você deve declará-la desse modo:
```cpp
CRITICAL_SECTION cs; 
```
Algum outra thread que foi criada pela critical section pode destruí-la.

### EVENTOS
Um objeto de evento é útil quando você quer alertar as threads de alguma ação ocorrendo. Um botão sendo apertado por exemplo.
Um evento é enviado para todas as threads em aguardo, chamar ```CreateEvent``` para criar um novo evento; ```SetEvent``` para sinalizar o evento, ```ResetEvent``` que desliga esse sinal, quando um evento é sinalizado, todas as threads passam a aguardar a sua resposta. ```PulseEvent``` para ligar.

```cpp
    HANDLE CreateEvent(

        LPSECURITY_ATTRIBUTES lpEventAttributes,

        BOOL bManualReset, // reset type

        BOOL bInitialState, // initial state

        LPCTSTR lpName // object name

    );
```

Se o segundo parâmetro for VERDADEIRO, o evento será reiniciado manualmente. Caso for falso o evento retornará o estado "desinalizado" após a chamada do SetEvent. Se o terceiro parâmetro for VERDADEIRO, o evento será criado e sinalizado, e o último parâmetro é um nome opcional do objeto.

```cpp
    BOOL SetEvent(HANDLE hEvent); 

    BOOL ResetEvent(HANDLE hEvent); 

    BOOL PulseEvent(HANDLE hEvent); 
```

Exemplo de implementação: 

```cpp
#include <windows.h>
#include <iostream>

using namespace std;

HANDLE hMutex, hWriteDone, hReadDone;

int num, state;

void Writer(){
    for(int x=10; x>=0; x--)
    {
        while(true)
        {
            if (WaitForSingleObject(hMutex, INFINITE) == WAIT_FAILED){
                cout << "Escrevendo em loop, sem mutex" << endl;
                ExitThread(0);
            }

            if (state == 0){
                ReleaseMutex(hMutex);
                WaitForSingleObject(hReadDone, INFINITE);
                continue;
            }

            break;
        }

        cout << "Escrita feita" << endl;
        num = x; 
        state = 0;
        ReleaseMutex(hMutex);
        PulseEvent(hWriteDone);
    }
}

void Reader(){
    while(true)
    {
        if(WaitForSingleObject(hMutex, INFINITE) == WAIT_FAILED)
        {
            cout << "Na leitura, sem Mutex" << endl;
            ExitThread(0);
        }
        if (state == 1)
        {
            ReleaseMutex(hMutex);
            WaitForSingleObject(hWriteDone, INFINITE);
            continue;
        }

        if (num == 0){
            cout << "Fim dps dados" << endl;
            ReleaseMutex(hMutex);
            ExitThread(0);
        } else {
            cout << "Leitura feita" << endl;
            state=1;
            ReleaseMutex(hMutex);
            PulseEvent(hReadDone);
        }
    }
}


int main(){
    HANDLE TName[2];
    DWORD ThreadID;

    state=1;

    hMutex = CreateMutex(NULL, FALSE, NULL);
    hWriteDone = CreateEvent(NULL, TRUE, FALSE, NULL);
    hReadDone = CreateEvent(NULL, TRUE, FALSE, NULL);

    TName[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Writer, NULL, 0, &ThreadID);
    TName[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Reader, NULL, 0, &ThreadID);
    
    WaitForMultipleObjects(2, TName, TRUE, INFINITE); //Barrier-alike
    CloseHandle(TName);

    return 0;
}
 
```
## Extra: O código de eventos do Moro:

```cpp
#include <process.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>

using namespace std;

typedef struct {
	int id;
	int cont;
	bool stopThread;
} PARAM;

long int contador = 0;
long int contadorunico = 0;

void testeparametro(void* parametroFuncao);


HANDLE hMutex;
HANDLE hSemafaro;

HANDLE hEvento;
HANDLE hEvento1;

CRITICAL_SECTION hSecaoCritica;

bool finaliza = false;

int main()
{
	time_t tinicial, tfinal;

	int n_threads = 2;

	vector<HANDLE> hThread;
	vector<PARAM> vetorparametros;
	PARAM adicionaVetor;


	hMutex = CreateMutex(NULL, FALSE, NULL);
	hSemafaro = CreateSemaphore(NULL, 2, 2, NULL);
	InitializeCriticalSection(&hSecaoCritica);

    hEvento = CreateEvent(NULL, TRUE, FALSE, NULL);
	hEvento1 = CreateEvent(NULL, TRUE, TRUE, NULL);

	//  tinicial = time(0);

	for (int i = 0; i < n_threads; i++)
	{
		adicionaVetor.id = i;
		adicionaVetor.cont = 0;
		adicionaVetor.stopThread = FALSE;
		vetorparametros.push_back(adicionaVetor);
	}

	for (int i = 0; i < n_threads; i++)
	{
		//hThread[i] = (HANDLE)_beginthread(contar, 1, &param[i]);
		hThread.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&testeparametro, &vetorparametros[i], CREATE_SUSPENDED, NULL));  // 0 inicia a thread em execução
		// hThread_01[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&contar, &vetorparametros[i], CREATE_SUSPENDED, NULL);
	}

	cout << " Mudando a Prioridade da Thread 0/2 !!!" << endl;
	getchar();

	SetThreadPriority(hThread[0], THREAD_PRIORITY_HIGHEST);
	SetThreadPriority(hThread[1], THREAD_PRIORITY_LOWEST);

	cout << "Prioridade Alterada !!! " << endl;

	for (int i = 0; i < n_threads; i++) {
		cout << "Prioridade da Thread " << i << " " << GetThreadPriority(hThread[i]) << endl;
	}

	cout << "Aguardando Acordar as Threads !!! " << endl;
	getchar();

	for (int i = 0; i < n_threads; i++) {
		ResumeThread(hThread[i]); // Inicia/retorna a execução de uma Thread após ser suspensa
	}

	cout << "threads Acordadas e Aguardando a Finalização!!! " << endl;

	cout << "Aguardando Gerar Evento !!! " << endl;
	getchar();
	PulseEvent(hEvento);
	getchar();
	cout << "Aguardando Gerar Evento !!! " << endl;
	PulseEvent(hEvento);
	getchar();
	cout << "Aguardando Gerar Evento !!! " << endl;
	PulseEvent(hEvento);
	getchar();
	cout << "Mudando o evento para TRUE !!! " << endl;
	SetEvent(hEvento);
	
	WaitForMultipleObjects(n_threads, hThread.data(), TRUE, INFINITE);

	cout << "O Valor Total e " << contadorunico << endl;

	/*	WaitForSingleObject(hMutex_01, INFINITE); // Inicio Seção Critica
			cout << "Aguardando para Encerrar as Threads " << endl;
		ReleaseMutex(hMutex_01);  // Fim Seção Critica


	*/

	//SetEvent(hEvento);
	//Sleep(3000);

	/*	for (int i = 0; i < n_threads; i++) {
			contadorunico = contadorunico + vetorparametros[i].cont;
		}*/
		//tfinal = time(0);
		//cout << "O tempo total de execução foi " << tfinal - tinicial << endl;
		//cout << " Valor Total e " << contadorunico << endl;

	system("pause");

	for (int i = 0; i < n_threads; i++) {
		CloseHandle(hThread[i]);
	}

	CloseHandle(hMutex);
	CloseHandle(hSemafaro);

	CloseHandle(hEvento);
	CloseHandle(hEvento1);

	DeleteCriticalSection(&hSecaoCritica);

	return 0;
}
void testeparametro(void* parametroFuncao)
{
	PARAM* parametro = (PARAM*)parametroFuncao;
	
	long int c = 1;
	long int temp = 0;

	WaitForSingleObject(hMutex, INFINITE); // Inicio Seção Critica
	  cout << "Tread ID " << parametro->id << endl;
	  cout << "Valor cont " << parametro->cont << endl;
	ReleaseMutex(hMutex);

	WaitForSingleObject(hSemafaro, INFINITE);

	cout << "Inicializando o processamento da Thread " << parametro->id << endl;

	ReleaseSemaphore(hSemafaro,1,NULL);


	while (c <= 100) {
	
		WaitForSingleObject(hEvento, INFINITE); // aguardando a geração de um sinal (evento)

		if (parametro->stopThread) {
			break;
		}

		EnterCriticalSection(&hSecaoCritica); // Inicio Seção Critica
		cout << "Thread " << parametro->id << " fez uma interacao !!!" << "Valor c " << c << endl;
		   contadorunico += 1;
		   parametro->cont++;
		LeaveCriticalSection(&hSecaoCritica);  // Fim Seção Critica

		c = c + 1;

		//WaitForSingleObject(hEvento, INFINITE);
		//cout << "Contei + 1 " << "Thread " << parametro->id << " " << contador << endl;

	}
	_endthread();
}

```

[Fonte: Win32 Multithreading and Synchronization](https://faq.programmerworld.net/programming/win32-multithreading-and-synchronization.html)
