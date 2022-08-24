#include "args.h"

#include <string.h>

typedef enum {
	REGULAR,
	SHORT_FLAG,
	LONG_FLAG
} ArgType;

typedef struct {
	ArgType t;
	const char* arg;
} TypeArgPair;

void
Args_nop(const char* arg){ return; }

void
Args_nohandle(const char** args, size_t n){ return; }

size_t
Args_regSize(FlagRegistry *reg){
	return reg->len;
}

FlagRegistry
Args_newReg(){
	FlagRegistry reg;
	for(size_t i = 0; i < ARGS_MAX_ENTRIES; i++)
		reg.entries[i] = (Flag){NULL, NULL, false};
	reg.len = 0;
	return reg;
}

void
Args_pushFlag(FlagRegistry *reg, Flag f){
	// check for lenght, nulls and weather flag already exists
	if(reg->len + 1>= ARGS_MAX_ENTRIES || reg == NULL || f.id == NULL 
		 || f.action == NULL || Args_getFlag(reg, f.id) != NULL)
		return ;
	reg->entries[reg->len] = f;
	reg->len++;
}

void
Args_popFlag(FlagRegistry *reg){
	if(reg->len == 0 || reg == NULL)
		return;
	
	reg->entries[reg->len - 1] = (Flag){NULL, NULL, false};
	reg->len--;
}

Flag*
Args_getFlag(FlagRegistry *reg, const char* id){
	if(id == NULL || reg == NULL) return NULL;

	for(size_t i = 0; i < reg->len; i++){
		if(strcmp(id, reg->entries[i].id) == 0)
			return &(reg->entries[i]);
	}

	return NULL;
}

#include <stdio.h> // DEBUG

// Scan args using reg as a reference, write contents to a argtype map.
static void
Args_scan(FlagRegistry *reg, TypeArgPair* map, const char** args, size_t n){
	const char* arg = NULL;
	size_t arglen = 0;

	for(size_t i = 0; i < n; i++){
		map[i].arg = arg;
	 	arg = args[i];
		arglen = strlen(arg);
		printf("> %s\t", arg);
		if(arglen > 1){
			// short flag
			if(arg[0] == '-' && arg[1] != '-'){ 
				printf("SHORT\n");
				if(Args_getFlag(reg, arg) == NULL){
					map[i].t = SHORT_FLAG;
				}
			}
			// long flag
			else if (arg[0] == '-' && arg[1] == '-' && arglen == 2){
				printf("LONG\n");
				if(Args_getFlag(reg, arg) == NULL){
					map[i].t = LONG_FLAG;
				}
			}
			// arg
			else {
				printf("ARG\n");
				map[i].t = REGULAR;
			}
		}
		// too short to be a flag
		else {
			printf("SMALL ARG\n");
			map[i].t = REGULAR;
		}
	}
}

void
Args_execFlags(FlagRegistry *reg, const char** args, size_t n){
	if(n == 0) return;
	TypeArgPair typeMap[n];
	const char* arg = NULL;
	Args_scan(reg, typeMap, args, n);
	for(size_t i = 0; i < n - 1; i++){
		switch(typeMap[i].t){
			case SHORT_FLAG:
			break;
			case LONG_FLAG:
			break;
			case REGULAR:
			break;
		}
	}
}

