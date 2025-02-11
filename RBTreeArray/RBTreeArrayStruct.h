#ifndef __RBTREE_ARRAY_STRUCT_H__
#define __RBTREE_ARRAY_STRUCT_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef struct RBTreeNode32{
	unsigned fatherIndex; 
	unsigned leftIndex;   
	unsigned rightIndex;  
	unsigned color;       
	unsigned key;         
	unsigned value;       
}Node32;

typedef struct RBTreeNode64{
	long long unsigned int fatherIndex; // 0
	long long unsigned int leftIndex;   // 1
	long long unsigned int rightIndex;  // 2
	long long unsigned int color;       // 3
	long long unsigned int key;         // 4
	long long unsigned int value;       // 5
}Node64;

typedef struct RBTree{
	long long unsigned int nodeCount;
	long long unsigned int rootIndex;
	long long unsigned int size;
	long long unsigned int bitLength;
	char nodes[];
}RBTree;

#ifdef __cplusplus
}
#endif

#endif // RBTreeArrayStruct.h