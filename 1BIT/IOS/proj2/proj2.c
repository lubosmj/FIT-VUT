#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#include "proj2.h"

int *num_process; 			//number of process
int	*num_passenger;    		//number of passenger process
int	*num_ent_passengers;	//number of passengers who entered car

sem_t *sem_car;				//semaphore of car 
sem_t *sem_load;			//semaphore to handle load into car
sem_t *sem_unload;			//semaphore to handle unload from car
sem_t *sem_finish;			//semaphore to check final status of process
sem_t *sem_full;			//semaphore to handle full capacity of car
sem_t *sem_start;			//semaphore to check start of ride
sem_t *sem_write;			//semaphore to handle synchronized writing to file

//identifiers to shared memory segments
int shm_num_process;
int	shm_num_passenger;
int	shm_num_ent_passengers;

//get random number from given interval
long rand_int(long max) {

	long num;			//holds randomed number

	unsigned long num_bins = (unsigned long) max + 1;
	unsigned long num_rand = (unsigned long) RAND_MAX + 1;
	unsigned long bin_size = num_rand / num_bins;
	unsigned long defect   = num_rand % num_bins;

	do {
		num = random();
	}
	while (num_rand - defect <= (unsigned long)num);

	//return truncated division of randomed number
	return num/bin_size;
}

//car process
void car(params_t *param, FILE *fp){

	int time_ride = 0;   	//time of car ride
	int num_car = 1;		//number of car
	int i = 0;				//counter of rides

	//number of rides
	int number_ride = (int)param->P/param->C;  

	sem_wait(sem_write);
	fprintf(fp,"%d\t: C %d\t: started\n",(*num_process)++, num_car);
	sem_post(sem_write);

	//ride until number of rides is reached
	while(i < number_ride){

		sem_wait(sem_start);

		//load passengers
		if((*num_ent_passengers) == 0){

			sem_wait(sem_write);
			fprintf(fp,"%d\t: C %d\t: load\n",(*num_process)++, num_car);
			sem_post(sem_write);
			sem_post(sem_load);
		}
		sem_wait(sem_full);

		//start ride if the car filled its maximum capacity
		if(*num_ent_passengers == param->C){

			sem_wait(sem_load);
			sem_wait(sem_write);
			fprintf(fp,"%d\t: C %d\t: run\n",(*num_process)++, num_car);
			sem_post(sem_write);
			
			//generate random time of ride
			if(param->RT != 0){
				time_ride = rand_int(param->RT);
			}
			usleep(time_ride * 1000);
			++i;
			
			//end ride
			sem_wait(sem_write);
			fprintf(fp,"%d\t: C %d\t: unload\n",(*num_process)++, num_car);
			sem_post(sem_write);
			sem_post(sem_unload);
		}
	}

	//handle ending of car process
	if(i == number_ride){
		//end of all rides
		sem_wait(sem_start);
		sem_wait(sem_write);
		fprintf(fp,"%d\t: C %d\t: finished\n",(*num_process)++, num_car);
		sem_post(sem_write);
		sem_post(sem_finish);
	}
	//exit process
	exit(0);
}

//passenger process
void passenger(params_t *param, FILE *fp){

	int lok_num_passenger = *num_passenger; 

	sem_wait(sem_write);
	fprintf(fp,"%d\t: P %d\t: started\n",(*num_process)++, lok_num_passenger);
	fflush(fp);
	//increment number of passengers
	++(*num_passenger);
	sem_post(sem_write);

	//wait until the car process is loading 
	sem_wait(sem_load);
	sem_wait(sem_car);
	sem_wait(sem_write);
	fprintf(fp,"%d\t: P %d\t: board\n",(*num_process)++, lok_num_passenger);
	sem_post(sem_write);
	//increment counter of entered passengers
	++(*num_ent_passengers);
	
	//board last passenger
	if((*num_ent_passengers) == param->C){

		sem_wait(sem_write);
		fprintf(fp,"%d\t: P %d\t: board last\n",(*num_process)++, lok_num_passenger);
		sem_post(sem_write);
		sem_post(sem_load); 
		sem_post(sem_full);
	}

	//board passengers in given order and print that order
	if((*num_ent_passengers) < param->C){

		sem_wait(sem_write);
		fprintf(fp,"%d\t: P %d\t: board order %d\n",(*num_process)++, lok_num_passenger, *num_ent_passengers);
		sem_post(sem_write);
		sem_post(sem_load);
	}
	
	//unboard passengers
	sem_wait(sem_unload);
	sem_wait(sem_write);
	fprintf(fp,"%d\t: P %d\t: unboard\n",(*num_process)++, lok_num_passenger);
	sem_post(sem_write);
	sem_post(sem_car);
	--(*num_ent_passengers);
	
	//unboard last passenger
	if((*num_ent_passengers) == 0){

		sem_wait(sem_write);
		fprintf(fp,"%d\t: P %d\t: unboard last\n",(*num_process)++, lok_num_passenger);
		sem_post(sem_write);
		sem_post(sem_start);
	}

	//unboard passengers in given order and print that order
	if((*num_ent_passengers) > 0){

		sem_wait(sem_write);
		fprintf(fp,"%d\t: P %d\t: unboard order %d\n",(*num_process)++, lok_num_passenger, -(*num_ent_passengers-param->C));
		sem_post(sem_write);
		sem_post(sem_unload);
	}
	
	//wait for finish and end process
	
	sem_wait(sem_finish);
	sem_wait(sem_write);
	fprintf(fp,"%d\t: P %d\t: finished\n",(*num_process)++, lok_num_passenger);
	sem_post(sem_write);
	sem_post(sem_finish);
	//exit process
	exit(0);
}

//load shared memory and semaphores
void load_resources(params_t *param){

	//create new mapping in virtual adress space for sempahores

	if((sem_car = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED){
		//handle error
		error("An error occured while creating new mapping in virtual adress space",2);
	}
	if((sem_load = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED){
		//handle error
		error("An error occured while creating new mapping in virtual adress space",2);
	}
	if((sem_unload = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED){
		//handle error
		error("An error occured while creating new mapping in virtual adress space",2);
	}
	if((sem_finish = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED){
		//handle error
		error("An error occured while creating new mapping in virtual adress space",2);
	}
	if((sem_full = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED){
		//handle error
		error("An error occured while creating new mapping in virtual adress space",2);
	}
	if((sem_start = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED){
		//handle error
		error("An error occured while creating new mapping in virtual adress space",2);
	}
	if((sem_write = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED){
		//handle error
		error("An error occured while creating new mapping in virtual adress space",2);
	}

	//initialize all semaphores with particular initial value

	if(sem_init(sem_car, 1, param->C) == -1){
		//handle error
		error("An error occured while initializing a semaphore",2);
	}
	if(sem_init(sem_load, 1, 0) == -1){
		//handle error
		error("An error occured while initializing a semaphore",2);
	}
	if(sem_init(sem_unload, 1, 0) == -1){
		//handle error
		error("An error occured while initializing a semaphore",2);
	}
	if(sem_init(sem_finish, 1, 0) == -1){
		//handle error
		error("An error occured while initializing a semaphore",2);
	}
	if(sem_init(sem_full, 1, 0) == -1){
		//handle error
		error("An error occured while initializing a semaphore",2);
	}
	if(sem_init(sem_start, 1, 1) == -1){
		//handle error
		error("An error occured while initializing a semaphore",2);
	}
	if(sem_init(sem_write, 1, 1) == -1){
		//handle error
		error("An error occured while initializing a semaphore",2);
	}

	//create shared memory segment 

	if((shm_num_process = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1){
		//handle error
		error("An error occured while allocating shared memory segment",2);
	}
	if((shm_num_ent_passengers = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1){
		//handle error
		error("An error occured while allocating shared memory segment",2);
	}
	if((shm_num_passenger = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1){
		//handle error
		error("An error occured while allocating shared memory segment",2);
	}

	//attatch created shared memory segment to particular shared memory ID

	if((num_process = shmat(shm_num_process, NULL, 0)) == NULL){
		//handle error
		error("An error occured while allocating shared memory segment",2);
	}
	if((num_ent_passengers = shmat(shm_num_ent_passengers, NULL, 0)) == NULL){
		//handle error
		error("An error occured while allocating shared memory segment",2);
	}
	if((num_passenger = shmat(shm_num_passenger, NULL, 0)) == NULL){
		//handle error
		error("An error occured while allocating shared memory segment",2);
	}
}

//delete all allocated space and shared memory
void free_resources(void){

	//destroy all created semaphores
	sem_destroy(sem_write);
	sem_destroy(sem_car);
	sem_destroy(sem_load);
	sem_destroy(sem_unload);
	sem_destroy(sem_finish);
	sem_destroy(sem_full);
	sem_destroy(sem_start);

	//destroy segment of shared memory	
	shmctl(shm_num_process, IPC_RMID, 0);
	shmctl(shm_num_passenger, IPC_RMID, 0);
	shmctl(shm_num_ent_passengers, IPC_RMID, 0);

	//detatch shared memory segment 
	shmdt(num_process);
	shmdt(num_passenger);
	shmdt(num_ent_passengers);
}

//force exit running processes and free memory
void force_exit(){

	//call function to free all allocated space
	free_resources();
	//kill process
	kill(getpid(), SIGTERM);
	//exit with exit code 2
	exit(2);
}

//handle and create all processes 
void handle_processes(params_t *param, FILE *fp){

	pid_t pid_car;			//id of car process
	pid_t pid_passenger;	//id of passenger process

	//create an array of child processes
	int children[param->P];

	*num_ent_passengers = 0;	//zero passengers have already entered a car
	*num_passenger = 1;
	*num_process = 1;

	//set new sequence of pseudo random numbers
	srand(time(0));

	//create new process
	pid_car = fork();

	//handle parrent process
	if(pid_car == 0){
		//call car process
		car(param, fp);
	}
	//handle child process
	else if (pid_car > 0){
		//create new passenger processes
		for(int i = 0; i < param->P; ++i){
			//wait for new creation
			usleep(rand_int(param->PT) * 1000);
			//create new passenger
			pid_passenger = fork();
			//handle passenger process
			if(pid_passenger == 0){
				//call passenger process
				passenger(param, fp);
			}  
			else if (pid_passenger > 0){
				//remeber ID of child process
				children[i] = pid_passenger;
			}
			else{
				//handle error
				fprintf(stderr, "An error in system call occured!\n");
				force_exit();
			}
		}
	}
	else {
		//handle error
		fprintf(stderr, "An error in system call occured!\n");
		force_exit();
	}

	//wait until all children have sucessfully ended
	for(int i = 0; i < param->P; ++i){
		//wait for particular child process
		waitpid(children[i], NULL, 0);
	}
}

void load_parameters(int argc, char **argv, params_t *param){

	//number of passengers, car capacity, process creation time and ride time
	int P, C, PT, RT;
	//temporary variable
	char *tmp;

	if (argc == 5){
		//store number of passengers
		P = strtol(*(argv + 1), &tmp, 10);

		//check if number of passengers contains any alpha 
		//characters or is more than zero
		if ((*tmp != '\0') || (P <= 0)){
			//handle error
			error("Wrong number of passengers entered",1);
		}
		//store capacity of car
		C = strtol(*(argv + 2), &tmp, 10);

		//check if car capacity is more than zero, is multiple
		//of number of passengers and does not contain any alpha characters
		if ((*tmp != '\0') || (C <= 0) || (P % C != 0) || ( P <= C)){
			//handle error
			error("Wrong number of car capacity entered",1);
		}
		//store max generation time of passenger process
		PT = strtol(*(argv + 3), &tmp, 10);

		//check if time is more or equal to zero and is less than 5001 
		//by definition and if contains any alpha characters
		if ((*tmp != '\0') || (PT < 0) || (PT >= 5001)){
			//handle error
			error("Wrong max process generation time entered",1);
		}
		//store max time of car ride
		RT = strtol(*(argv + 4), &tmp, 10);

		//check if time of ride is more or equal to zero
		//or is less than 5001 by definition
		if ((*tmp != '\0') || (RT < 0) || (RT >= 5001)){
			//handle error
			error("Wrong max time of car ride entered",1);
		}
	}
	//if number of arguments is less or more than 5,
	//error message will be printed
	else {
		error("Wrong number of arguments entered",1);
	}

	//save arguments to structure
	param->P = P;
	param->C = C;
	param->PT = PT;
	param->RT = RT;
}

int main(int argc, char **argv){

	//handle signals
	signal(SIGINT, force_exit);
	signal(SIGTERM, force_exit);

	//allcate memory for pointer to structure
	params_t *param = malloc(sizeof(params_t)); 

	//open file
	FILE *fp = fopen("proj2.out", "w+");
	//check if file was opened successfully
	if (fp == NULL){
		//handle error
		perror("Error");
		exit(1);
	}
	//turn off buffering 
	setbuf(fp, NULL);

	//load parameters as asrguments
	load_parameters(argc, argv, param);
	//load arguments to parameters
	load_resources(param);
	//create and handle processes	
	handle_processes(param, fp);

	//free all shared memory and allocated space for semaphores	
	free_resources();
	//free allocated space for pointer to structure
	free(param);
	//close file
	fclose(fp);
	//exit with exit code 0
	exit(0);
}
