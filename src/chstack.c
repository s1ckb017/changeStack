#ifndef CHSTACK_H
#define CHSTACK_H
#include "chstack.h"

long int getStackBase(long int * startStack){
	FILE * fp = NULL;
	char line[1024], *tmp;
	char addr[18] = {0};
	long int ret = 0, endAddr = 0;
	char path[256];

	BUILDMAPSPATH(path, 256)

	fp = fopen(path, "r");
	if(fp == NULL){
		P_ERR("Fopen Error:", strerror(errno), ret)
	}
	while(fgets(line, 1024, fp) != NULL ){
		if(strstr(line, "[stack]") != NULL){
			if((tmp = strchr(line, '-')) != NULL){
				strcpy(addr, "0x");
				strncat(addr, line, tmp - line);
				*startStack = strtol(addr, NULL, 16);
			}		
			if((tmp = strchr(line, ' '))!= NULL){

				strcpy(addr, "0x");
				strncpy(addr,strchr(line, '-') + 1, tmp - strchr(line, '-'));
				endAddr = strtol(addr, NULL, 16);
				ret = endAddr - *startStack;					
				printf("[+]\tstackLen:%ld\tstartStack:%p\tendStack:%p\n", ret, (void *) *startStack, (void *) endAddr);
				break;
			}

		} else continue;
	
	}
	fclose(fp);
	return ret;
}

int swapRegisters(void * newStackAddr, const long int stackBaseAddr, const long int len){
	/*
	 * routine to set up new rsp and rbp cancelling current values
	 */
	long int _rsp_ = 0;
	long int _rbp_ = 0;
	long int newRbp_off = 0;
	long int newRsp_off = 0;
	
	__asm__(
			"mov %%rsp, %0\n\t"
			: "+r" (_rsp_)
			::
			);

	__asm__(
			"mov %%rbp, %0\n\t"
			: "+r" (_rbp_)
			::
			);

	printf("\t[+]\trsp:%p\trbp:%p\n", (void *)_rsp_, (void *)_rbp_);	
	
	newRsp_off = _rsp_ - stackBaseAddr;
	newRbp_off = _rbp_ - stackBaseAddr;
	memcpy(newStackAddr, (void *)stackBaseAddr, len);
	_rsp_ = newRsp_off + (long int)newStackAddr;
	_rbp_ = newRbp_off + (long int)newStackAddr;


	printf("\t[+]\tNewRsp:%p\tNewRbp:%p\n", (void *)_rsp_, (void *)_rbp_);	

	__asm__(
			"mov %0, %%rsp\n\t"
			: "+r" (_rsp_)
			::
			);
	__asm__(
			"mov %0, %%rbp\n\t"
			: "+r" (_rbp_)
			::
			);
	
	return 0;
}

void * setupNewStack(long int len){
	void * nstack = NULL;
	nstack = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_STACK, -1, 0);	
	if(nstack == (void *)1){
		P_ERR("mmap Error:", strerror(errno), NULL)
	}
	printf("[+]\tNewStackLen:%ld\tNewStartStack:%p\tNewEndStack:%p\n", len, nstack, (void *)(len + nstack));
	return nstack;
}


int chgStack(void){
	long int baseAddress = NULL, stackLen = 0;
	void * newStack = NULL;

	stackLen = getStackBase(&baseAddress);
	if(stackLen == 0) return -1;

	newStack = setupNewStack(stackLen);
	if(newStack == NULL) return -1;
	
	swapRegisters(newStack, baseAddress, stackLen);
	printf("It works\n");
	getchar();

	return 0;
}

#endif
