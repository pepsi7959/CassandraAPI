#ifndef _XTHREAD_H
#define _XTHREAD_H

#include "common.h"

struct xthread;
typedef struct xthread * xthread_t;

struct xthread_mutex;
typedef struct xthread_mutex * xthread_mutex_t;

struct xthread_storage;
typedef struct xthread_storage * xthread_storage_t;

typedef int (*xthread_routine_t) (void *param);

/**
 * Create a new thread to execute \p routine.
 *
 * @param thread Pointer to \c xthread_t to receive a thread handle.
 * @param routine Pointer to \c xthread_routine_t to execute on created thread.
 * @param param Value to passing to \c xthread_routine_t via parameter.
 *
 * @return zero on success, otherwise one of EQX_STATUS_*.
 */
int xthread_create(xthread_t *thread, xthread_routine_t routine, void *param);

/**
 * Detach a thread.
 * 
 * This function will free any resource that associated with specified thread
 * handle. Thus, make it invalid to use in the future. But, do not terminate the
 * thread.
 * 
 * @param thread A thread to detach. This must be \c xthread_t that returned
 *        from xthread_create().
 * 
 * @return zero on success, otherwise one of EQX_STATUS_*.
 */
int xthread_detach(xthread_t thread);

/**
 * Check for specified thread is terminated or not.
 * 
 * @param thread Thread to check. Must be handle that returned from
 *        xthread_create().
 * 
 * @return non-zero if specified thread is not terminated. Otherwise zero.
 */
bool xthread_is_running(xthread_t thread);

/**
 * Wait a thread to terminate and free handle and any resource that associated
 * with it.
 * 
 * Do not call this function on the same thread with multiple times, unless the
 * function is failed.
 * 
 * @param thread Thread to wait. Must be handle that returned from
 *        xthread_create().
 * @param result Pointer to \c int to receive value that returned from thread
 *        routine; can be NULL.
 * 
 * @return zero on success, otherwise one of EQX_STATUS_*.
 */
int xthread_join(xthread_t thread, int *result);

/**
 * Create a new mutex object.
 *
 * @param mutex Pointer to \c xthread_mutex_t to receive created mutex.
 *
 * @return zero on success, otherwise one of EQX_STATUS_*.
 */
int xthread_mutex_create(xthread_mutex_t *mutex);

/**
 * Destroy mutext object. A destroyed mutex cannot use anymore.
 *
 * @param mutex Mutext to destroy.
 *
 * @return zero on success, otherwise one of EQX_STATUS_*.
 */
int xthread_mutex_destroy(xthread_mutex_t mutex);

/**
 * Lock specified mutex.
 *
 * Only one thread can lock a mutex. If specified mutex has already locked by
 * another thread, it will wait until that thread unlock it.
 *
 * Mutex can be locked recursive by the same thread. A locked mutext must be
 * unlock with the same locking count.
 *
 * @param mutex Mutex to lock.
 *
 * @return zero on success, otherwise one of EQX_STATUS_*.
 */
int xthread_mutex_lock(xthread_mutex_t mutex);

/**
 * Unlock specified mutex.
 *
 * @param mutex Mutex to unlock.
 *
 * @return zero on success, otherwise one of EQX_STATUS_*.
 */
int xthread_mutex_unlock(xthread_mutex_t mutex);

/**
 * Get a thread handle for the calling thread.
 * 
 * Thread handle that obtained via this function can be differenced from handle
 * that obtained via xthread_create().
 * 
 * Do not use \c xthread_t that obtained via this function outside the calling
 * thread.
 * 
 * @return \c xthread_t for the calling thread.
 */
xthread_t xthread_self(void);

/**
 * Allocate a new TLS (Thread Local Storage).
 * 
 * \p storage can be used in any thread without locking to retrieve data that
 * associate with the calling thread.
 * 
 * @param storage Pointer to \c xthread_storage_t to receive TLS identifier.
 * 
 * @return zero on success, otherwise one of EQX_STATUS_*.
 */
int xthread_storage_alloc(xthread_storage_t *storage);

/**
 * Free specified storage.
 * 
 * User is responsible to free all data that associate with specified storage
 * on all thread.
 * 
 * This function is not thread safe; so, make sure no other thread is using
 * \p storage during this function is running.
 * 
 * @param storage Storage to free.
 * 
 * @return zero on success, otherwise one of EQX_STATUS_*.
 */
int xthread_storage_free(xthread_storage_t storage);

/**
 * Get value that associate with storage for the calling thread.
 * 
 * This function can be calling from any thread without locking specified
 * storage.
 * 
 * Every thread has it own value with the same storage.
 * 
 * @param storage Storage of value to get.
 * 
 * @return value that associate with specified storage for the calling thread.
 */
intptr_t xthread_storage_value_get(xthread_storage_t storage);

/**
 * Associate value with storage for the calling thread.
 * 
 * This function can be calling from any thread without locking specified
 * storage.
 * 
 * Every thread has it own value with the same storage.
 * 
 * @param storage Storage to use for storing \p value.
 * @param value Value to store in \p storage.
 * 
 * @return zero on success, otherwise one of EQX_STATUS_*.
 */
int xthread_storage_value_set(xthread_storage_t storage, intptr_t value);

#endif /* _XTHREAD_H */
