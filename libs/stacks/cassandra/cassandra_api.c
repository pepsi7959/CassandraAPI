#include "cassandra_api.h"

CassCluster* create_cluster(const char* hosts, const char *user, const char *passwd, int n_worker, int queues, int init_connection, int max_connection){
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
