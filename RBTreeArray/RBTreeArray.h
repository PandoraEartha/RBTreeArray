#ifndef __RBTREE_ARRAY_H__
#define __RBTREE_ARRAY_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "RBTreeArrayStruct.h"

#define likely(x)   __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)

#define COLOR_RED 0
#define COLOR_BLACK 1

const long long unsigned int LeastNodeCount=256;
const long long unsigned int Max32NodeCount=0xFFFFFFFFLLU;
const long long unsigned int Max64NodeCount=0xFFFFFFFFFFFFFFFFLLU;
const unsigned RBTree32BitLength=32;
const unsigned RBTree64BitLength=64;

#define RBTreeCreate RBTree32Create
#define RBTreeCreateSize RBTree32CreateSize
#define RBTreeSearchCore RBTreeSearchCore32
#define RBTreeInsertCore RBTreeInsertCore32
#define RBTreeDeleteCore RBTreeDeleteCore32
#define RBTreeGetMaxCore RBTreeGetMaxCore32
#define RBTreeGetMinCore RBTreeGetMinCore32
#define RBTreeDeleteNode RBTreeDeleteNode32
#define RBTreeMemoryShrinkCore RBTreeMemoryShrinkCore32
#define RBTreeNodeGetRouteCase RBTreeNodeGetRouteCase32
#define FatherBrotherGrandFatherUpdate FatherBrotherGrandFatherUpdate32
#define Node Node32
#define MaxNodeCount Max32NodeCount
#define RBTreeBitLength RBTree32BitLength
#define RBTreeInvalidNodeFlag Max32NodeCount
#include "RBTreeArrayCore.h"
#undef RBTreeCreate
#undef RBTreeCreateSize
#undef RBTreeSearchCore
#undef RBTreeInsertCore
#undef RBTreeDeleteCore
#undef RBTreeGetMaxCore
#undef RBTreeGetMinCore
#undef RBTreeDeleteNode
#undef RBTreeMemoryShrinkCore
#undef RBTreeNodeGetRouteCase
#undef FatherBrotherGrandFatherUpdate
#undef Node
#undef MaxNodeCount
#undef RBTreeBitLength
#undef RBTreeInvalidNodeFlag

#undef __RBTREE_ARRAY_CORE_H__ // RBTreeArrayCore.h

#define RBTreeCreate RBTree64Create
#define RBTreeCreateSize RBTree64CreateSize
#define RBTreeSearchCore RBTreeSearchCore64
#define RBTreeInsertCore RBTreeInsertCore64
#define RBTreeDeleteCore RBTreeDeleteCore64
#define RBTreeGetMaxCore RBTreeGetMaxCore64
#define RBTreeGetMinCore RBTreeGetMinCore64
#define RBTreeDeleteNode RBTreeDeleteNode64
#define RBTreeMemoryShrinkCore RBTreeMemoryShrinkCore64
#define RBTreeNodeGetRouteCase RBTreeNodeGetRouteCase64
#define FatherBrotherGrandFatherUpdate FatherBrotherGrandFatherUpdate64
#define Node Node64
#define MaxNodeCount Max64NodeCount
#define RBTreeBitLength RBTree64BitLength
#define RBTreeInvalidNodeFlag Max64NodeCount
#include "RBTreeArrayCore.h"
#undef RBTreeCreate
#undef RBTreeCreateSize
#undef RBTreeSearchCore
#undef RBTreeInsertCore
#undef RBTreeDeleteCore
#undef RBTreeGetMaxCore
#undef RBTreeGetMinCore
#undef RBTreeDeleteNode
#undef RBTreeMemoryShrinkCore
#undef RBTreeNodeGetRouteCase
#undef FatherBrotherGrandFatherUpdate
#undef Node
#undef MaxNodeCount
#undef RBTreeBitLength
#undef RBTreeInvalidNodeFlag

static inline unsigned RBTree32NodeCreate(RBTree** treePointer,unsigned fatherIndex,long long unsigned int key,long long unsigned int value){
	RBTree* tree=*(treePointer);
	unsigned nodeCount=tree->nodeCount;
	if(unlikely(nodeCount==tree->size)){
		long long unsigned int size=tree->size;
		size=size<<1;
		if(size>Max32NodeCount){
			size=Max32NodeCount;
		}
		RBTree* newTree=RBTree32CreateSize(size);
		memcpy(newTree,tree,sizeof(RBTree)+sizeof(Node32)*(tree->size));
		free(tree);
		newTree->size=size;
		*(treePointer)=newTree;
		tree=newTree;
	}
	Node32* nodes=(Node32*)(tree->nodes);
	nodes[nodeCount].fatherIndex=fatherIndex;
	nodes[nodeCount].key=key;
	nodes[nodeCount].value=value;
	nodes[nodeCount].leftIndex=Max32NodeCount;
	nodes[nodeCount].rightIndex=Max32NodeCount;
	nodes[nodeCount].color=COLOR_RED;
	tree->nodeCount=tree->nodeCount+1;
	return tree->nodeCount-1;
}

static inline long long unsigned int RBTree64NodeCreate(RBTree** treePointer,long long unsigned int fatherIndex,long long unsigned int key,long long unsigned int value){
	RBTree* tree=*(treePointer);
	long long unsigned int nodeCount=tree->nodeCount;
	if(unlikely(nodeCount==tree->size)){
		long long unsigned int size=tree->size;
		size=size<<1;
		RBTree* newTree=RBTree64CreateSize(size);
		memcpy(newTree,tree,sizeof(RBTree)+sizeof(Node64)*(tree->size));
		free(tree);
		newTree->size=size;
		*(treePointer)=newTree;
		tree=newTree;
	}
	Node64* nodes=(Node64*)(tree->nodes);
	nodes[nodeCount].fatherIndex=fatherIndex;
	nodes[nodeCount].key=key;
	nodes[nodeCount].value=value;
	nodes[nodeCount].leftIndex=Max64NodeCount;
	nodes[nodeCount].rightIndex=Max64NodeCount;
	nodes[nodeCount].color=COLOR_RED;
	tree->nodeCount=tree->nodeCount+1;
	return tree->nodeCount-1;
}

static inline bool RBTreeSearch(const RBTree* tree,long long unsigned int key,long long unsigned int* value){
	if((!tree)||(!value)){
		return false;
	}
	if(tree->bitLength==RBTree32BitLength){
		return RBTreeSearchCore32(tree,key,value);
	}else if(tree->bitLength==RBTree64BitLength){
		return RBTreeSearchCore64(tree,key,value);
	}else{
		return false;
	}
}

static inline bool RBTree32Insert(RBTree** treePointer,long long unsigned int key,long long unsigned int value){
	RBTree* tree=*(treePointer);
	Node32* nodes=(Node32*)(tree->nodes);
	if(unlikely(tree->nodeCount==0)){
		unsigned rootIndex=RBTree32NodeCreate(treePointer,Max32NodeCount,key,value);
		tree=*(treePointer);
		tree->rootIndex=rootIndex;
		nodes=(Node32*)(tree->nodes);
		nodes[rootIndex].color=COLOR_BLACK;
		return true;
	}
	Node32* firstNode=(Node32*)(tree->nodes);
	Node32* current=nodes+tree->rootIndex;
	while(true){
		if(key>current->key){
			if(current->rightIndex==Max32NodeCount){
				if(unlikely(tree->nodeCount==Max32NodeCount)){
					return false;
				}
				long long unsigned int currentIndex=current-nodes;
				unsigned rightIndex=RBTree32NodeCreate(treePointer,currentIndex,key,value);
				tree=*(treePointer);
				nodes=(Node32*)(tree->nodes);
				current=nodes+currentIndex;
				current->rightIndex=rightIndex;
				current=nodes+rightIndex;
				break;
			}
			current=nodes+current->rightIndex;
			continue;
		}
		if(key<current->key){
			if(current->leftIndex==Max32NodeCount){
				if(unlikely(tree->nodeCount==Max32NodeCount)){
					return false;
				}
				long long unsigned int currentIndex=current-nodes;
				unsigned leftIndex=RBTree32NodeCreate(treePointer,current-nodes,key,value);
				tree=*(treePointer);
				nodes=(Node32*)(tree->nodes);
				current=nodes+currentIndex;
				current->leftIndex=leftIndex;
				current=nodes+leftIndex;
				break;
			}
			current=nodes+current->leftIndex;
			continue;
		}
		current->value=value;
		return true;
	}
	// printf("tree->size=%llu\n",tree->size);
	firstNode=(Node32*)(tree->nodes);
	Node32* root=firstNode+tree->rootIndex;
	Node32* father=firstNode+current->fatherIndex;
	// RR==0 RL==1 LR==2 LL==3
	if(father->fatherIndex!=Max32NodeCount){
		Node32* grandfather=firstNode+father->fatherIndex;
		Node32* greatGrandfather=NULL;
		return RBTreeInsertCore32(tree,firstNode,root,current,father,grandfather);
	}
	return true;
}

static inline bool RBTree64Insert(RBTree** treePointer,long long unsigned int key,long long unsigned int value){
	RBTree* tree=*(treePointer);
	Node64* nodes=(Node64*)(tree->nodes);
	if(unlikely(tree->nodeCount==0)){
		long long unsigned int rootIndex=RBTree64NodeCreate(treePointer,Max64NodeCount,key,value);
		tree=*(treePointer);
		tree->rootIndex=rootIndex;
		nodes=(Node64*)(tree->nodes);
		nodes[rootIndex].color=COLOR_BLACK;
		return true;
	}
	Node64* current=nodes+tree->rootIndex;
	while(true){
		if(key>current->key){
			if(current->rightIndex==Max64NodeCount){
				long long unsigned int currentIndex=current-nodes;
				long long unsigned int rightIndex=RBTree64NodeCreate(treePointer,currentIndex,key,value);
				tree=*(treePointer);
				nodes=(Node64*)(tree->nodes);
				current=nodes+currentIndex;
				current->rightIndex=rightIndex;
				current=nodes+rightIndex;
				break;
			}
			current=nodes+current->rightIndex;
			continue;
		}
		if(key<current->key){
			if(current->leftIndex==Max64NodeCount){
				long long unsigned int currentIndex=current-nodes;
				long long unsigned int leftIndex=RBTree64NodeCreate(treePointer,currentIndex,key,value);
				tree=*(treePointer);
				nodes=(Node64*)(tree->nodes);
				current=nodes+currentIndex;
				current->leftIndex=leftIndex;
				current=nodes+leftIndex;
				break;
			}
			current=nodes+current->leftIndex;
			continue;
		}
		current->value=value;
		return true;
	}
	Node64* firstNode=(Node64*)(tree->nodes);
	Node64* root=firstNode+tree->rootIndex;
	Node64* father=firstNode+current->fatherIndex;
	// RR==0 RL==1 LR==2 LL==3
	if(father->fatherIndex!=Max64NodeCount){
		Node64* grandfather=firstNode+father->fatherIndex;
		Node64* greatGrandfather=NULL;
		return RBTreeInsertCore64(tree,firstNode,root,current,father,grandfather);
	}
	return true;
}

static inline bool RBTreeInsert(RBTree** treePointer,long long unsigned int key,long long unsigned int value){
	if(!treePointer){
		return false;
	}else{
		if(!(*(treePointer))){
			return false;
		}
	}
	RBTree* tree=*(treePointer);
	if(tree->bitLength==RBTree32BitLength){
		return RBTree32Insert(treePointer,key,value);
	}else if(tree->bitLength==RBTree64BitLength){
		return RBTree64Insert(treePointer,key,value);
	}else{
		return false;
	}
	return false;
}

static inline bool RBTreeDelete(RBTree* tree,long long unsigned int key){
	if(!tree){
		return false;
	}
	if(tree->nodeCount==0){
		return false;
	}
	if(tree->bitLength==RBTree32BitLength){
		return RBTreeDeleteCore32(tree,key&Max32NodeCount);
	}else if(tree->bitLength==RBTree64BitLength){
		return RBTreeDeleteCore64(tree,key);
	}else{
		return false;
	}
}

static inline bool RBTreeClear(RBTree* tree){
	if(!tree){
		return false;
	}
	tree->nodeCount=0;
	tree->rootIndex=0;
	return true;
}

static inline bool RBTreeDestroy(RBTree** treePointer){
	if(!treePointer){
		return false;
	}else{
		if(!(*(treePointer))){
			return false;
		}
	}
	free(*(treePointer));
	*(treePointer)=NULL;
	return true;
}

static inline long long unsigned int RBTreeKeyCount(const RBTree* tree){
	if(!tree){
		return 0;
	}
	return tree->nodeCount;
}

static inline void RBTreeGetMax(const RBTree* tree,long long unsigned int* key,long long unsigned int* value){
	if((!tree)||(!key)||(!value)){
		return;
	}
	if(!tree->nodeCount){
		return;
	}
	if(tree->bitLength==RBTree32BitLength){
		RBTreeGetMaxCore32(tree,key,value);
	}else if(tree->bitLength==RBTree64BitLength){
		RBTreeGetMaxCore64(tree,key,value);
	}else{
		return;
	}
}

static inline void RBTreeGetMin(const RBTree* tree,long long unsigned int* key,long long unsigned int* value){
	if((!tree)||(!key)||(!value)){
		return;
	}
	if(!tree->nodeCount){
		return;
	}
	if(tree->bitLength==RBTree32BitLength){
		RBTreeGetMinCore32(tree,key,value);
	}else if(tree->bitLength==RBTree64BitLength){
		RBTreeGetMinCore64(tree,key,value);
	}else{
		return;
	}
}

static inline bool RBTreeMemoryShrink(RBTree** treePointer){
	if(!treePointer){
		return false;
	}else{
		if(!(*(treePointer))){
			return false;
		}
	}
	RBTree* tree=*(treePointer);
	if(tree->bitLength==RBTree32BitLength){
		return RBTreeMemoryShrinkCore32(treePointer);
	}else if(tree->bitLength==RBTree64BitLength){
		return RBTreeMemoryShrinkCore64(treePointer);
	}else{
		return false;
	}
}

#ifdef __cplusplus
}
#endif

#endif // RBTreeArray.h