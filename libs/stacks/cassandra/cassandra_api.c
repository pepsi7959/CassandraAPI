#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cassandra_api.h"


CassCluster* cass_api_create_cluster(const char* hosts, const char *user, const char *passwd, int n_worker, int queues, int init_connection, int max_connection){
  CassCluster* cluster = cass_cluster_new();
    cass_cluster_set_protocol_version(cluster,2);
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


int cass_api_get_result_column(const CassResult *result, CASS_API_BUFFER *buffer, char *error, const CassSession* session){

    const CassSchemaMeta* schema_meta = cass_session_get_schema_meta(session);
    const CassKeyspaceMeta* keyspace_meta = cass_schema_meta_keyspace_by_name(schema_meta, "examples");
    const CassTableMeta* table_meta = cass_keyspace_meta_table_by_name(keyspace_meta, "peaw");

    
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

        CassIterator* iterator_table = cass_iterator_tables_from_keyspace_meta(keyspace_meta);

        
        
        while (cass_iterator_next(iterator_table)) {
              const CassTableMeta* meta_table = cass_iterator_get_table_meta(iterator_table);
                    const char* name;
                    size_t name_length;
                    cass_table_meta_name(meta_table, &name, &name_length);  
                    
                  if (strcmp(name, "peaw")==0) {
                              
                    i += sprintf(&buffer->data[i], "\"Table\":\"%.*s\",\n", (int)name_length,name);

                    }
                  }
              

          int col = 0;
          //for(;;col++){

            CassIterator* iterator_colunm = cass_iterator_columns_from_table_meta(table_meta);
                while (cass_iterator_next(iterator_colunm)) {
                  const CassValue* value = cass_row_get_column(row, col);
                  if( value == NULL){break;}
                  //const CassValue* meta = cass_row_get_column_by_name(row, "uid");

                  const char* col_value = NULL;
                  size_t length;
                  cass_value_get_string(value, &col_value, &length);



                  const CassColumnMeta* meta = cass_iterator_get_column_meta(iterator_colunm);
                  const char* name;
                  size_t name_length;
                  cass_column_meta_name(meta, &name, &name_length);

                  if( col_value != NULL){
                      i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                      i += sprintf(&buffer->data[i], "\"%.*s\",\n", (int)length,col_value);
                      col = col +1;
                      //continue;


                  }else if( col_value == NULL){
                      i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                      i += sprintf(&buffer->data[i], "\"%.*s\",\n", (int)length,col_value);
                      col = col +1;
                    //continue;
                   } 

                //while
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
