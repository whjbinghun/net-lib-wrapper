#include <iostream>
#include <signal.h>
#include <execinfo.h>
#include <sys/resource.h>
#include "ms-epoll-tcp.h"
#include "test-client.h"

using namespace std;

static void WidebrightSegvHandler(int signum)
{
    void *array[10];
    size_t size;
    char **strings;
    size_t i = 0;

    signal(signum, SIG_DFL);//SIG_DFL代表执行系统默认操作，其实对于大多数信号的系统默认动作时终止该进程

    size = backtrace (array, 10);
    strings = (char **)backtrace_symbols (array, size);

    MS_LOGER_FATAL( "signum = %d, stack info:", signum );

    //fprintf(stderr, "widebright received SIGSEGV! Stack trace:\n");
    for (i = 0; i < size; i++) {
        //fprintf(stderr, "%d %s \n",i,strings[i]);
        MS_LOGER_FATAL( "%s", strings[i] );
    }

    free (strings);
    exit(1);
}

void signal_WidebrightSegvHandler()
{
    signal( SIGSEGV, WidebrightSegvHandler );
    signal( SIGABRT, WidebrightSegvHandler );
    signal( SIGBUS, WidebrightSegvHandler );
    signal( SIGFPE, WidebrightSegvHandler );
    signal( SIGILL, WidebrightSegvHandler );
    signal( SIGXCPU, WidebrightSegvHandler );
    signal( SIGXFSZ, WidebrightSegvHandler );
    signal( SIGTERM, WidebrightSegvHandler );
}

void signal_sigign()
{
    //signal信号函数，第一个参数表示需要处理的信号值（SIGHUP），第二个参数为处理函数或者是一个表示，这里，SIG_IGN表示忽略SIGHUP那个注册的信号。
    signal( SIGHUP, SIG_IGN );
    signal( SIGINT, SIG_IGN );
    signal( SIGQUIT, SIG_IGN );
    signal( SIGPIPE, SIG_IGN );
}

int main()
{  
    signal_sigign();
    signal_WidebrightSegvHandler();
    std::string str_ip = "192.168.8.114";//"192.168.8.200";
    unsigned short us_port = 8899;
    Test_Client test_client( str_ip, us_port );
    test_client.Init();

    while( true ){
        sleep( 1 );
    }

    return 0;
}

