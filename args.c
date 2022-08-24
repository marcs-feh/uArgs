#include "args.h"

#include <string.h>

FlagRegistry
Args_newReg(){
	FlagRegistry reg;
	for(size_t i = 0; i < ARGS_MAX_ENTRIES; i++)
		reg.entries[i] = (Flag){NULL, NULL};
	reg.len = 0;
	return reg;
}

void
Args_pushFlag(FlagRegistry *reg, Flag f){
	if(reg->len + 1>= ARGS_MAX_ENTRIES || reg == NULL 
		 || f.id == NULL || f.action == NULL)
		return ;
	reg->entries[reg->len] = f;
	reg->len++;
}

void
Args_popFlag(FlagRegistry *reg){
	if(reg->len == 0 || reg == NULL)
		return;
	
	reg->entries[reg->len - 1] = (Flag){NULL, NULL};
	reg->len--;
}

Flag*
Args_getFlag(FlagRegistry *reg, const char* id){
	if(id == NULL || reg == NULL) return NULL;
	for(size_t i = 0; i < reg->len; i++)
		if(strcmp(id, reg->entries[i].id))
			return &(reg->entries[i]);

	return NULL;
}

