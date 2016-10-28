#include "xthread.h"

/* Standard C Header Files. */

#include <stdlib.h>
#include <assert.h>
#include <errno.h>

/* Platform Header Files. */

#ifndef EQX_WIN32
#include <pthread.h>
#include <signal.h>
#else
#include <windows.h>
#include <process.h>
#endif

/* Thread Routine Invoker. */

struct routine_invoker_params {
    xthread_routine_t routine;
    void *routine_param;
};

#ifdef EQX_WIN32
static unsigned __stdcall thread_routine_invoker(void *arg)
#else
static void * thread_routine_invoker(void *arg)
#endif
{
    struct routine_invoker_params *params = arg;
    intptr_t result;
    
    assert(params != NULL);
    
    result = params->routine(params->routine_param);
    free(params);
    
#ifdef EQX_WIN32
    return (unsigned) result;
#else
    return (void *) result;
#endif
}

/* Cross-platform Thread APIs. */

struct xthread_mutex {
#ifdef EQX_WIN32
    CRITICAL_SECTION mutex;
#else
    pthread_mutex_t mutex;
#endif
};

struct xthread_storage {
#ifdef EQX_WIN32
    DWORD key;
#else
    pthread_key_t key;
#endif
};

int xthread_create(xthread_t *thread, xthread_routine_t routine, void *param)
{
    struct routine_invoker_params *invoker_param;
    int status;
    
    assert(thread != NULL);
    assert(routine != NULL);
    
    /* Setup Thread Invoker Parameters. */
    invoker_param = calloc(1, sizeof(*invoker_param));
    
    if (invoker_param == NULL)
        return EQX_STATUS_NOMEM;
    
    invoker_param->routine = routine;
    invoker_param->routine_param = param;
    
    /* Create Thread. */
#ifdef EQX_WIN32
    *thread = (xthread_t) _beginthreadex(NULL, 0, thread_routine_invoker,
        invoker_param, 0, NULL);
    
    if (*thread == NULL)
        status = EQX_STATUS_CREATE_THREAD_FAILED;
    else
        status = EQX_STATUS_SUCCESS;
#else
    if (pthread_create((pthread_t *) thread, NULL, thread_routine_invoker,
        invoker_param) == 0)
        status = EQX_STATUS_SUCCESS;
    else
        status = EQX_STATUS_CREATE_THREAD_FAILED;
#endif
    
    if (status != EQX_STATUS_SUCCESS)
        free(invoker_param);
    
    return status;
}

int xthread_detach(xthread_t thread)
{
    int status;
    
#ifdef EQX_WIN32
    if (CloseHandle((HANDLE) thread))
        status = 0;
    else
        status = EQX_STATUS_DETACH_THREAD_FAILED;
#else
    switch (pthread_detach((pthread_t) thread)) {
    case 0:
        status = 0;
        break;
    case ESRCH:
        status = EQX_STATUS_INVALID_PARAMETER;
        break;
    default:
        status = EQX_STATUS_DETACH_THREAD_FAILED;
        break;
    }
#endif
    
    return status;
}

bool xthread_is_running(xthread_t thread)
{
    assert(thread != NULL);
    
#ifdef EQX_WIN32
    if (WaitForSingleObject((HANDLE) thread, 0) == WAIT_TIMEOUT)
        return true;
#else
    if (pthread_kill((pthread_t) thread, 0) == 0)
        return true;
#endif
    
    return false;
}

int xthread_join(xthread_t thread, int *result)
{
    int status;
    
#ifdef EQX_WIN32
    switch (WaitForSingleObject((HANDLE) thread, INFINITE)) {
    case WAIT_OBJECT_0:
        if (result != NULL)
            GetExitCodeThread((HANDLE) thread, (LPDWORD) result);
        CloseHandle((HANDLE) thread);
        status = 0;
        break;
    default:
        status = EQX_STATUS_WAIT_FAILED;
        break;
    }
#else
    intptr_t exit_code;
    
    switch (pthread_join((pthread_t) thread, (void **) &exit_code)) {
    case 0:
        if (result != NULL)
            *result = (int) exit_code;
        status = 0;
        break;
    default:
        status = EQX_STATUS_WAIT_FAILED;
        break;
    }
#endif
    
    return status;
}

int xthread_mutex_create(xthread_mutex_t *mutex)
{
    struct xthread_mutex *m;
    int status;
    
    assert(mutex != NULL);
    
    /* Initialize Mutex Instance. */
    m = calloc(1, sizeof(*m));
    
    if (m == NULL)
        return EQX_STATUS_NOMEM;
    
#ifdef EQX_WIN32
    status = EQX_STATUS_SUCCESS;
    
    __try
    {
        InitializeCriticalSection(&m->mutex);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        // Alway STATUS_NO_MEMORY.
        status = EQX_STATUS_NOMEM;
    }
#else
    {
        pthread_mutexattr_t attr;
        
        status = pthread_mutexattr_init(&attr);
        
        if (status == 0) {
            status = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            
            if (status == 0) {
                status = pthread_mutex_init(&m->mutex, &attr);

                if (status == 0)
                    status = EQX_STATUS_SUCCESS;
                else
                    status = EQX_STATUS_CREATE_MUTEX_FAILED;
            } else {
                status = EQX_STATUS_ENABLE_MUTEX_RECURSIVE_FAILED;
            }
            
            pthread_mutexattr_destroy(&attr);
        } else {
            status = EQX_STATUS_CREATE_MUTEX_ATTR_FAILED;
        }
    }
#endif
    
    if (status == EQX_STATUS_SUCCESS)
        *mutex = m;
    else
        free(m);
    
    return status;
}

int xthread_mutex_destroy(xthread_mutex_t mutex)
{
    int status;
    
    assert(mutex != NULL);
    
#ifdef EQX_WIN32
    DeleteCriticalSection(&mutex->mutex);
    status = EQX_STATUS_SUCCESS;
#else
    status = pthread_mutex_destroy(&mutex->mutex);

    if (status == 0)
        status = EQX_STATUS_SUCCESS;
    else
        status = EQX_STATUS_DESTROY_MUTEX_FAILED;
#endif
    
    if (status == EQX_STATUS_SUCCESS)
        free(mutex);
    
    return status;
}

int xthread_mutex_lock(xthread_mutex_t mutex)
{
    int status;
    
    assert(mutex != NULL);
    
#ifdef EQX_WIN32
    EnterCriticalSection(&mutex->mutex);
    status = EQX_STATUS_SUCCESS;
#else
    status = pthread_mutex_lock(&mutex->mutex);

    if (status == 0)
        status = EQX_STATUS_SUCCESS;
    else
        status = EQX_STATUS_LOCK_MUTEX_FAILED;
#endif
    
    return status;
}

int xthread_mutex_unlock(xthread_mutex_t mutex)
{
    int status;
    
    assert(mutex != NULL);
    
#ifdef EQX_WIN32
    LeaveCriticalSection(&mutex->mutex);
    status = EQX_STATUS_SUCCESS;
#else
    status = pthread_mutex_unlock(&mutex->mutex);

    if (status == 0)
        status = EQX_STATUS_SUCCESS;
    else
        status = EQX_STATUS_UNLOCK_MUTEX_FAILED;
#endif
    
    return status;
}

xthread_t xthread_self(void)
{
#ifdef EQX_WIN32
    return (xthread_t) GetCurrentThread();
#else
    return (xthread_t) pthread_self();
#endif
}

int xthread_storage_alloc(xthread_storage_t *storage)
{
    struct xthread_storage *s;
    int status;
    
    assert(storage != NULL);
    
    /* Initialize Storage Instance. */
    s = calloc(1, sizeof(*s));
    
    if (s == NULL)
        return EQX_STATUS_NOMEM;
    
    /* Allocate TLS Index. */
#ifdef EQX_WIN32
    s->key = TlsAlloc();
    
    if (s->key == TLS_OUT_OF_INDEXES)
        status = EQX_STATUS_ALLOC_TLS_FAILED;
    else
        status = 0;
#else
    switch (pthread_key_create(&s->key, NULL)) {
    case 0:
        status = 0;
        break;
    case ENOMEM:
        status = EQX_STATUS_NOMEM;
        break;
    default:
        status = EQX_STATUS_ALLOC_TLS_FAILED;
        break;
    }
#endif
    
    if (status == 0)
        *storage = s;
    
    return status;
}

int xthread_storage_free(xthread_storage_t storage)
{
    int status;
    
    assert(storage != NULL);
    
#ifdef EQX_WIN32
    if (TlsFree(storage->key))
        status = 0;
    else
        status = EQX_STATUS_FREE_TLS_FAILED;
#else
    if (pthread_key_delete(storage->key) == 0)
        status = 0;
    else
        status = EQX_STATUS_FREE_TLS_FAILED;
#endif
    
    if (status == 0)
        free(storage);
    
    return status;
}

intptr_t xthread_storage_value_get(xthread_storage_t storage)
{
    intptr_t value;
    
    assert(storage != NULL);
    
#ifdef EQX_WIN32
    value = (intptr_t) TlsGetValue(storage->key);
#else
    value = (intptr_t) pthread_getspecific(storage->key);
#endif
    
    return value;
}

int xthread_storage_value_set(xthread_storage_t storage, intptr_t value)
{
    int status;
    
    assert(storage != NULL);
    
#ifdef EQX_WIN32
    if (TlsSetValue(storage->key, (LPVOID) value))
        status = 0;
    else
        status = EQX_STATUS_SET_TLS_VALUE_FAILED;
#else
    switch (pthread_setspecific(storage->key, (const void *) value)) {
    case 0:
        status = 0;
        break;
    case ENOMEM:
        status = EQX_STATUS_NOMEM;
        break;
    case EINVAL: /* Specified storage is invalid. */
        status = EQX_STATUS_INVALID_PARAMETER;
        break;
    default:
        status = EQX_STATUS_SET_TLS_VALUE_FAILED;
        break;
    }
#endif
    
    return status;
}
