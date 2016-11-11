#include <cassandra.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <uv.h>

#include "cassandra_api.h"
#include "testcase.h"


void print_error(CassFuture* future) {
    printf("print_errorn ");
    const char* message;
    size_t message_length;
    cass_future_error_message(future, &message, &message_length);
    fprintf(stderr, "Error: %.*s\n", (int)message_length, message);
}

CassCluster* create_cluster(const char* hosts) {
    printf("create_cluster ");
    CassCluster* cluster = cass_cluster_new();
    cass_cluster_set_contact_points(cluster, hosts);
    return cluster;
}

/*connection*/
CassError connect_session(CassSession* session, const CassCluster* cluster) {
    printf("connect_session ");
    CassError rc = CASS_OK;
    CassFuture* future = cass_session_connect(session, cluster);

    cass_future_wait(future);
    rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        print_error(future);
    }
    cass_future_free(future);

    return rc;
}

CassError execute_query(CassSession* session, const char* query, CassFuture **result) {
    printf("execute_query ");
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
    printf("test_select_version ");
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
    
    int ret = execute_query(session,  "SELECT * FROM testdb.testtable", &future);
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
    printf("test_select_simpledb ");
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
    
    int ret = execute_query(session,  "SELECT * FROM testdb.testtable", &future);
    if( ret == CASS_OK){
        const CassResult* result = cass_future_get_result(future);
        size_t row_count = cass_result_row_count(result);
        DEBUG("row count : %d", (int)row_count);

        CassIterator* iterator = cass_iterator_from_result(result);
        while(cass_iterator_next(iterator)) {
            const CassRow* row = cass_iterator_get_row(iterator);
            if(row){
                const CassValue* value = cass_row_get_column(row, 1);
                //const CassValue* value = cass_row_get_column_by_name(row, "emp_city");
                const char* col_value;
                size_t length;
                cass_value_get_string(value, &col_value, &length);
                DEBUG(":id'%.*s'", (int)length,col_value);
            }
        }
    }


    cass_future_free(future);
    cass_cluster_free(cluster);
    cass_session_free(session);

    return TEST_RESULT_SUCCESS;
}



int test_cass_api_get_result_json(){
    printf("test_cass_api_get_result_json ");
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
    
    int ret = execute_query(session,  "SELECT * FROM testdb.testtable", &future);
    const CassResult* result = cass_future_get_result(future);
    CASS_API_BUFFER *buffer = NULL;
    ASSERT( cass_api_get_result_json(result, buffer, error) == CASS_API_OK );

    cass_future_free(future);
    cass_cluster_free(cluster);
    cass_session_free(session);

    return TEST_RESULT_SUCCESS;
}

int test_cass_api_get_result_column(){
    printf("test_cass_api_get_result_column ");
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



    
    int ret = execute_query(session, "SELECT * FROM testdb.testtable", &future);
    const CassResult* result = cass_future_get_result(future);
    CASS_API_BUFFER *buffer = NULL;
    //ASSERT( cass_api_get_result_column(result, buffer, error,session) == CASS_API_OK ); 




    cass_future_free(future);
    cass_cluster_free(cluster);
    cass_session_free(session);

    return TEST_RESULT_SUCCESS;
}



int test_cass_api_get_result_paging(){
    printf("test_cass_api_get_result_paging ");
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



    
    //int ret = execute_query(session, "SELECT * FROM testdb.testtable WHERE id='a09'", &future);
    const CassResult* result = cass_future_get_result(future);
    CASS_API_BUFFER *buffer = NULL;
    ASSERT( cass_api_get_result_paging(result, buffer, error,session) == CASS_API_OK ); 




    cass_future_free(future);
    cass_cluster_free(cluster);
    cass_session_free(session);

    return TEST_RESULT_SUCCESS;
}



int select_from_paging(CassSession* session) {
    printf("select_from_paging ");

    const CassSchemaMeta* schema_meta = cass_session_get_schema_meta(session);
    const CassKeyspaceMeta* keyspace_meta = cass_schema_meta_keyspace_by_name(schema_meta, "testdb");
    const CassTableMeta* table_meta = cass_keyspace_meta_table_by_name(keyspace_meta, "testtable");

    int count_row=0;
    int count_page=0;
    int size_page=10;
    int select_page=2;
    int i = 0;
    CASS_API_BUFFER *buffer;
    cass_bool_t has_more_pages = cass_false;
    const CassResult* result = NULL;
    const char* query = "SELECT * FROM testdb.testtable";
    //CassString query = cass_string_init("SELECT * FROM testdb.testtable");
    //const char* query = "SELECT * FROM testtable WHERE test_pri='a00' ALLOW FILTERING";
    //SELECT * FROM testtable WHERE token(id)=token('a09')
    CassStatement* statement = cass_statement_new(query, 0);

    cass_statement_set_paging_size(statement, size_page);
    do {
        CassIterator* iterator;
        CassFuture* future = cass_session_execute(session, statement);

        if (cass_future_error_code(future) != 0) {
            print_error(future);
            break;
        }
        result = cass_future_get_result(future);
        iterator = cass_iterator_from_result(result);
        cass_future_free(future);
        if (count_page == select_page){
            printf(" eq \n");





            while (cass_iterator_next(iterator)) {
                const CassRow* row = cass_iterator_get_row(iterator);

                if(row){
                    int col = 0;
                    CassIterator* iterator_column = cass_iterator_columns_from_table_meta(table_meta);
                    while (cass_iterator_next(iterator_column)) {
                        ALLOCATE_BUFFER(buffer, DATA_BUFFER);
                        const CassValue* value = cass_row_get_column(row, col);
                        if( value == NULL){
                            break;
                        }


                        /*get name column*/
                        const CassColumnMeta* meta_column = cass_iterator_get_column_meta(iterator_column);
                        const char* name_column;
                        size_t name_column_length;
                        cass_column_meta_name(meta_column, &name_column, &name_column_length);


                        const char* col_value = NULL;
                        size_t length;
                        cass_value_get_string(value, &col_value, &length);


                        if( col_value != NULL){
                            i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_column_length,name_column);
                            i += sprintf(&buffer->data[i], "\"%.*s\",\n", (int)length,col_value);
                        }else if( col_value == NULL){
                            i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_column_length,name_column);
                            i += sprintf(&buffer->data[i], "\"%.*s\",\n", (int)length,col_value);
                        } 



                        /*
                        CassUuid key;
                        char key_str[CASS_UUID_STRING_LENGTH];
                        const char* value;
                        size_t value_length;
                        char value_buffer[256];

     
                        cass_value_get_uuid(cass_row_get_column(row, 0), &key);
                        cass_uuid_string(key, key_str);

                        cass_value_get_string(cass_row_get_column(row, 1), &value, &value_length);
                        memcpy(value_buffer, value, value_length);
                        value_buffer[value_length] = '\0';

                        //printf("key: '%s' value: '%s'\n", key_str, value_buffer);

                        printf("value: '%s'\n",value_buffer);*/
                        col = col +1;
                    }/*end while col*/
                    count_row=count_row+1;
                    
                }/*end if row*/



            count_page=count_page+1;
            }/*end loop while*/

            printf("%s\n", buffer->data);


        }
        else if (count_page != select_page){
            printf(" nq \n");
            count_page=count_page+1;
        }
        //printf("Count row : %d row\n",count_row);

       //printf("Count page : %d page\n",count_page);
        //printf(" \n");
        count_row=0;
        has_more_pages = cass_result_has_more_pages(result);

        if (has_more_pages) {
            cass_statement_set_paging_state(statement, result);
        }

        cass_iterator_free(iterator);
        cass_result_free(result);

    } while (has_more_pages);
    cass_statement_free(statement);
    return CASS_API_OK;
}







int main(){
    printf("main ");
    //UNIT_TEST(test_select_version());
    //UNIT_TEST(test_select_simpledb());
    //UNIT_TEST(test_cass_api_get_result_json());
    //UNIT_TEST(test_cass_api_get_result_column());
    UNIT_TEST(test_cass_api_get_result_paging());
    
    //REPORT();
    
}

