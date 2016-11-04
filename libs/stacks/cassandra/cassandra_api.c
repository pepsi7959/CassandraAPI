#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cassandra_api.h"
#include <time.h>       /* time_t, time (for timestamp in second) */


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
    const CassKeyspaceMeta* keyspace_meta = cass_schema_meta_keyspace_by_name(schema_meta, "peaw");
    const CassTableMeta* table_meta = cass_keyspace_meta_table_by_name(keyspace_meta, "dearall");

    
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

          int col = 0;
          //for(;;col++){

            CassIterator* iterator_colunm = cass_iterator_columns_from_table_meta(table_meta);
                while (cass_iterator_next(iterator_colunm)) {
                  const CassValue* value = cass_row_get_column(row, col);
                  if( value == NULL){break;}

                      //get_name_column
                      const CassColumnMeta* meta = cass_iterator_get_column_meta(iterator_colunm);
                      const char* name;
                      size_t name_length;
                      cass_column_meta_name(meta, &name, &name_length);


                      CassIterator* iterator_list = cass_iterator_from_collection(value);
                      //cass_bool_t is_first = cass_true;
                        


                      //get_string
                      const char* col_value = NULL;
                      size_t length;

                      //cass_valueType
                      cass_int64_t t;
                      cass_int32_t it;
                      cass_bool_t b;
                      cass_double_t d;
                      CassUuid u;
                      char us[CASS_UUID_STRING_LENGTH];
                      
                      //time
                      struct tm  timeinfo;
                      char       buf[80];


                      if( col_value != NULL || col_value == NULL){col = col +1;}

                    
                      CassValueType type = cass_value_type(value);
                      switch (type) {
                        case CASS_VALUE_TYPE_INT:
                          if (cass_value_is_null(value)) {
                            i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                            i += sprintf(&buffer->data[i], "\"NULL\",\n");
                          break;
                          }else{
                            cass_value_get_int32(value, &it);
                            i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                            i += sprintf(&buffer->data[i], "\"%d\",\n",it);
                          break;
                          }

                        case CASS_VALUE_TYPE_BOOLEAN:
                        
                          //printf("%s", b ? "true" : "false");
                          if (cass_value_is_null(value)) {
                            i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                            i += sprintf(&buffer->data[i], "\"NULL\",\n");
                          break;
                          }else{
                            cass_value_get_bool(value, &b);
                            i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                            i += sprintf(&buffer->data[i], "%s", b ? "true" : "false");
                          break;
                          }

                        case CASS_VALUE_TYPE_DOUBLE:
                         // printf("%f", d);
                         if (cass_value_is_null(value)) {
                            i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                            i += sprintf(&buffer->data[i], "\"NULL\",\n");
                          break;
                          }else{
                           cass_value_get_double(value, &d);
                            i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                            i += sprintf(&buffer->data[i], "\"%f\",\n",d);
                          break;
                          }

                        case CASS_VALUE_TYPE_TEXT:
                        case CASS_VALUE_TYPE_ASCII:
                        case CASS_VALUE_TYPE_VARCHAR:
                        if (cass_value_is_null(value)) {
                          i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                          i += sprintf(&buffer->data[i], "\"NULL\",\n");
                          break;
                         }else{

                            cass_value_get_string(value, &col_value, &length);
                            i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                            i += sprintf(&buffer->data[i], "\"%.*s\",\n", (int)length,col_value);
                          //printf("\"%.*s\"\n", (int)length, col_value);
                          break;
                        }

                        case CASS_VALUE_TYPE_UUID:
                         
                          if (cass_value_is_null(value)) {
                            i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                            i += sprintf(&buffer->data[i], "\"NULL\",\n");
                          break;
                         }else{

                          cass_value_get_uuid(value, &u);
                          cass_uuid_string(u, us);
                          //printf("%s", us);
                            i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                            i += sprintf(&buffer->data[i], "\"%s\",\n", us);
                         
                          break;
                        }

                       case CASS_VALUE_TYPE_TIMESTAMP:
                       //case CASS_VALUE_TYPE_DATE:

                         if (cass_value_is_null(value)) {
                              i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                              i += sprintf(&buffer->data[i], "\"NULL\",\n");
                            break;
                        }else{

                            
                            cass_value_get_int64(value, &t);
                           
                            time_t tt = (time_t)t/1000;

                            // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
                             timeinfo = *localtime(&tt);
                             strftime(buf, sizeof(buf), " %Y-%m-%d %H:%M:%S %z", &timeinfo);

                            i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                            i += sprintf(&buffer->data[i], "\"%s\",\n", buf);

                          break;
                        }

                       case CASS_VALUE_TYPE_LIST:

                     
                        //printf("[ ");
                        while (cass_iterator_next(iterator_list)) {
                          int count_row=count_row+1;
                          //if (!is_first) i += sprintf(&buffer->data[i], ",");//printf(", ");
                          const CassValue* value = cass_iterator_get_value(iterator_list);
                          //const char* col_value = NULL;
                          //size_t length;
                          //is_first = cass_false;

                          cass_value_get_string(value, &col_value, &length);
                          i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                          i += sprintf(&buffer->data[i], "\"%.*s\",\n", (int)length,col_value);

                          //printf("\"%.*s\"\n", (int)length, col_value);
                        }
                        //printf(" ]");
              
                      

                          break;

                        case CASS_VALUE_TYPE_MAP:
                          //print_schema_map(value);
                          break;

                        case CASS_VALUE_TYPE_BLOB:
                          //print_schema_bytes(value);
                          break;

                        default:
                          
                            if (cass_value_is_null(value)) {
                              i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                              i += sprintf(&buffer->data[i], "\"NULL\",\n");
                            } else {
                              i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                              i += sprintf(&buffer->data[i], "\"<unhandled type>\",\n");

                            }

                          break;
                      }

                  }  
                    cass_iterator_free(iterator_colunm);

   
                i += sprintf(&buffer->data[i], "},\n");
              }
            }

             cass_iterator_free(iterator);

        i += sprintf(&buffer->data[i], "}");
        printf("%s\n", buffer->data);
        return CASS_API_OK;
    }







                        /*void print_schema_list(const CassValue* value) {
                        CassIterator* iterator = cass_iterator_from_collection(value);
                        cass_bool_t is_first = cass_true;

                        printf("[ ");
                        while (cass_iterator_next(iterator)) {

                          if (!is_first) printf(", ");
                          CassValue* value = cass_iterator_get_value(iterator);
                          const char* col_value = NULL;
                          size_t length;
                          is_first = cass_false;
                          cass_value_get_string(value, &col_value, &length);

                          i += sprintf(&buffer->data[i], "\"%.*s\":", (int)name_length,name);
                          i += sprintf(&buffer->data[i], "\"%.*s\",\n", (int)length,col_value);
                          //printf("\"%.*s\"\n", (int)length, col_value);
                        }
                        printf(" ]");
                        cass_iterator_free(iterator);
                      }*/




