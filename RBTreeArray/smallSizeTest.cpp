#include <map>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include "PCG32.h"
#include "RBTreeArray.h"

#define REPEAT 1000
#define CASE_SIZE 10000

static inline void swap(unsigned* array,unsigned index0,unsigned index1){
	unsigned temporary=array[index1];
	array[index1]=array[index0];
	array[index0]=temporary;
}

void smallSizeTest(PCG32Struct* PCGStatus){
	unsigned* keys32=(unsigned*)malloc(CASE_SIZE*sizeof(unsigned));
	unsigned* values32=(unsigned*)malloc(CASE_SIZE*sizeof(unsigned));

	for(unsigned index=0;index<CASE_SIZE;index=index+1){
		keys32[index]=PCG32(PCGStatus);
		values32[index]=index+1;
	}

	RBTree* RBTree32=RBTree32CreateSize(CASE_SIZE);
	std::map<unsigned,unsigned>* stdMap32[REPEAT];
	for(unsigned indexRepeat=0;indexRepeat<REPEAT;indexRepeat=indexRepeat+1){
		stdMap32[indexRepeat]=new std::map<unsigned,unsigned>;
	}
	struct timeval start,end;
	unsigned insertTime[2];
	unsigned searchTime[2];
	unsigned eraseTime[2];
	unsigned clearTime[2];

	gettimeofday(&start,NULL);
	for(unsigned indexRepeat=0;indexRepeat<REPEAT;indexRepeat=indexRepeat+1){
		RBTreeClear(RBTree32);
		for(unsigned index=0;index<CASE_SIZE;index=index+1){
			RBTreeInsert(&RBTree32,keys32[index],values32[index]);
		}
	}
	gettimeofday(&end,NULL);
	insertTime[0]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;

	gettimeofday(&start,NULL);
	for(unsigned indexRepeat=0;indexRepeat<REPEAT;indexRepeat=indexRepeat+1){
		for(unsigned index=0;index<CASE_SIZE;index=index+1){
			stdMap32[indexRepeat]->insert(std::make_pair(keys32[index],values32[index]));
		}
	}
	gettimeofday(&end,NULL);
	insertTime[1]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;

	for(unsigned index=0;index<CASE_SIZE;index++){
		swap(keys32,index,PCG32(PCGStatus)%(CASE_SIZE-index)+index);
	}

	gettimeofday(&start,NULL);
	for(unsigned indexRepeat=0;indexRepeat<REPEAT;indexRepeat=indexRepeat+1){
		for(unsigned index=0;index<CASE_SIZE;index=index+1){
			static long long unsigned int value;
			RBTreeSearch(RBTree32,keys32[index],&value);
		}
	}
	gettimeofday(&end,NULL);
	searchTime[0]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;

	gettimeofday(&start,NULL);
	for(unsigned indexRepeat=0;indexRepeat<REPEAT;indexRepeat=indexRepeat+1){
		for(unsigned index=0;index<CASE_SIZE;index=index+1){
			RBTreeDelete(RBTree32,keys32[index]);
		}
		for(unsigned index=0;index<CASE_SIZE;index=index+1){
			RBTreeInsert(&RBTree32,keys32[index],values32[index]);
		}
	}
	gettimeofday(&end,NULL);
	eraseTime[0]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;
	eraseTime[0]=eraseTime[0]-insertTime[0];

	gettimeofday(&start,NULL);
	for(unsigned indexRepeat=0;indexRepeat<REPEAT;indexRepeat=indexRepeat+1){
		for(unsigned index=0;index<CASE_SIZE;index=index+1){
			volatile long long unsigned int value;
			value=stdMap32[indexRepeat]->find(keys32[index])->second;
		}
	}
	gettimeofday(&end,NULL);
	searchTime[1]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;

	gettimeofday(&start,NULL);
	for(unsigned indexRepeat=0;indexRepeat<REPEAT;indexRepeat=indexRepeat+1){
		for(unsigned index=0;index<CASE_SIZE;index=index+1){
			stdMap32[indexRepeat]->erase(keys32[index]);
		}
	}
	gettimeofday(&end,NULL);
	eraseTime[1]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;
	eraseTime[1]=eraseTime[1];

	printf("RBTree32 %u size:\n",CASE_SIZE);
	printf("RBTreeInsert(): %u milliseconds\n",insertTime[0]);
	printf("std::map::insert: %u milliseconds\n",insertTime[1]);
	printf("RBTreeSearch(): %u milliseconds\n",searchTime[0]);
	printf("std::map::find(): %u milliseconds\n",searchTime[1]);
	printf("RBTreeDelete(): %u milliseconds\n",eraseTime[0]);
	printf("std::map::erase(): %u milliseconds\n",eraseTime[1]);
	printf("\n");
	for(unsigned indexRepeat=0;indexRepeat<REPEAT;indexRepeat=indexRepeat+1){
		stdMap32[indexRepeat]->clear();
	}
	RBTreeDestroy(&RBTree32);
}

int main(int argc, char const *argv[]){
	PCG32Struct PCGStatus;
	PCGStatus.seed=time(NULL);
	PCGStatus.seed=1;
	PCG32Init(&PCGStatus);

	smallSizeTest(&PCGStatus);
	return 0;
}