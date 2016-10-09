#include <cassandra.h>
#include "common.h"

#define NUM_IO_WORKER_THREADS 		2
#define PENDING_REQ_HIGH_WATER_MARK	500
#define PENDING_REQ_LOW_WATER_MARK	1000

enum CASS_API_RETURN_ERROR_CODE{
	CASS_API_OK = 0,
	CASS_API_NULL_POINTER,
	CASS_API_INVALID_LENGHT
};

typedef struct _buffer_t CASS_API_BUFFER;

int cass_api_get_result_json(const CassResult *result, CASS_API_BUFFER *buffer, char *error);

CassCluster* cass_api_create_cluster(const char* hosts, const char *user, const char *passwd, int n_worker, int queues, int init_connection, int max_connection);
int cass_api_destroy_cluster(CassCluster *cass);