#ifndef __PCG32_H__
#define __PCG32_H__

static long long unsigned int const multiplier=6364136223846793005LLU;
static long long unsigned int const increment=1442695040888963407LLU;

typedef struct _PCG32Struct{
	long long unsigned int state;
	long long unsigned int seed;
}PCG32Struct;

static inline unsigned rotr32(unsigned x,unsigned r){
	return x>>r|x<<(-r&31);
}

static inline unsigned PCG32(PCG32Struct* status){
	long long unsigned int x=status->state;
	unsigned count=(unsigned)(x>>59);
	status->state=x*multiplier+increment;
	x=x^(x>>18);
	return rotr32((unsigned)(x>>27),count);
}

static inline void PCG32Init(PCG32Struct* status){
	status->state=status->seed+increment;
	PCG32(status);
}

#endif