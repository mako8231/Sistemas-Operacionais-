/*
    Sincronização: Um programa cria duas threads, uma para ler um arquivo e outro que escreve esse arquivo;
    A primeira thread espera a segunda terminar a sua escrita e então ler seu conteúdo. As duas threads 
    trabalham felizes :D, tudo funciona bem até que em um dia sombrio, a Thread de leitura termina a sua tarefa 
    antes da Thread de escrita terminar o seu trabalho. Isso pode acarretar problemas que levam a uma falha do programa.

    Esse evento é conhecido como "race condition" isso porque duas Threads estão competindo pelo seu tempo na CPU. 
    O programa sempre irá falhar caso a Thread de leitura vencer essa corrida, esse tipo de situação é consequência 
    de uma má sincronização. Outro problema é o Deadlock, onde ambas as threads são incapazes de terminarem suas funções 
    por esperaram uma terminar outra thread terminar a sua.

    Para ganhar a posse de qualquer objeto de sincronização, a thread pode usar WaitForSingleObject, WaitForSingleObjectEx, 
    WaitForMultipleObjects, or WaitForMultipleObjectsEx.      0

    WaitForSingleObject permite que uma thread entre em suspensão afim de esperar por uma posse de um dos objetos de sincronização

    DWORD WaitForSingleObject(

        HANDLE hHandle, // handle to object

        DWORD dwMilliseconds // time-out interval

    );

    O segundo parâmetro é a quantidade de tempo em milisegundos, a thread irá esperar para que o objeto de antes retorne.
    O segundo parâmetro é settado como INFINITO caso você não deseje que ele tenha esse timeout, se for definido como 0 
    a função irá tentar ganhar posse de um objeto e retornar imediatamente, mesmo se não ganhar a dita posse.

    WaitForSingleObjectEx é o mesmo que WaitForSingleObject, com a opção de alerta no módulo de I/O.

    DWORD WaitForSingleObjectEx(

        HANDLE hHandle, // handle to object

        DWORD dwMilliseconds, // time-out interval

        BOOL bAlertable // alertable option

    );    

    Se o último parâmetro for colocado como Verdade (ou 1), a função irá quando uma operação de I/O ser concluída

    WaitForMultipleObjects permite que a thread espere por múltiplos objetos de sincronização de uma vez. Pode ser definido como 
    para retorno quando qualquer um desses objetos estiverem disponíveis 

    DWORD WaitForMultipleObjects(

        DWORD nCount, // number of handles in array

        CONST HANDLE *lpHandles, // object-handle array

        BOOL bWaitAll, // wait option

        DWORD dwMilliseconds // time-out interval

    );

    Se o terceiro parâmetro for definido como VERDADEIRO, a função irá esperar **todos** os objetos ficarem disponíveis
    WaitForMultipleObjectsEx = mesma coisa só que com a opção de alerta:

    DWORD WaitForMultipleObjectsEx(

        DWORD nCount, // number of handles in array

        CONST HANDLE *lpHandles, // object-handle array

        BOOL bWaitAll, // wait option

        DWORD dwMilliseconds, // time-out interval

        BOOL bAlertable // alertable option

    );

    Mutexes 
    Criados pelo Windows Object Manager, A.K.A Mutual Exclusion, significando que apenas UMA thread pode possuir outra por vez.
    Entendível como um ticket de acesso para acessar seja lá o que o Mutex esteja protegendo.
    Uma vez que a thread termina o que faz, é dado um ticket para a próxima thread fazer o que necessário enquanto as demais esperam

    CreateMutex(
        LPSECURITY_ATTRIBUTES lpMutexAttributes,

        BOOL bInitialOwner, // initial owner

        LPCTSTR lpName // object's name
    );

    O primeiro parâmetro pode ser nulo para ter os direitos de acesso. Se o segundo parâmetro é definido como TRUE, o criador tem a posse 
    primeiro. Enquanto o terceiro parâmetro é opcional, facilita a identificação do mutex 

    Para liberar o Mutex, chama ReleaseMutex

    BOOL ReleaseMutex(

        HANDLE hMutex // handle to mutex

    );

    Apenas a thread que possui o mutex. Para destruir o mutex, chamar CloseHandle usando o mutex como parâmetro 

    Semáforos: 
    Um semáforo é util para a permitir um número limitado de Threads acessar um recurso protegido por vez. Com o mutex, apenas uma thread 
    pode possuir as permissões por vez. Com um semáforo, multiplas threads podem possuir com o tempo. Consequentemente, qualquer thread 
    pode "destruir" o semaforo. 

    HANDLE CreateSemaphore(

        LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,

        LONG lInitialCount, // initial count

        LONG lMaximumCount, // maximum count

        LPCTSTR lpName // object's name

    ); 

    Primeiro parâmetro é identico como lidar com a criação do CreateMutex, O segundo conjunto de parâmetros define o contador inicial 
    do semáforo, tipicamente, é o mesmo valor que o contador máximo. O terceiro parâmetro define o contador máximo que limita o número 
    de threads que pode possuir o semáforo. O último parâmetro é lidado da mesma forma que o CreateMutex. Após a thread ganhar a possessão
    do semáforo, o contador inicial é decrementado por um. Se o contador chegar a 0, significa que não há nenhuma thread usando o semáforo
    
    E para deletar um semaforo
    
    BOOL ReleaseSemaphore(

        HANDLE hSemaphore, // handle to semaphore

        LONG lReleaseCount, // count increment amount

        LPLONG lpPreviousCount // previous count

    );

    O segundo parâmetro define a quantidade de incrementos, por padrão, é apenas 1. 

    Secções críticas:

    (vou chamar de Critical Section) é bem similar com um Mutex, uma critical section só pode ser possuída por uma thread por vez
    Entretanto, uma secção crítica não pode ser compartilhada entre processos, uma Mutex pode. Por causa disso, a secção crítica 
    trabalha de forma mais rápida. Para criar uma Critical Section, chamar InitializeCriticalSection; para possuir uma, chamar 
    EnterCriticalSection; e para tirar a permissão: LeaveCriticalSection e para "destruir" DeleteCriticalSection

    VOID InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection); 

    VOID EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection); 

    VOID LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection); 

    VOID DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection); 

    Todas as funções que requer um ponteiro para a Critical Section, você deve declará-la desse modo:

    CRITICAL_SECTION cs; 

    Algum outra thread que foi criada pela critical section pode destruí-la

    EVENTOS 

    Um objeto de evento é útil quando você quer alertar as threads de alguma ação ocorrendo. Um botão sendo apertado por exemplo.
    Um evento é enviado para todas as threads em aguardo, chamar CreateEvent para criar um novo evento; SetEvent para sinalizar o 
    evento, ResetEvent que desliga esse sinal, quando um evento é sinalizado, todas as threads passam a aguardar a sua resposta
    PulseEvent para ligar.

    HANDLE CreateEvent(

        LPSECURITY_ATTRIBUTES lpEventAttributes,

        BOOL bManualReset, // reset type

        BOOL bInitialState, // initial state

        LPCTSTR lpName // object name

    );

    Se o segundo parâmetro for VERDADEIRO, o evento será reiniciado manualmente. Caso for falso o evento retornará o estado "desinalizado"
    após a chamada do SetEvent. Se o terceiro parâmetro for VERDADEIRO, o evento será criado e sinalizado, e o último parâmetro é um nome
    opcional do objeto.

    BOOL SetEvent(HANDLE hEvent); 

    BOOL ResetEvent(HANDLE hEvent); 

    BOOL PulseEvent(HANDLE hEvent); 

*/

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
