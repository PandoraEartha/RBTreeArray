#include <map>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include "PCG32.h"
#include "RBTreeArray.h"

#define TEST_CASE (1<<26)
#define THREAD_NUMBER 32

long long unsigned int* keys;
unsigned* values;

static inline void swap(long long unsigned int* array,unsigned index0,unsigned index1){
	long long unsigned int temporary=array[index1];
	array[index1]=array[index0];
	array[index0]=temporary;
}

template<typename Map>
static inline void check(const RBTree* tree,const Map& map){
	if(RBTreeKeyCount(tree)!=map.size()){
		printf("RBTree%llu insert failed\n",tree->bitLength);
		return;
	}
	for(auto [key,value]: map){
		long long unsigned int valueSearch;
		if(RBTreeSearch(tree,key,&valueSearch)){
			if(value!=valueSearch){
				printf("RBTree%llu search failed\n",tree->bitLength);
				return;
			}
		}else{
			printf("RBTree%llu search failed\n",tree->bitLength);
			return;
		}
	}
	printf("RBTree%llu check pass\n",tree->bitLength);
}

template<typename Map>
static inline void timeConsume(PCG32Struct* PCGStatus,RBTree** treePointer,Map& map){
	struct timeval start,end;
	unsigned insertTime[2];
	unsigned searchTime[2];
	unsigned eraseTime[2];
	unsigned clearTime[2];
	long long unsigned int bitLength=(*(treePointer))->bitLength;

    for(unsigned index=0;index<TEST_CASE;index++){
		keys[index]=(((long long unsigned int)PCG32(PCGStatus))<<32)+PCG32(PCGStatus);
		values[index]=index;
	}

	gettimeofday(&start,NULL);
	for(unsigned index=0;index<TEST_CASE;index++){
		RBTreeInsert(treePointer,keys[index],values[index]);
	}
	gettimeofday(&end,NULL);
	insertTime[0]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;

	gettimeofday(&start,NULL);
	for(unsigned index=0;index<TEST_CASE;index++){
		static long long unsigned int value;
		RBTreeSearch(*(treePointer),keys[index],&value);
	}
	gettimeofday(&end,NULL);
	searchTime[0]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;
	gettimeofday(&start,NULL);
	for(unsigned index=0;index<TEST_CASE;index++){
		RBTreeDelete(*(treePointer),keys[index]);
	}
	gettimeofday(&end,NULL);
	eraseTime[0]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;
	
	for(unsigned index=0;index<TEST_CASE;index++){
		RBTreeInsert(treePointer,keys[index],values[index]);
	}
	gettimeofday(&start,NULL);
	RBTreeClear(*(treePointer));
	RBTreeDestroy(treePointer);
	gettimeofday(&end,NULL);
	clearTime[0]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;
	gettimeofday(&start,NULL);
	for(unsigned index=0;index<TEST_CASE;index++){
		map.insert(std::make_pair(keys[index],values[index]));
		// map[keys[index]]=values[index];
	}
	gettimeofday(&end,NULL);
	insertTime[1]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;
	
	for(unsigned index=0;index<TEST_CASE;index++){
		swap(keys,index,PCG32(PCGStatus)%(TEST_CASE-index)+index);
	}
	
	gettimeofday(&start,NULL);
	for(unsigned index=0;index<TEST_CASE;index++){
		volatile long long unsigned int value;
		value=map.find(keys[index])->second;
	}
	gettimeofday(&end,NULL);
	searchTime[1]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;
	

	gettimeofday(&start,NULL);
	for(unsigned index=0;index<TEST_CASE;index++){
		map.erase(keys[index]);
	}
	gettimeofday(&end,NULL);
	eraseTime[1]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;
	
	for(unsigned index=0;index<TEST_CASE;index++){
		map[keys[index]]=values[index];
	}

	gettimeofday(&start,NULL);
	map.clear();
	gettimeofday(&end,NULL);
	clearTime[1]=(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0+0.5;
	
	printf("RBTree%llu:\n",bitLength);
	printf("RBTreeInsert(): %u milliseconds\n",insertTime[0]);
	printf("std::map overload []: %u milliseconds\n",insertTime[1]);
	printf("RBTreeSearch(): %u milliseconds\n",searchTime[0]);
	printf("std::map::find(): %u milliseconds\n",searchTime[1]);
	printf("RBTreeDelete(): %u milliseconds\n",eraseTime[0]);
	printf("std::map::erase(): %u milliseconds\n",eraseTime[1]);
	printf("RBTreeClear(): %u milliseconds\n",clearTime[0]);
	printf("std::map::clear(): %u milliseconds\n",clearTime[1]);
	printf("\n");
}

void test(PCG32Struct* PCGStatus){
	unsigned* keys=(unsigned*)malloc(TEST_CASE*sizeof(unsigned)*2);
	unsigned* values=(unsigned*)malloc(TEST_CASE*sizeof(unsigned));
	for(unsigned index=0;index<TEST_CASE;index++){
		keys[index<<1]=PCG32(PCGStatus);
		keys[(index<<1)+1]=PCG32(PCGStatus);
		values[index]=index;
	}
	std::map<unsigned,unsigned> stdMap32;
	std::map<long long unsigned int,long long unsigned int> stdMap64;
	RBTree* RBTree32=RBTree32Create();
	RBTree* RBTree64=RBTree64Create();
	long long unsigned int* keys64=(long long unsigned int*)keys;
	for(unsigned index=0;index<TEST_CASE;index++){
		RBTreeInsert(&RBTree32,keys[index],values[index]);
		RBTreeInsert(&RBTree64,keys64[index],values[index]);
		stdMap32[keys[index]]=values[index];
		stdMap64[keys64[index]]=values[index];
	}
	check(RBTree32,stdMap32);
	check(RBTree64,stdMap64);
	for(unsigned index=0;index<TEST_CASE;index++){
		unsigned rand=PCG32(PCGStatus);
		if(rand%3<1){
			stdMap32.erase(keys[index]);
			RBTreeDelete(RBTree32,keys[index]);
		}else{
			stdMap32[rand]=rand;
			RBTreeInsert(&RBTree32,rand,rand);
		}
		long long unsigned int rand64;
		{
			unsigned rand[2];
			rand[0]=PCG32(PCGStatus);
			rand[1]=PCG32(PCGStatus);
			rand64=*((long long unsigned int*)rand);
		}
		if(rand64%3<1){
			stdMap64.erase(keys64[index]);
			RBTreeDelete(RBTree64,keys64[index]);
		}else{
			stdMap64[rand64]=rand64;
			RBTreeInsert(&RBTree64,rand64,rand64);
		}
	}
	RBTreeMemoryShrink(&RBTree32);
	RBTreeMemoryShrink(&RBTree64);
	check(RBTree32,stdMap32);
	check(RBTree64,stdMap64);
	printf("\n");
	stdMap32.clear();
	stdMap64.clear();
	RBTreeDestroy(&RBTree32);
	RBTreeDestroy(&RBTree64);
	free(keys);
	free(values);
}

int main(int argc, char const *argv[]){

	keys=(long long unsigned int*)malloc(TEST_CASE*sizeof(long long unsigned int));
	values=(unsigned*)malloc(TEST_CASE*sizeof(unsigned));

	PCG32Struct PCGStatus;
	PCGStatus.seed=time(NULL);
	PCGStatus.seed=1;
	PCG32Init(&PCGStatus);
	test(&PCGStatus);

	std::map<unsigned,unsigned> stdMap32;
	std::map<long long unsigned int,long long unsigned int> stdMap64;
	RBTree* RBTree32=RBTree32Create();
	RBTree* RBTree64=RBTree64Create();
	timeConsume(&PCGStatus,&RBTree32,stdMap32);
	timeConsume(&PCGStatus,&RBTree64,stdMap64);
	return 0;
}