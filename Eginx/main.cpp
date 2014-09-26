#include <iostream>
#include "Interface/Egx_IProcMutex.h"
#include "Implementation/Egx_ProcMutex.h"
#include "Utility/ScopedPtr.h"
using namespace std;

int main()
{
/*
    cout << "Hello World!" << endl;
    int a = 5;
    __sync_bool_compare_and_swap(&a,5,6);
    __asm__ ("pause");
    cout<<a<<endl;
*/
    ScopedPtr<Egx_IProcMutex> mutex;
    mutex.reset(new Egx_ProcMutex());
    int i=0;
    if(fork()==0){
        //child process
        mutex->initialize();
        for(i=0;i<10;++i){
            mutex->lock();
            sleep(3);
            cout<<getpid()<<" : "<<i<<endl;
            mutex->unlock();
            sleep(1);
        }
    }
    else{
        //parent process
        mutex->initialize();
        for(;;){
            if(mutex->tryLock()){
                cout<<"tryLock Successfully"<<endl;
                mutex->unlock();
            }
            else{
                cout<<"tryLock Failed"<<endl;
            }

            sleep(1);
        }
    }
    return 0;
}

