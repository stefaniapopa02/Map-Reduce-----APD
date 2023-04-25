#include <iostream>
#include <fstream>
#include <pthread.h>
#include <queue>
#include <set>
#include <cmath>

typedef struct thread_params{
    int id;
    int noOfMappers;
    int noOfReducers;
    int noOfFiles;
    std::queue<std::string> *fileNames;
    pthread_mutex_t *mutexAddr;
    pthread_barrier_t *barrierAddr;
    std::vector<long> **perfectPowers;

}thread_params;

bool mapperFunc(int n, int noOfReducers, int e, int maxValue){
    int left = 0;
    int right = maxValue;

    int middle;
    int value;

    if(n == 0) {
        return false;
    }

    if(n == 1) {
        return true;
    }

    while(left <= right){
        middle = (left + right) / 2;
        value = pow(middle, e);

        if(value == n){
            return true;
        }else if(value < n){
            left = middle + 1;
        }else{
            right = middle - 1;
        }

    }
    return false;

}

void *f(void *args){
    thread_params * params = (thread_params*)args;

    int id = params->id;
    int noOfMappers = params->noOfMappers;
    int noOfReducers = params->noOfReducers;
    pthread_mutex_t *mutexAddr = params->mutexAddr;
    pthread_barrier_t *barrierAddr = params->barrierAddr;
    std::queue<std::string> *fileNames = params->fileNames;

    std::string fileName;
    std::ifstream inputFile;
    int no;
    std::vector<int> numbers;
    int crtNr;

    std::string outFileName;
    std::ofstream outputFile;

    if(id < noOfMappers)
    {
        int maxValue = sqrt(INT32_MAX);

        for(int e = 2; e < noOfReducers + 2; e++) {
            while(pow(maxValue, e) > INT32_MAX) {
                maxValue--;
            }
            numbers.push_back(maxValue);
        }

        while(fileNames->empty() == false){
            pthread_mutex_lock(mutexAddr);
            fileName = fileNames->front();
            fileNames->pop();
            pthread_mutex_unlock(mutexAddr);

            inputFile.open(fileName);

            inputFile >> no;

            for(int i = 0; i < no; i++){
                inputFile >> crtNr;

                for(int e = 2; e < noOfReducers + 2; e++){
                    bool res = mapperFunc(crtNr, noOfReducers, e, numbers[e - 2]);
                    if(res) {
                        params->perfectPowers[id][e].push_back(crtNr);
                    }
                }

            }
            inputFile.close();
        }
    }

    pthread_barrier_wait(barrierAddr);
    
    if(id >= noOfMappers) {

        std::set<long> setOfPerfectPowers;

        int ee = id - noOfMappers + 2;

        for(int i = 0; i < noOfMappers; i++) {
            for(int j = 0; j < params->perfectPowers[i][ee].size(); j++) {
                setOfPerfectPowers.insert(params->perfectPowers[i][ee].at(j));
            }
        
        }
        
        outFileName = "out" + std::to_string(ee) + ".txt";
        outputFile.open(outFileName);
        outputFile << setOfPerfectPowers.size();
        outputFile.close();                                                                                               

    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    int noOfMappers;
    int noOfReducers;
    std::string inputFileName;

    std::ifstream inputFile;
    int noOfFiles;
    std::string forRead;
    std::queue<std::string> *fileNames = new std::queue<std::string>(); 

    int r;
    int totalThreads;
    void *status;

    pthread_mutex_t mutex;
    pthread_barrier_t barrier;

    if(argc < 4){
        std::cout <<"Error : we need 3 arguments \n";
        return -1;
    }

   
    noOfMappers = atoi(argv[1]);
    noOfReducers = atoi(argv[2]);
    totalThreads = noOfMappers + noOfReducers;
    inputFileName = argv[3];

    std::vector<long> **perfectPowers = new std::vector<long>*[noOfMappers];
    for(int i = 0; i < noOfMappers; i++) {
        perfectPowers[i] = new std::vector<long>[noOfReducers + 2];
    }

    for(int i = 0; i < noOfMappers; i++) {
        for(int j = 0; j < noOfReducers + 2; j++) {
            perfectPowers[i][j].reserve(100);
        }
    }

    pthread_t threads[totalThreads];
    thread_params threadsParams[totalThreads] ;

    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, noOfMappers + noOfReducers);

    inputFile.open(inputFileName);

    inputFile >> noOfFiles;

    for(int i = 0; i < noOfFiles; i++){
        inputFile >> forRead;
        fileNames->push(forRead);   
    }

    for(int i = 0; i < totalThreads; i++){
        threadsParams[i].id = i;
        threadsParams[i].noOfMappers = noOfMappers;
        threadsParams[i].noOfReducers = noOfReducers;
        threadsParams[i].fileNames = fileNames;
        threadsParams[i].mutexAddr = &mutex;
        threadsParams[i].barrierAddr = &barrier;
        threadsParams[i].perfectPowers = perfectPowers;

        r = pthread_create(&threads[i],NULL, f, (void *)&threadsParams[i]);
        if (r) {
            printf(" Error creating thread %d\n", i);
            exit(-1);
        }

    }

    for(int i = 0; i < totalThreads; i++){
        r = pthread_join(threads[i], &status);
        if (r) {
            printf(" Error waiting for thread %d\n", i);
            exit(-1);
        }
    }

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);

    delete(fileNames);

}