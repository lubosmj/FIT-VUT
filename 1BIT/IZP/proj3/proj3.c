#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

#ifdef NDEBUG

#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)

#else

#define debug(s) printf("- %s\n", s)

#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

struct obj_t {
	int id;
	float x;
	float y;
};

struct cluster_t {
	int size;
	int capacity;
	struct obj_t *obj;
};

void init_cluster(struct cluster_t *c, int cap){
	assert(c != NULL);
	assert(cap >= 0);

	c->size = 0;
	if (cap == 0){ 
		c->capacity = 0; 
		return;
	}
	c->obj = malloc(sizeof(struct obj_t)*cap);
	if (c->obj == NULL){
		c->capacity = 0;
		return;
	}
	c->capacity = cap;
}

void clear_cluster(struct cluster_t *c){
	assert(c != NULL);

	free(c->obj);
	init_cluster(c,0);
}

const int CLUSTER_CHUNK = 10;

struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap){
	assert(c);
	assert(c->capacity >= 0);
	assert(new_cap >= 0);

	if (c->capacity >= new_cap)
		return c;

	size_t size = sizeof(struct obj_t) * new_cap;

	void *arr = realloc(c->obj, size);
	if (arr == NULL)
		return NULL;

	c->obj = arr;
	c->capacity = new_cap;
	return c;
}

void append_cluster(struct cluster_t *c, struct obj_t obj){
	assert(c != NULL);

	if (c->size == c->capacity) {
		c = resize_cluster(c, c->capacity + CLUSTER_CHUNK);
	}	
	int size = c->size;
	c->obj[size].id = obj.id;
	c->obj[size].x = obj.x;
	c->obj[size].y = obj.y;
	c->size = c->size + 1;
}

void sort_cluster(struct cluster_t *c);

void merge_clusters(struct cluster_t *c1, struct cluster_t *c2){
	assert(c1 != NULL);
	assert(c2 != NULL);

	for (int i = 0; i < c2->size; i++){
		append_cluster(c1, c2->obj[i]);
	}
	sort_cluster(c1);
}

int remove_cluster(struct cluster_t *carr, int narr, int idx){
	assert(idx < narr);
	assert(narr > 0);

	clear_cluster(&carr[idx]);

	for (int i = idx; i < (narr - 1); i++){
		carr[i] = carr[i + 1];
	}

	return narr-1;
}

float obj_distance(struct obj_t *o1, struct obj_t *o2){
	assert(o1 != NULL);
	assert(o2 != NULL);

	float dist = 0.0;
	float x1 = o1->x, y1 = o1->y;
	float x2 = o2->x, y2 = o2->y;
	dist = sqrtf((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

	return dist;
}

float cluster_distance(struct cluster_t *c1, struct cluster_t *c2){
	assert(c1 != NULL);
	assert(c1->size > 0);
	assert(c2 != NULL);
	assert(c2->size > 0);

	float dist1 = 0.0;
	float dist_c = 1415.0;

	for (int i = 0; i < c1->size; i++){
		for (int j = 0; j < c2->size; j++){
			dist1 = obj_distance(&c1->obj[i], &c2->obj[j]);
			if (dist1 < dist_c)
				dist_c = dist1;
		}
	}

	return dist_c;
}

void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2){
	assert(narr > 0);

	float dist_n = 1415.0;
	float dist1 = 0.0;

	for (int i = 0; i < narr; i++){
		for (int j = i + 1; j < narr; j++){
			dist1 = cluster_distance(&carr[i], &carr[j]);
			if (dist1 < dist_n){
				dist_n = dist1;
				*c1 = i;
				*c2 = j;
			}
		}
	}
}

static int obj_sort_compar(const void *a, const void *b){
	const struct obj_t *o1 = a;
	const struct obj_t *o2 = b;
	if (o1->id < o2->id) return -1;
	if (o1->id > o2->id) return 1;
	return 0;
}

void sort_cluster(struct cluster_t *c){
	qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

void print_cluster(struct cluster_t *c){
	for (int i = 0; i < c->size; i++){
		if (i) putchar(' ');
		printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
	}
	putchar('\n');
}

int load_clusters(char *filename, struct cluster_t **arr){
	assert(arr != NULL);

	char id[10], x[10], y[10], temp_in[20];
	char *temp1, *temp2, *temp3;
	char ca[100];
	float x_d, y_d;
	long long id_d;
	int count=-5;
	int scan_for;
	FILE *fp = fopen(filename,"r");
	if (fp == NULL){
		arr = NULL;
		return -1;
	}

	int scan = fscanf(fp, "count=%15s", temp_in);
	long long count_d = strtol(temp_in, &temp1, 10);
	if (scan != 1 || count_d > INT_MAX || count_d <= 0 || *temp1 != '\0'){
		fclose(fp);
		return -2;
	}

	count = (int)count_d;
	for (int i = 0; i < count; i++){
		//how does this work ? 
		scan_for = fscanf(fp, "%5s %5s %5s%[^\n]", id, x, y, ca);
		if (scan_for != 3){
			fclose(fp);
			return -3;
		}

		id_d = strtol(id, &temp1, 10);
		x_d = strtof(x, &temp2);
		y_d = strtof(y, &temp3);

		if ((id_d > INT_MAX || id_d < 0) || (x_d > 1000 || x_d < 0) || (y_d > 1000 || y_d < 0) 
				|| *temp1 != '\0' || *temp2 != '\0' || *temp3 != '\0'){
			fclose(fp);
			return -3;
		}
	}

	*arr = malloc (sizeof(struct cluster_t) * count);
	if (arr == NULL){
		fclose(fp);
		return -4;
	}
	char buffer[100];
	rewind(fp);
	fgets(buffer, 100, fp);
	for (int i = 0; i < count; i++){
		init_cluster(&(*arr)[i],1);
		fscanf(fp, "%d %f %f", &((&(*arr)[i])->obj)->id, &((&(*arr)[i])->obj)->x, &((&(*arr)[i])->obj)->y);
		(&(*arr)[i])->size = 1;
	}
	fclose(fp);
	return count;
}

void print_clusters(struct cluster_t *carr, int narr){
	printf("Clusters:\n");
	for (int i = 0; i < narr; i++){
		printf("cluster %d: ", i);
		print_cluster(&carr[i]);
	}
}

int main(int argc, char *argv[]){
	struct cluster_t *clusters = NULL;
	double num_d =0;
	int num_cl = 1, a = 0, b = 0;
	char *temp;

	if (argc >= 2 && argc < 4){
		if (argc == 3){
			num_d = strtod (argv[2], &temp);
			if (num_d <= 0 || num_d > INT_MAX || *temp != '\0'){
				fprintf(stderr, "Error: Invalid argument entered!\n");
				return 1;
			}
			num_cl = (int)num_d;
		}
		int c = load_clusters(argv[1],&clusters);
		switch (c){
			case -1:
				fprintf(stderr, "Error: Cannot access this file!\n");
				return 1;
				break;
			case -2:
				fprintf(stderr, "Error: Wrong format or number of objects which are to be counted!\n");
				return 1;
				break;
			case -3: 
				fprintf(stderr, "Error: The coordinates of the object or ID does not meet the rules of the program!\n");
				return 1;
				break;
			case -4:
				fprintf(stderr, "Error: Cannot allocate memory for this file!\n");
				return 1;
				break;
			case -5:
				fprintf(stderr, "Error while loading the objects!\n");
				return 1;
				break;
			default:
				while (c > num_cl){
					find_neighbours(clusters, c, &a, &b);
					merge_clusters(&clusters[a], &clusters[b]);
					c = remove_cluster(clusters, c, b);
				}
				print_clusters(clusters, c);

				for (int i = c; i > 0; i = remove_cluster(clusters, i, 0));

				free(clusters);
		}
	}
	else {
		fprintf(stderr, "Error: Not enough or too much arguments entered!\n");
		return 1;
	}
	return 0;
}
