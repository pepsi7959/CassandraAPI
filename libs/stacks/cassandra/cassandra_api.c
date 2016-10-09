#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cassandra_api.h"

CassCluster* cass_api_create_cluster(const char* hosts, const char *user, const char *passwd, int n_worker, int queues, int init_connection, int max_connection){
  CassCluster* cluster = cass_cluster_new();
    cass_cluster_set_contact_points(cluster, hosts);
    cass_cluster_set_credentials(cluster, user, passwd);
    cass_cluster_set_num_threads_io(cluster, n_worker);
    cass_cluster_set_queue_size_io(cluster, queues);
    cass_cluster_set_pending_requests_low_water_mark(cluster, 5000);
    cass_cluster_set_pending_requests_high_water_mark(cluster, 10000);
    cass_cluster_set_core_connections_per_host(cluster, init_connection);
    cass_cluster_set_max_connections_per_host(cluster, max_connection);
    cass_cluster_set_max_requests_per_flush(cluster, 10000);
    return cluster;
}

int cass_api_get_result_json(const CassResult *result, CASS_API_BUFFER *buffer, char *error){
    int i = 0;
    if( buffer == NULL ){
        ALLOCATE_BUFFER(buffer, DATA_BUFFER);
    }

    i += sprintf(&buffer->data[i], "{\n");
    CassIterator* iterator = cass_iterator_from_result(result);
    while(cass_iterator_next(iterator)) {
        const CassRow* row = cass_iterator_get_row(iterator);
        if(row){
            i += sprintf(&buffer->data[i], "{");
            int col = 0;
            for(;;col++){
                const CassValue* value = cass_row_get_column(row, col);
                if( value == NULL){break;}
                // /const CassValue* value = cass_row_get_column_by_name(row, "emp_city");
                const char* col_value;
                size_t length;
                cass_value_get_string(value, &col_value, &length);
                i += sprintf(&buffer->data[i], "\"attribute\":\"%.*s\",\n", (int)length,col_value);
            }
            i += sprintf(&buffer->data[i], "},\n");
        }
    }
    i += sprintf(&buffer->data[i], "}");
    printf("%s\n", buffer->data);
    return CASS_API_OK;
}