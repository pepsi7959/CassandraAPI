#include <cassandra.h>

#define NUM_IO_WORKER_THREADS 		2
#define PENDING_REQ_HIGH_WATER_MARK	500
#define PENDING_REQ_LOW_WATER_MARK	1000

CassCluster* create_cluster(const char* hosts, const char *user, const char *passwd, int n_worker, int queues, int init_connection, int max_connection);
int destroy_cluster(CassCluster *cass);