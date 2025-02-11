# RBTreeArray
Red black tree C implementation, tree and nodes are in a continuous memory region thus you can write the RBTree into file/share_memory or read from file/share_memory.
红黑树C实现, 树和节点都在一段连续的内存空间内, 因此可以将其写入文件/共享内存, 或者从文件/共享内存中读取.

High performance, search in `RBTree32` is 3 timmes quicker than `std::map::find`.

# Usage
As a head only library, just `#include "RBTreeArray.h"` and make sure `RBTreeArrayCore.h` and `RBTreeArrayStruct.h` are in the same directory. 
做为一个仅有头文件的库, 只需要`#include "RBTreeArray.h"`并且保证`RBTreeArrayCore.h`和`RBTreeArrayStruct.h`在同一个文件夹.

# APIs
## Create
`RBTree* RBTree32Create()`
Create `RBTree32` with the default node array size(LeastNodeCount, 256). if the node number of a tree equal to its node size, 
the next time of node inserting will cause auto-increment on its node array.
以默认节点数组大小(`LeastNodeCount`, 256)创建创建`RBTree32`, 当节点数组大小等于节点个数的时候, 下一次添加节点的时候将会触发节点数组的自增.

`RBTree* RBTree64Create()`
Create `RBTree64` with the default node array size(LeastNodeCount, 256)
以默认节点数组大小(`LeastNodeCount`, 256)创建创建`RBTree64` 

`RBTree* RBTree32CreateSize(long long unsigned int size)`
Create `RBTree32` with the specific `size`, if `size==0`, then the node array size if 1.
以指定节点数组大小创建`RBTree32`,  如果`size==0`, 则节点数组大小为1

`RBTree* RBTree64CreateSize(long long unsigned int size)`
Create `RBTree64` with the specific `size`, if `size==0`, then the node array size if 1.
以指定节点数组大小创建`RBTree64`,  如果`size==0`, 则节点数组大小为1

## Search
`bool RBTreeSearch(const RBTree* tree,long long unsigned int key,long long unsigned int* value)`
Searh in `RBTree32` or `RBTree64` by key, value will save to `*(value)`
在`RBTree32`或者`RBTree64`中按`key`查找, 其值写入`*(value)`中 

## Insert
`bool RBTreeInsert(RBTree** treePointer,long long unsigned int key,long long unsigned int value)`
Insert a key, value pair to `RBTree32` or `RBTree64`, note the parameter is `RBTree**` as it may cause tree's node array auto-increment.
The new pointer of the tree will save to `*(treePointer)`
example of calling:
```
#define CASE_SIZE 10000
RBTree* RBTree32=RBTree32CreateSize(CASE_SIZE);
RBTreeInsert(&RBTree32,123456,111);
```
在`RBTree32`或者`RBTree64`中插入一个key, value对, 注意参数是`RBTree**`, 因为该接口可能导致节点数组的自增, 新的树指针将存入`*(treePointer)`
调用方式示例:
```
#define CASE_SIZE 10000
RBTree* RBTree32=RBTree32CreateSize(CASE_SIZE);
RBTreeInsert(&RBTree32,123456,111);
```

## Delete
`bool RBTreeDelete(RBTree* tree,long long unsigned int key)`
Delete a key, value pair in `RBTree32` or `RBTree64`.
在`RBTree32`或者`RBTree64`中删除一个key, value对

## Clear and Destory
`bool RBTreeClear(RBTree* tree)`
Clear a `RBTree32` or `RBTree64`, this will not change its memory usage.
清除`RBTree32`或者`RBTree64` , 不会改变红黑树的内存占用

`bool RBTreeDestroy(RBTree** treePointer)`
Destroy a `RBTree32` or `RBTree64`, this will release its memory usage and set `*(treePointer)` to `NULL`
销毁`RBTree32`或者`RBTree64`,  清除红黑树的内存占用并且设置`*(treePointer)`为`NULL`

## Others
`long long unsigned int RBTreeKeyCount(const RBTree* tree)`
Return the number of key, value in a `RBTree32` or `RBTree64`.
返回`RBTree32`或者`RBTree64`的key, value对的个数

`void RBTreeGetMax(const RBTree* tree,long long unsigned int* key,long long unsigned int* value)`
Get the value of the maximal key in `RBTree32` or `RBTree64`, its value will save to `*(value)`
获取最大的`key`的值, 写入`*(value)`

`void RBTreeGetMin(const RBTree* tree,long long unsigned int* key,long long unsigned int* value)`
Get the value of the minimal key in `RBTree32` or `RBTree64`, its value will save to `*(value)`
获取最小的`key`的值, 写入`*(value) `

`bool RBTreeMemoryShrink(RBTree** treePointer)`
Shirk the memory usage of `RBTree32` or `RBTree64`.
缩小`RBTree32`或者`RBTree64`的内存占用.


