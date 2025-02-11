#ifndef __RBTREE_ARRAY_CORE_H__
#define __RBTREE_ARRAY_CORE_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROUTE_CASE_RR 0
#define ROUTE_CASE_RL 1
#define ROUTE_CASE_LR 2
#define ROUTE_CASE_LL 3

static inline RBTree* RBTreeCreate(){
	RBTree* tree=(RBTree*)malloc(sizeof(RBTree)+sizeof(Node)*LeastNodeCount);
	if(tree){
		tree->nodeCount=0;
		tree->rootIndex=0;
		tree->size=LeastNodeCount&MaxNodeCount;
		tree->bitLength=RBTreeBitLength;
	}
	return tree;
}

static inline RBTree* RBTreeCreateSize(long long unsigned int size){
	if(!size){
		size=1;
	}
	RBTree* tree=(RBTree*)malloc(sizeof(RBTree)+sizeof(Node)*(size&MaxNodeCount));
	if(tree){
		tree->nodeCount=0;
		tree->rootIndex=0;
		tree->size=size&MaxNodeCount;
		tree->bitLength=RBTreeBitLength;
	}
	return tree;
}

static inline bool RBTreeSearchCore(const RBTree* tree,long long unsigned int key,long long unsigned int* value){
	Node* nodes=(Node*)(tree->nodes);
	Node* current=nodes+tree->rootIndex;
	while(true){
		if(key>current->key){
			if(current->rightIndex==RBTreeInvalidNodeFlag){
				return false;
			}
			current=nodes+current->rightIndex;
			continue;
		}
		if(key<current->key){
			if(current->leftIndex==RBTreeInvalidNodeFlag){
				return false;
			}
			current=nodes+current->leftIndex;
			continue;
		}
		*(value)=current->value;
		return true;
	}
}

static inline unsigned RBTreeNodeGetRouteCase(const Node* firstNode,const Node* current,const Node* father,const Node* grandfather){
	if(grandfather->leftIndex==father-firstNode){
		if(father->leftIndex==current-firstNode){
			return ROUTE_CASE_LL;
		}
		return ROUTE_CASE_LR;
	}
	if(father->leftIndex==current-firstNode){
		return ROUTE_CASE_RL;
	}
	return ROUTE_CASE_RR;
}

static inline bool RBTreeInsertCore(RBTree* tree,Node* firstNode,Node* root,Node* current,Node* father,Node* grandfather){
	unsigned routeCase;
	Node* greatGrandfather;
	while((current->color==COLOR_RED)&&(father->color==COLOR_RED)){
		routeCase=RBTreeNodeGetRouteCase(firstNode,current,father,grandfather);
		switch(routeCase){
		case ROUTE_CASE_RR:
			if(grandfather->leftIndex!=RBTreeInvalidNodeFlag){
				if((firstNode+grandfather->leftIndex)->color==COLOR_RED){
					goto redUncle;
				}
			}
			grandfather->rightIndex=father->leftIndex;
			if(grandfather->rightIndex!=RBTreeInvalidNodeFlag){
				(firstNode+grandfather->rightIndex)->fatherIndex=grandfather-firstNode;
			}
			father->leftIndex=grandfather-firstNode;
			greatGrandfather=firstNode+grandfather->fatherIndex;
			grandfather->fatherIndex=father-firstNode;
			father->fatherIndex=greatGrandfather-firstNode;
			if(grandfather==root){
				tree->rootIndex=father-firstNode;
			}else{
				if(firstNode+greatGrandfather->leftIndex==grandfather){
					greatGrandfather->leftIndex=father-firstNode;
				}else{
					greatGrandfather->rightIndex=father-firstNode;
				}
			}
			father->color=COLOR_BLACK;
			grandfather->color=COLOR_RED;
			return true;
		case ROUTE_CASE_RL:
			if(grandfather->leftIndex!=RBTreeInvalidNodeFlag){
				if((firstNode+grandfather->leftIndex)->color==COLOR_RED){
					goto redUncle;
				}
			}
			father->leftIndex=current->rightIndex;
			if(father->leftIndex!=RBTreeInvalidNodeFlag){
				(firstNode+father->leftIndex)->fatherIndex=father-firstNode;
			}
			grandfather->rightIndex=current->leftIndex;
			if(grandfather->rightIndex!=RBTreeInvalidNodeFlag){
				(firstNode+grandfather->rightIndex)->fatherIndex=grandfather-firstNode;
			}
			current->leftIndex=grandfather-firstNode;
			current->rightIndex=father-firstNode;
			greatGrandfather=firstNode+grandfather->fatherIndex;
			current->fatherIndex=greatGrandfather-firstNode;
			father->fatherIndex=current-firstNode;
			grandfather->fatherIndex=current-firstNode;
			if(grandfather==root){
				tree->rootIndex=current-firstNode;
			}else{
				if(greatGrandfather->leftIndex==grandfather-firstNode){
					greatGrandfather->leftIndex=current-firstNode;
				}else{
					greatGrandfather->rightIndex=current-firstNode;
				}
			}
			current->color=COLOR_BLACK;
			grandfather->color=COLOR_RED;
			return true;
		case ROUTE_CASE_LR:
			if(grandfather->rightIndex!=RBTreeInvalidNodeFlag){
				if((firstNode+grandfather->rightIndex)->color==COLOR_RED){
					goto redUncle;
				}
			}
			father->rightIndex=current->leftIndex;
			if(father->rightIndex!=RBTreeInvalidNodeFlag){
				(firstNode+father->rightIndex)->fatherIndex=father-firstNode;
			}
			grandfather->leftIndex=current->rightIndex;
			if(grandfather->leftIndex!=RBTreeInvalidNodeFlag){
				(firstNode+grandfather->leftIndex)->fatherIndex=grandfather-firstNode;
			}
			current->leftIndex=father-firstNode;
			current->rightIndex=grandfather-firstNode; 
			greatGrandfather=firstNode+grandfather->fatherIndex;
			current->fatherIndex=greatGrandfather-firstNode;
			father->fatherIndex=current-firstNode;
			grandfather->fatherIndex=current-firstNode;
			if(grandfather==root){
				tree->rootIndex=current-firstNode;
			}else{
				if(greatGrandfather->leftIndex==grandfather-firstNode){
					greatGrandfather->leftIndex=current-firstNode;
				}else{
					greatGrandfather->rightIndex=current-firstNode;
				}
			}
			current->color=COLOR_BLACK;
			grandfather->color=COLOR_RED;
			return true;
		case ROUTE_CASE_LL:
			if(grandfather->rightIndex!=RBTreeInvalidNodeFlag){
				if((firstNode+grandfather->rightIndex)->color==COLOR_RED){
					goto redUncle;
				}
			}
			grandfather->leftIndex=father->rightIndex;
			if(grandfather->leftIndex!=RBTreeInvalidNodeFlag){
				(firstNode+grandfather->leftIndex)->fatherIndex=grandfather-firstNode;
			}
			father->rightIndex=grandfather-firstNode;
			greatGrandfather=firstNode+grandfather->fatherIndex;
			father->fatherIndex=greatGrandfather-firstNode;
			grandfather->fatherIndex=father-firstNode;
			if(root==grandfather){
				tree->rootIndex=father-firstNode;
			}else{
				if(greatGrandfather->leftIndex==grandfather-firstNode){
					greatGrandfather->leftIndex=father-firstNode;
				}else{
					greatGrandfather->rightIndex=father-firstNode;
				}
			}
			father->color=COLOR_BLACK;
			grandfather->color=COLOR_RED;
			return true;
		default:
			return false;
		}
	redUncle:
		grandfather->color=COLOR_RED;
		(firstNode+grandfather->leftIndex)->color=COLOR_BLACK;
		(firstNode+grandfather->rightIndex)->color=COLOR_BLACK;
		current=firstNode+((firstNode+current->fatherIndex)->fatherIndex);
		if(current-firstNode==tree->rootIndex||current->fatherIndex==tree->rootIndex){
			(firstNode+tree->rootIndex)->color=COLOR_BLACK;
			return true;
		}
		grandfather=firstNode+((firstNode+current->fatherIndex)->fatherIndex);
		father=firstNode+current->fatherIndex;
	}
	return true;
}

static inline void FatherBrotherGrandFatherUpdate(RBTree* tree,long long unsigned int toMoveIndex,long long unsigned int toDeleteIndex,Node* nodes,long long unsigned int** indexes,Node*** nodesToUpdate){
	// Loop unwinding
	long long unsigned int changeIndex=RBTreeInvalidNodeFlag;
	if(*(indexes[0])==toMoveIndex){
		changeIndex=*(indexes[0]);
		*(indexes[0])=toDeleteIndex;
		*(nodesToUpdate[0])=nodes+toDeleteIndex;
		goto checkRoot;
	}
	if(*(indexes[1])==toMoveIndex){
		changeIndex=*(indexes[1]);
		*(indexes[1])=toDeleteIndex;
		*(nodesToUpdate[1])=nodes+toDeleteIndex;
		goto checkRoot;
	}
	if(*(indexes[2])==toMoveIndex){
		changeIndex=*(indexes[2]);
		*(indexes[2])=toDeleteIndex;
		*(nodesToUpdate[2])=nodes+toDeleteIndex;
		goto checkRoot;
	}
	return;
	checkRoot:
	if(tree->rootIndex==changeIndex){
		tree->rootIndex=toDeleteIndex;
	}
}

static inline void RBTreeDeleteNode(RBTree* tree,Node* nodes,Node* father,long long unsigned int toDeleteIndex,long long unsigned int** indexes,Node*** nodesToUpdate){
	if(father->leftIndex==toDeleteIndex){
		father->leftIndex=RBTreeInvalidNodeFlag;
	}else{
		father->rightIndex=RBTreeInvalidNodeFlag;
	}
	long long unsigned int toMove=tree->nodeCount-1;
	if(likely(toMove!=toDeleteIndex)){
		if(toMove!=tree->rootIndex){
			if(nodes[nodes[toMove].fatherIndex].leftIndex==toMove){
				nodes[nodes[toMove].fatherIndex].leftIndex=toDeleteIndex;
			}else{
				nodes[nodes[toMove].fatherIndex].rightIndex=toDeleteIndex;
			}
		}else{
			tree->rootIndex=toDeleteIndex;
		}
		FatherBrotherGrandFatherUpdate(tree,toMove,toDeleteIndex,nodes,indexes,nodesToUpdate);
		if(nodes[toMove].leftIndex!=RBTreeInvalidNodeFlag){
			nodes[nodes[toMove].leftIndex].fatherIndex=toDeleteIndex;
		}
		if(nodes[toMove].rightIndex!=RBTreeInvalidNodeFlag){
			nodes[nodes[toMove].rightIndex].fatherIndex=toDeleteIndex;
		}
		nodes[toDeleteIndex]=nodes[toMove];
	}
	tree->nodeCount=tree->nodeCount-1;
}

static inline bool RBTreeDeleteCore(RBTree* tree,long long unsigned int key){
	Node* nodes=(Node*)(tree->nodes);
	Node* current=nodes+tree->rootIndex;
	if(unlikely(tree->nodeCount==1)){
		if(key==current->key){
			tree->rootIndex=0;
			tree->nodeCount=0;
			return true;
		}
		return false;
	}
	while(true){
		if(key>current->key){
			if(current->rightIndex==RBTreeInvalidNodeFlag){
				return false;
			}
			current=nodes+current->rightIndex;
			continue;
		}
		if(key<current->key){
			if(current->leftIndex==RBTreeInvalidNodeFlag){
				return false;
			}
			current=nodes+current->leftIndex;
			continue;
		}
		break;
	}
	// printf("tree->rootIndex=%llu\n",tree->rootIndex);
	bool deleted=false;
	Node* brother;
	Node* father=nodes+current->fatherIndex;
	Node* grandfather;
	long long unsigned int currentIndex=current-nodes;
	long long unsigned int myFatherIndex=current->fatherIndex;
	long long unsigned int myBrotherIndex=RBTreeInvalidNodeFlag;
	long long unsigned int myGrandfatherIndex=RBTreeInvalidNodeFlag;
	long long unsigned int* indexes[]={&myFatherIndex,&myBrotherIndex,&myGrandfatherIndex};
	Node** nodesToUpdate[]={&father,&brother,&grandfather};
	if(unlikely(current->fatherIndex==RBTreeInvalidNodeFlag)){
		if(current->leftIndex==RBTreeInvalidNodeFlag){
			current->key=(nodes+current->rightIndex)->key;
			current->value=(nodes+current->rightIndex)->value;
			RBTreeDeleteNode(tree,nodes,current,current->rightIndex,indexes,nodesToUpdate);
			return true;
		}else{
			if(current->rightIndex==RBTreeInvalidNodeFlag){
				current->key=(nodes+current->leftIndex)->key;
				current->value=(nodes+current->leftIndex)->value;
				RBTreeDeleteNode(tree,nodes,current,current->leftIndex,indexes,nodesToUpdate);
				return true;
			}else{
				goto gotoRightSmallest;
			}
		}
	}
	if(current->leftIndex==RBTreeInvalidNodeFlag){
		deleteBegin:
		if(current->rightIndex==RBTreeInvalidNodeFlag){
			// No child
			if(current->color==COLOR_RED){
				RBTreeDeleteNode(tree,nodes,father,currentIndex,indexes,nodesToUpdate);
				return true;
			}
			doubleBlackFix:
			grandfather=nodes+father->fatherIndex;
			myGrandfatherIndex=father->fatherIndex;
			if(father->leftIndex==currentIndex){
				brother=nodes+father->rightIndex;
				myBrotherIndex=father->rightIndex;
				if(brother->color==COLOR_BLACK){
					if(brother->rightIndex!=RBTreeInvalidNodeFlag){
						if((nodes+brother->rightIndex)->color==COLOR_RED){
							// case RR
							if(!deleted){
								RBTreeDeleteNode(tree,nodes,father,father->leftIndex,indexes,nodesToUpdate);
							}
							(nodes+brother->rightIndex)->color=COLOR_BLACK;
							brother->color=father->color;
							father->color=COLOR_BLACK;
							father->rightIndex=brother->leftIndex;
							if(father->rightIndex!=RBTreeInvalidNodeFlag){
								(nodes+father->rightIndex)->fatherIndex=myFatherIndex;
							}
							brother->leftIndex=myFatherIndex;
							father->fatherIndex=myBrotherIndex;
							brother->fatherIndex=myGrandfatherIndex;
							if(tree->rootIndex==myFatherIndex){
								tree->rootIndex=myBrotherIndex;
							}else{
								if(grandfather->leftIndex==myFatherIndex){
									grandfather->leftIndex=myBrotherIndex;
								}else{
									grandfather->rightIndex=myBrotherIndex;
								}
							}
							return true;
						}
					}
					if(brother->leftIndex!=RBTreeInvalidNodeFlag){
						if((nodes+brother->leftIndex)->color==COLOR_RED){
							// case RL
							if(!deleted){
								RBTreeDeleteNode(tree,nodes,father,father->leftIndex,indexes,nodesToUpdate);
							}
							Node* leftChild=nodes+brother->leftIndex;
							leftChild->color=father->color;
							father->color=COLOR_BLACK;
							father->rightIndex=leftChild->leftIndex;
							if(father->rightIndex!=RBTreeInvalidNodeFlag){
								(nodes+father->rightIndex)->fatherIndex=myFatherIndex;
							}
							leftChild->leftIndex=myFatherIndex;
							father->fatherIndex=leftChild-nodes;
							leftChild->fatherIndex=myGrandfatherIndex;
							if(tree->rootIndex==myFatherIndex){
								tree->rootIndex=leftChild-nodes;
							}else{
								if(grandfather->leftIndex==myFatherIndex){
									grandfather->leftIndex=leftChild-nodes;
								}else{
									grandfather->rightIndex=leftChild-nodes;
								}
							}
							brother->leftIndex=leftChild->rightIndex;
							if(brother->leftIndex!=RBTreeInvalidNodeFlag){
								(nodes+brother->leftIndex)->fatherIndex=myBrotherIndex;
							}
							leftChild->rightIndex=myBrotherIndex;
							brother->fatherIndex=leftChild-nodes;
							return true;
						}
					}
					if(!deleted){
						RBTreeDeleteNode(tree,nodes,father,father->leftIndex,indexes,nodesToUpdate);
					}
					goto brotherChildBothBlack;
				}else{
					brother->color=COLOR_BLACK;
					father->color=COLOR_RED;
					father->rightIndex=brother->leftIndex;
					if(father->rightIndex!=RBTreeInvalidNodeFlag){
						(nodes+father->rightIndex)->fatherIndex=myFatherIndex;
					}
					father->fatherIndex=myBrotherIndex;
					brother->leftIndex=myFatherIndex;
					brother->fatherIndex=myGrandfatherIndex;
					if(tree->rootIndex==myFatherIndex){
						tree->rootIndex=myBrotherIndex;
					}else{
						if(grandfather->leftIndex==myFatherIndex){
							grandfather->leftIndex=myBrotherIndex;
						}else{
							grandfather->rightIndex=myBrotherIndex;
						}
					}
					goto doubleBlackFix;
				}
			}else{
				brother=nodes+father->leftIndex;
				myBrotherIndex=father->leftIndex;
				if(brother->color==COLOR_BLACK){
					if(brother->leftIndex!=RBTreeInvalidNodeFlag){
						if((nodes+brother->leftIndex)->color==COLOR_RED){
							// case LL
							if(!deleted){
								RBTreeDeleteNode(tree,nodes,father,father->rightIndex,indexes,nodesToUpdate);
							}
							(nodes+brother->leftIndex)->color=COLOR_BLACK;
							brother->color=father->color;
							father->color=COLOR_BLACK;
							father->leftIndex=brother->rightIndex;
							if(father->leftIndex!=RBTreeInvalidNodeFlag){
								(nodes+father->leftIndex)->fatherIndex=myFatherIndex;
							}
							brother->rightIndex=myFatherIndex;
							father->fatherIndex=myBrotherIndex;
							brother->fatherIndex=myGrandfatherIndex;
							if(tree->rootIndex==myFatherIndex){
								tree->rootIndex=myBrotherIndex;
							}else{
								if(grandfather->leftIndex==myFatherIndex){
									grandfather->leftIndex=myBrotherIndex;
								}else{
									grandfather->rightIndex=myBrotherIndex;
								}
							}
							return true;
						}
					}
					if(brother->rightIndex!=RBTreeInvalidNodeFlag){
						if((nodes+brother->rightIndex)->color==COLOR_RED){
							// case LR
							if(!deleted){
								RBTreeDeleteNode(tree,nodes,father,father->rightIndex,indexes,nodesToUpdate);
							}
							Node* rightChild=nodes+brother->rightIndex;
							rightChild->color=father->color;
							father->color=COLOR_BLACK;
							brother->rightIndex=rightChild->leftIndex;
							if(brother->rightIndex!=RBTreeInvalidNodeFlag){
								(nodes+brother->rightIndex)->fatherIndex=myBrotherIndex;
							}
							rightChild->leftIndex=myBrotherIndex;
							brother->fatherIndex=rightChild-nodes;
							rightChild->fatherIndex=myGrandfatherIndex;
							if(tree->rootIndex==myFatherIndex){
								tree->rootIndex=rightChild-nodes;
							}else{
								if(grandfather->leftIndex==myFatherIndex){
									grandfather->leftIndex=rightChild-nodes;
								}else{
									grandfather->rightIndex=rightChild-nodes;
								}
							}
							father->leftIndex=rightChild->rightIndex;
							if(father->leftIndex!=RBTreeInvalidNodeFlag){
								(nodes+father->leftIndex)->fatherIndex=myFatherIndex;
							}
							rightChild->rightIndex=myFatherIndex;
							father->fatherIndex=rightChild-nodes;
							return true;
						}
					}
					// both of the brother's childern are null or black, and brother is black
					if(!deleted){
						RBTreeDeleteNode(tree,nodes,father,father->rightIndex,indexes,nodesToUpdate);
					}
					brotherChildBothBlack:
					brother->color=COLOR_RED;
					if(unlikely(tree->rootIndex==myFatherIndex)){
						return true;
					}
					if(father->color==COLOR_RED){
						father->color=COLOR_BLACK;
						return true;
					}
					current=father;
					currentIndex=current-nodes;
					deleted=true;
					father=nodes+current->fatherIndex;
					myFatherIndex=current->fatherIndex;
					grandfather=nodes+father->fatherIndex;
					myGrandfatherIndex=father->fatherIndex;
					goto doubleBlackFix;
				}else{
					brother->color=COLOR_BLACK;
					father->color=COLOR_RED;
					father->leftIndex=brother->rightIndex;
					if(father->leftIndex!=RBTreeInvalidNodeFlag){
						(nodes+father->leftIndex)->fatherIndex=myFatherIndex;
					}
					father->fatherIndex=myBrotherIndex;
					brother->rightIndex=myFatherIndex;
					brother->fatherIndex=myGrandfatherIndex;
					if(tree->rootIndex==myFatherIndex){
						tree->rootIndex=myBrotherIndex;
					}else{
						if(grandfather->leftIndex==myFatherIndex){
							grandfather->leftIndex=myBrotherIndex;
						}else{
							grandfather->rightIndex=myBrotherIndex;
						}
					}
					goto doubleBlackFix;
				}
			}
		}
		// no left child but right child
		current->key=(nodes+current->rightIndex)->key;
		current->value=(nodes+current->rightIndex)->value;
		RBTreeDeleteNode(tree,nodes,current,current->rightIndex,indexes,nodesToUpdate);
		return true;
	}else{
		if(current->rightIndex==RBTreeInvalidNodeFlag){
			// no right child but left child
			current->key=(nodes+current->leftIndex)->key;
			current->value=(nodes+current->leftIndex)->value;
			RBTreeDeleteNode(tree,nodes,current,current->leftIndex,indexes,nodesToUpdate);
			return true;
		}
		// left child and right child
		gotoRightSmallest:
		Node* rightSmallest=nodes+current->rightIndex;
		while(rightSmallest->leftIndex!=RBTreeInvalidNodeFlag){
			rightSmallest=nodes+rightSmallest->leftIndex;
		}
		current->key=rightSmallest->key;
		current->value=rightSmallest->value;
		current=rightSmallest;
		currentIndex=rightSmallest-nodes;
		father=nodes+current->fatherIndex;
		myFatherIndex=current->fatherIndex;
		grandfather=nodes+father->fatherIndex;
		myGrandfatherIndex=father->fatherIndex;
		goto deleteBegin;
	}
}

static inline void RBTreeGetMaxCore(const RBTree* tree,long long unsigned int* key,long long unsigned int* value){
	Node* current=(Node*)(tree->nodes)+tree->rootIndex;
	Node* nodes=(Node*)(tree->nodes);
	while(current->rightIndex!=RBTreeInvalidNodeFlag){
		current=nodes+current->rightIndex;
	}
	*(key)=current->key;
	*(value)=current->value;
}

static inline void RBTreeGetMinCore(const RBTree* tree,long long unsigned int* key,long long unsigned int* value){
	Node* current=(Node*)(tree->nodes)+tree->rootIndex;
	Node* nodes=(Node*)(tree->nodes);
	while(current->leftIndex!=RBTreeInvalidNodeFlag){
		current=nodes+current->leftIndex;
	}
	*(key)=current->key;
	*(value)=current->value;
}

static inline bool RBTreeMemoryShrinkCore(RBTree** treePointer){
	RBTree* tree=*(treePointer);
	RBTree* newTree=(RBTree*)malloc(sizeof(RBTree)+sizeof(Node)*tree->size);
	if(!newTree){
		return false;
	}
	memcpy(newTree,tree,sizeof(RBTree)+sizeof(Node)*tree->size);
	free(tree);
	*(treePointer)=newTree;
	return true;
}

#ifdef __cplusplus
}
#endif

#endif // RBTreeArrayCore.h