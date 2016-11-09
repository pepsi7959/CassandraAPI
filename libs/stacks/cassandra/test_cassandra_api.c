#include <cassandra.h>
#include <stdio.h>
#include <string.h>
#include <uv.h>

#include "cassandra_api.h"
#include "testcase.h"


void print_error(CassFuture* future) {
    const char* message;
    size_t message_length;
    cass_future_error_message(future, &message, &message_length);
    fprintf(stderr, "Error: %.*s\n", (int)message_length, message);
}

CassError execute_query(CassSession* session, const char* query, CassFuture **result) {
    CassError rc = CASS_OK;
    CassFuture* future = NULL;
    CassStatement* statement = cass_statement_new(query, 0);

    future = cass_session_execute(session, statement);
    cass_future_wait(future);

    rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        print_error(future);
    }

    *result = future;
    cass_statement_free(statement);
 
    return rc;
}


int test_select_version(){

    /* Setup and connect to cluster */
    CassError rc = CASS_OK;
    CassFuture* future = NULL;
    
    CassCluster* cluster = cass_api_create_cluster("127.0.0.1", "cassandra", "cassandra", 2, 10000, 1, 2);
    CassSession* session = cass_session_new();
    

    future = cass_session_connect(session, cluster);
    cass_future_wait(future);

    rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        print_error(future);
        cass_cluster_free(cluster);
        cass_session_free(session);
        return TEST_RESULT_FAILURE;
    }
    
    int ret = execute_query(session,  "SELECT release_version FROM system.local", &future);
    if( ret == CASS_OK){
        const CassResult* result = cass_future_get_result(future);
        CassIterator* iterator = cass_iterator_from_result(result);
        while(cass_iterator_next(iterator)) {
        const CassRow* row = cass_iterator_get_row(iterator);
            if(row){
                const CassValue* value = cass_row_get_column_by_name(row, "release_version");
                const char* release_version;
                size_t release_version_length;
                cass_value_get_string(value, &release_version, &release_version_length);
                DEBUG("release_version: '%.*s'\n", (int)release_version_length,
                   release_version);
            }
        }
    }


    cass_future_free(future);
    cass_cluster_free(cluster);
    cass_session_free(session);

    return TEST_RESULT_SUCCESS;
}

int test_select_simpledb(){

    /* Setup and connect to cluster */
    CassError rc = CASS_OK;
    CassFuture* future = NULL;
   
    CassCluster* cluster = cass_api_create_cluster("127.0.0.1", "cassandra", "cassandra", 2, 10000, 1, 2);
    CassSession* session = cass_session_new();
    
    future = cass_session_connect(session, cluster);
    cass_future_wait(future);

    rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        print_error(future);
        cass_cluster_free(cluster);
        cass_session_free(session);
        return TEST_RESULT_FAILURE;
    }
    
    int ret = execute_query(session,  "select uid,name,lasname,phone from examples.peaw", &future);
    if( ret == CASS_OK){
        const CassResult* result = cass_future_get_result(future);
        size_t row_count = cass_result_row_count(result);
        DEBUG("row count : %d", (int)row_count);

        CassIterator* iterator = cass_iterator_from_result(result);
        while(cass_iterator_next(iterator)) {
            const CassRow* row = cass_iterator_get_row(iterator);
            if(row){
                const CassValue* value = cass_row_get_column(row, 1);
                const char* col_value;
                size_t length;
                cass_value_get_string(value, &col_value, &length);
                DEBUG(":name'%.*s'", (int)length,col_value);
            }
        }
    }


    cass_future_free(future);
    cass_cluster_free(cluster);
    cass_session_free(session);

    return TEST_RESULT_SUCCESS;
}

int test_cass_api_get_result_json(){
/* Setup and connect to cluster */
    char error[1024]; error[0] = 0;
    CassError rc = CASS_OK;
    CassFuture* future = NULL;
    
    CassCluster* cluster = cass_api_create_cluster("127.0.0.1", "cassandra", "cassandra", 2, 10000, 1, 2);
    CassSession* session = cass_session_new();
    
    future = cass_session_connect(session, cluster);
    cass_future_wait(future);

    rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        print_error(future);
        cass_cluster_free(cluster);
        cass_session_free(session);
        return TEST_RESULT_FAILURE;
    }
    
    int ret = execute_query(session,  "select uid,name,lasname,phone from examples.peaw", &future);
    const CassResult* result = cass_future_get_result(future);
    CASS_API_BUFFER *buffer = NULL;
    ASSERT( cass_api_get_result_json(result, buffer, error) == CASS_API_OK );

    cass_future_free(future);
    cass_cluster_free(cluster);
    cass_session_free(session);

    return TEST_RESULT_SUCCESS;
}

int test_cass_api_get_result_column(){
/* Setup and connect to cluster */
    char error[1024]; error[0] = 0;
    CassError rc = CASS_OK;
    CassFuture* future = NULL;
    
    CassCluster* cluster = cass_api_create_cluster("127.0.0.1", "cassandra", "cassandra", 2, 10000, 1, 2);
    CassSession* session = cass_session_new();
    
    future = cass_session_connect(session, cluster);
    cass_future_wait(future);

    rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        print_error(future);
        cass_cluster_free(cluster);
        cass_session_free(session);
        return TEST_RESULT_FAILURE;
    }

    
    int ret = execute_query(session, "select * from examples.peaw ", &future);
    const CassResult* result = cass_future_get_result(future);
    CASS_API_BUFFER *buffer = NULL;
    ASSERT( cass_api_get_result_column(result, buffer, error,session) == CASS_API_OK ); 




    cass_future_free(future);
    cass_cluster_free(cluster);
    cass_session_free(session);

    return TEST_RESULT_SUCCESS;
}

/*main*/
int main(){
    UNIT_TEST(test_select_version());
    UNIT_TEST(test_select_simpledb());
    UNIT_TEST(test_cass_api_get_result_json());
    UNIT_TEST(test_cass_api_get_result_column());
    
    REPORT();
    
}


//bypeaw


