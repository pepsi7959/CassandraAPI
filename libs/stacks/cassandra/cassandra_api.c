/*
cassandra 3.x

*/




#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cassandra_api.h"

/*for paging*/
#include <assert.h>
#define NUM_CONCURRENT_REQUESTS 1000



CassCluster* cass_api_create_cluster(const char* hosts, const char *user, const char *passwd, int n_worker, int queues, int init_connection, int max_connection){
  CassCluster* cluster = cass_cluster_new();
    //cass_cluster_set_protocol_version(cluster,2);
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
                ALLOCATE_BUFFER(buffer, DATA_BUFFER);
                const CassValue* value = cass_row_get_column(row, col);
                if( value == NULL){break;}
                // /const CassValue* value = cass_row_get_column_by_name(row, "emp_city");
                const char* col_value = NULL;
                size_t length;
                cass_value_get_string(value, &col_value, &length);

                if( col_value != NULL){


                     i += sprintf(&buffer->data[i], "\"attribute\":\"%.*s\",\n", (int)length,col_value);


                }else if( col_value == NULL){

                    i += sprintf(&buffer->data[i], "\"attribute\":\"%.*s\",\n", (int)length,col_value);

                } 
               
            }
            i += sprintf(&buffer->data[i], "},\n");
        }
    }
    i += sprintf(&buffer->data[i], "}");
    printf("%s\n", buffer->data);
    return CASS_API_OK;
}


void print_keyspace_meta(const CassKeyspaceMeta* meta, int indent);
void print_table_meta(const CassTableMeta* meta, int indent);
void print_column_meta(const CassColumnMeta* meta, int indent);
int cass_api_get_result_column(const CassResult *result, CASS_API_BUFFER *buffer, char *error, const CassSession* session);


void print_keyspace(CassSession* session, const char* keyspace) {
  const CassSchemaMeta* schema_meta = cass_session_get_schema_meta(session);
  const CassKeyspaceMeta* keyspace_meta = cass_schema_meta_keyspace_by_name(schema_meta, keyspace);

  if (keyspace_meta != NULL) {
    print_keyspace_meta(keyspace_meta, 0);
  } else {
    fprintf(stderr, "Unable to find \"%s\" keyspace in the schema metadata\n", keyspace);
  }

  cass_schema_meta_free(schema_meta);
}


void print_table(CassSession* session, const char* keyspace, const char* table) {
  const CassSchemaMeta* schema_meta = cass_session_get_schema_meta(session);
  const CassKeyspaceMeta* keyspace_meta = cass_schema_meta_keyspace_by_name(schema_meta, keyspace);

  if (keyspace_meta != NULL) {
    const CassTableMeta* table_meta = cass_keyspace_meta_table_by_name(keyspace_meta, table);
    if (table_meta != NULL) {
      print_table_meta(table_meta, 0);
    } else {
      fprintf(stderr, "Unable to find \"%s\" table in the schema metadata\n", table);
    }
  } else {
    fprintf(stderr, "Unable to find \"%s\" keyspace in the schema metadata\n", keyspace);
  }

  cass_schema_meta_free(schema_meta);
}


void print_indent(int indent) {
  int i;
  for (i = 0; i < indent; ++i) {
    printf("\t");
  }
}


void print_keyspace_meta(const CassKeyspaceMeta* meta, int indent) {
  const char* name;
  size_t name_length;
  CassIterator* iterator;

  print_indent(indent);
  cass_keyspace_meta_name(meta, &name, &name_length);
  printf("Keyspace \"%.*s\":\n", (int)name_length, name);

  //print_meta_fields(cass_iterator_fields_from_keyspace_meta(meta), indent + 1);
  printf("\n");

  iterator = cass_iterator_tables_from_keyspace_meta(meta);
  while (cass_iterator_next(iterator)) {
    print_table_meta(cass_iterator_get_table_meta(iterator), indent + 1);
  }
  printf("\n");

  cass_iterator_free(iterator);
}

void print_table_meta(const CassTableMeta* meta, int indent) {
  const char* name;
  size_t name_length;
  CassIterator* iterator;

  print_indent(indent);
  cass_table_meta_name(meta, &name, &name_length);
  printf("Table \"%.*s\":\n", (int)name_length, name);

  //print_meta_fields(cass_iterator_fields_from_table_meta(meta), indent + 1);
  printf("\n");

  iterator = cass_iterator_columns_from_table_meta(meta);
  while (cass_iterator_next(iterator)) {
   print_column_meta(cass_iterator_get_column_meta(iterator), indent + 1);
  }
  printf("\n");

  cass_iterator_free(iterator);
}

  void print_column_meta(const CassColumnMeta* meta, int indent) {
  const char* name;
  size_t name_length;
  print_indent(indent);
  cass_column_meta_name(meta, &name, &name_length);
  printf("Column \"%.*s\":\n", (int)name_length, name);
  //print_meta_fields(cass_iterator_fields_from_column_meta(meta), indent + 1);

  printf("\n");
} 



/*JSON by Column name*/
int cass_api_get_result_column(const CassResult *result, CASS_API_BUFFER *buffer, char *error, const CassSession* session){
  int count_row=0;
  const CassSchemaMeta* schema_meta = cass_session_get_schema_meta(session);
  const CassKeyspaceMeta* keyspace_meta = cass_schema_meta_keyspace_by_name(schema_meta, "testdb");
  const CassTableMeta* table_meta = cass_keyspace_meta_table_by_name(keyspace_meta, "testtable");

    
  int indent=0;
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
      ALLOCATE_BUFFER(buffer, DATA_BUFFER);
      CassIterator* iterator_column = cass_iterator_columns_from_table_meta(table_meta);
      int col = 0;
        while (cass_iterator_next(iterator_column)) {
          const CassValue* value = cass_row_get_column(row, col);

          /*check value is null*/
          if( value == NULL){break;}


          /*get data of column*/
          const char* col_value = NULL;
          size_t length;
          cass_value_get_string(value, &col_value, &length);


          /*get name column*/
          const CassColumnMeta* meta_column = cass_iterator_get_column_meta(iterator_column);
          const char* name_column;
          size_t name_column_length;
          cass_column_meta_name(meta_column, &name_column, &name_column_length);


          /*check data type*/
          cass_int32_t j;
          cass_bool_t b;
          cass_double_t d;
          const char* s=NULL;
          size_t s_length;
          CassUuid u;
          char us[CASS_UUID_STRING_LENGTH];


          /*for time stamp*/
          cass_int64_t timestamp;
          struct tm  timeinfo;
          char buf[80];

          if( col_value != NULL){

            if(cass_value_type(value)==CASS_VALUE_TYPE_INT){
              //printf("A");
              cass_value_get_int32(value, &j);
              i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_column_length,name_column);
              i += sprintf(&buffer->data[i], "\"%d\",\n",j);
              //col = col +1;
              //printf("----------> %d <----------\n", j);
            }
            else if (cass_value_type(value)==CASS_VALUE_TYPE_TEXT || cass_value_type(value)==CASS_VALUE_TYPE_ASCII || cass_value_type(value)==CASS_VALUE_TYPE_VARCHAR ){
              //printf("B");
              cass_value_get_string(value, &s, &s_length);
              i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_column_length,name_column);
              i += sprintf(&buffer->data[i], "\"%.*s\",\n",(int)s_length,s);
              //col = col +1;
              //printf("----------> \"%.*s\" <----------\n", (int)s_length, s);
            }
            else if (cass_value_type(value)==CASS_VALUE_TYPE_UUID ){
              //printf("C");
              cass_value_get_uuid(value, &u);
              cass_uuid_string(u, us);
              i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_column_length,name_column);
              i += sprintf(&buffer->data[i], "\"%s\",\n",us);
              //printf("----------> %s <----------\n", us);
            }
            else if (cass_value_type(value)==CASS_VALUE_TYPE_BOOLEAN){
              cass_value_get_bool(value, &b);
              i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_column_length,name_column);
              i += sprintf(&buffer->data[i], "\"%s\",\n", b ? "true" : "false");
              //printf("%s", b ? "true" : "false");
            }
            else if (cass_value_type(value)==CASS_VALUE_TYPE_DOUBLE){
              cass_value_get_double(value, &d);
              i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_column_length,name_column);
              i += sprintf(&buffer->data[i], "\"%f\",\n", d);
              //printf("%f", d);
            }
            else if (cass_value_type(value)==CASS_VALUE_TYPE_TIMESTAMP){
 

              cass_value_get_int64(value, &timestamp);
              time_t tt = (time_t)timestamp/1000;
              timeinfo = *localtime(&tt);
              strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S %z", &timeinfo);





              //printf("timestamp %.s\n", us);
              i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_column_length,name_column);
              i += sprintf(&buffer->data[i], "\"%s\",\n", buf);
              

            }
            else{
              //printf("E");
              i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_column_length,name_column);
              i += sprintf(&buffer->data[i], "\"unhandled type\",\n");
              //printf("----------> unhandled type <----------\n");
            }
            col = col +1;
          }/*col_value != NULL*/
          else if( col_value == NULL ){
            if (cass_value_is_null(value)){
              i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_column_length,name_column);
              i += sprintf(&buffer->data[i], "\"null\",\n");
              col = col +1;
            }
          }/*col_value == NULL*/
        }/*end while*/
      count_row=count_row+1;
      i += sprintf(&buffer->data[i], "},\n");
    }/*end fi row*/
  }/*end while*/
  i += sprintf(&buffer->data[i], "}");
  printf("%s\n", buffer->data);
  cass_iterator_free(iterator);
  cass_result_free(result);
  printf("Count row : %d row\n",count_row);
  return CASS_API_OK;
}/*end function*/


/*JSON by Paging*/
int cass_api_get_result_paging(const CassResult *result, CASS_API_BUFFER *buffer, char *error, const CassSession* session){
 int i = 0;
    if( buffer == NULL ){
        ALLOCATE_BUFFER(buffer, DATA_BUFFER);
    }
    
    select_from_paging(session);
    //select_from_paging_v2(session);
    printf("Paging\n");
    return CASS_API_OK;
}/*end function*/
