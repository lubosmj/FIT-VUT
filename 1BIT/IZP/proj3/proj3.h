/**
 * @file Proj3.h
 * @author Lubos Mjachky
 * @date 13 Dec 2015
 * @brief Header file containing documentation of functions
 *
 * @details Contains documentation to the project number 3.
 */

/** \addtogroup Cluster
 * @{
 * Structures
 */

/**
 * Structure of objects called obj_t.
 */
struct obj_t {
	/** defines an unique ID of object */
    int id;
	/** defines position of the object on axis X */
	float x;
	/** defines position of the object on axis Y */
	float y; 
};

/**
 * Structure of clusters called cluster_t.
 */
struct cluster_t {
	/** size of cluster (means how many objects are in cluster) */
    int size;
	/** capacity of cluster (reserved memory for objects of cluster) */
	int capacity;
	/** array of objects */
	struct obj_t *obj; 
};
/** @} */

/**
 * @defgroup Cluster Cluster operations
 *
 * Operations which are done by cluster analysis.
 *
 * @{
 */

/**
 * @brief Initialization of cluster. 
 *
 * This function initializes the cluster and allocates the memory for capacity of the object.
 *
 * @param *c Pointer to the cluster_t structure
 * @param cap Integer value, represents the capacity of the cluster
 *
 * @pre cap > 0 
 * @post Initialization of cluster *c. If the pointer to the object array equals to NULL it means, that capacity of the cluster will be set to zero.
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief Clearing the cluster.
 *
 * It removes all objects of the cluster and initializes that cluster to the empty cluster.
 * 
 * @param *c Pointer to the cluster_t structure
 * @pre c != NULL
 */
void clear_cluster(struct cluster_t *c);

/** Chunk of clusters objects. Value recommended for reallocation. */
extern const int CLUSTER_CHUNK;

/**
 * @brief Resizing the cluster.
 *
 * This function changes capacity of the cluster to the new capacity.
 *
 * @param *c Pointer to the cluster_t structure
 * @param new_cap Integer value, represents new value of capacity of the cluster
 * @return struct Structure of cluster_t with resized capacity
 *
 * @pre new_cap > 0
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief Adding new object to cluster.
 *
 * Adds object at the end of the cluster. If required, cluster is resized.
 *
 * @param *c Pointer to the cluster_t structure
 * @param obj Structure of obj_t 
 * @pre c != NULL, obj != NULL
 * @return struct *cluster_t
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief Merging the clusters.
 *
 * This function merges two clusters. 
 *
 * @param *c1 Pointer to the cluster_t structure, cluster number 1
 * @param *c2 Pointer to the cluster_t structure, cluster number 2
 * @pre c1 != NULL, c2 != NULL
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Removal of cluster.
 *
 * Removes cluster from array of clusters. This function returns new number of clusters.
 *
 * @param *carr Pointer to the cluster_t array of clusters
 * @param narr Integer value, means number of clusters array
 * @param idx Integer value, represents the index of cluster meant to be removed
 * @return Integer value as a new number of clusters.
 *
 * @pre narr > 0, idx > 0, carr != NULL
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @brief Counting the distance between two objects.
 *
 * This function compute distance between two objects via Euklidean distance formula.
 *
 * @code
 * float dist = 0.0;
 *
 * dist = sqrtf((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
 *
 * return dist;
 * @endcode
 * @note x1, x2 and y1, y2 are the coordinates of the objects.
 *
 * @param *o1 Pointer to the obj_t structure, object number 1
 * @param *o2 Pointer to the obj_t strucure, object number 2
 *
 * @return Floating point number representing the distance between two objects.
 *
 * @pre *o1 > 0, *o2 > 0 
 * @post Counts the distance or returns 0.0 when an error occured.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief Counting the distance between two clusters.
 *
 * This function compute distance between two clusters. From this function is also called obj_distance() function.
 *
 * @param *c1 Pointer to the cluster_t structure, cluster number 1
 * @param *c2 Pointer to the cluster_t structure, cluster number 2
 * @return Floating point number representing the distance between two clusters
 *
 * @pre c1 != NULL, c2 != NULL
 * @post returned float > 0.0
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Finding the neighbours.
 *
 * Finds the nearest clusters. In this algorithm is used single linkage method of clustering. 
 *
 * @param *carr Pointer to the cluster_t array of clusters
 * @param narr Integer value, represents the size of cluster (how many objects are in the cluster)
 * @param *c1 Pointer to the integer, represents the index of the cluster number 1
 * @param *c2 Pointer to the integer, represents the index of the cluster number 2
 *
 * On the address of *c1 is saved index of the first cluster and on the address of *c2 is saved index of second cluster.  
 *
 * @pre 0 < narr
 * @post 0 <= *c1 < *c2 < narr
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @brief Sort ordering of clusters.
 *
 * This function sorts the objects inside the cluster ascending by ID.
 *
 * @param *c Pointer to the cluster_t structure
 * @pre c != NULL
 */
void sort_cluster(struct cluster_t *c);

/**
 * @brief Printing the  cluster.
 *
 * Prints cluster on the standard output.
 * 
 * @param *c Pointer to the cluster_t structure
 * @pre c != NULL
 */
void print_cluster(struct cluster_t *c);

/**
 * @brief Reading the objects from file.
 *
 * Loads objects from file. There will be created cluster for every single loaded object. After that, all of the objects will be saved to its attributable cluster. If any error occured, to the pointer of pointer arr will be assigned value NULL.
 *
 * @param *filename Pointer to the character, represents the name of accessed file
 * @param **arr Pointer to the pointer to the cluster_t structure, means the cluster array
 * @return Integer value as a number of successfully loaded objects
 * @pre filename != NULL 
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @brief Printing the array of clusters.
 *
 * It prints only array of clusters. 
 * @param *carr Pointer to the cluster_t structure, means cluster array
 * @param narr Integer value, represents number of cluster array to be printed
 *
 * @pre narr > 0, carr != NULL
 */
void print_clusters(struct cluster_t *carr, int narr);
/** @} */
