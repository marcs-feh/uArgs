#include "args.h"
#include <string.h>

#include <stdio.h> // debug
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
Args_nohandle(const char** args, size_t n){ return; }

size_t
Args_regSize(FlagRegistry *reg){
	return reg->len;
}

FlagRegistry
Args_newReg(){
	FlagRegistry reg;
	for(size_t i = 0; i < ARGS_MAX_ENTRIES; i++)
		reg.entries[i] = (Flag){NULL, NULL};
	reg.len = 0;
	reg.unknownFlagHandler = Args_nohandle;
	reg.missingArgHandler = Args_nohandle;
	return reg;
}

// Check if flag has a valid identifier (non-NULL, no whitespace and no leanding -).
static bool
Args_validateFlagID(Flag f){
	if(f.id == NULL)
		return false;
	size_t idlen = strlen(f.id);
	bool foundAlphaNumeric = false;
	//printf("%s\t", f.id);
	for(size_t i = 0; i < idlen; i++){
		//printf("(%c:%d) ", f.id[i], foundAlphaNumeric);
		// check for spaces and quotes
		if(f.id[i] == ' ' || f.id[i] == '\n' || f.id[i] == '\t' ||
			 f.id[i] == '"' || f.id[i] == '\'')
			return false;
		// check for leading -
		if(f.id[i] == '-' && !foundAlphaNumeric)
			return false;

		if(f.id[i] != '-')
			foundAlphaNumeric = true;
	}
	//printf("\n");
	return true;
}

void
Args_pushFlag(FlagRegistry *reg, Flag f){
	// check for lenght, nulls, weather flag already exists
	// and if it has a valid id
	if(reg->len + 1>= ARGS_MAX_ENTRIES || reg == NULL || f.id == NULL 
		 || Args_getFlag(reg, f.id) != NULL || !Args_validateFlagID(f))
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

const Flag*
Args_getFlag(const FlagRegistry *reg, const char* id){
	if(id == NULL || reg == NULL) return NULL;

	for(size_t i = 0; i < reg->len; i++){
		//printf("cmp: '%s'\t'%s'\n", id, reg->entries[i].id);
		if(strcmp(id, reg->entries[i].id) == 0)
			return (reg->entries + i);
	}

	return NULL;
}

// Executes flag action, checks for NULL.
void
Args_runFlag(const Flag *f, const char* arg){
	if(f == NULL) return;
	if(f->action == NULL) return;
	f->action(arg);
}

// Scan args using reg as a reference, write contents to a argtype map.
static void
Args_scan(FlagRegistry *reg, TypeArgPair* map, const char** args, size_t n){
	const char* arg = NULL;
	size_t arglen = 0;

	for(size_t i = 0; i < n; i++){
		map[i].arg = arg;
	 	arg = args[i];
		arglen = strlen(arg);
		//printf("> %s\t", arg);
		if(arglen > 1){
			// short flag
			if(arg[0] == '-' && arg[1] != '-'){ 
				//printf("SHORT\n");
				if(Args_getFlag(reg, arg) == NULL){
					map[i].t = SHORT_FLAG;
				}
			}
			// long flag
			else if (arg[0] == '-' && arg[1] == '-' && arglen > 2){
				//printf("LONG\n");
				if(Args_getFlag(reg, arg) == NULL){
					map[i].t = LONG_FLAG;
				}
			}
			// arg
			else {
				//printf("ARG\n");
				map[i].t = REGULAR;
			}
		}
		// too short to be a flag
		else {
			//printf("SMALL ARG\n");
			map[i].t = REGULAR;
		}
	}
}

void
Args_execFlags(FlagRegistry *reg, const char **args, size_t n){
	if(n == 0) return;
	TypeArgPair typeMap[n + 1];
	const char *arg = NULL;
	const Flag *flag = NULL;
	size_t arglen = 0;
	Args_scan(reg, typeMap, args, n);

	typeMap[n].arg = NULL; // padding
	typeMap[n].t = REGULAR;
	for(size_t i = 0; i < n ; i++){
	 	arg = args[i];
		arglen = strlen(arg);
		switch(typeMap[i].t){
			case SHORT_FLAG:
				//printf("SHORT FLAG: '%s'\n", arg);
				for(size_t j = 1; j < arglen; j++){
					char flagbuf[] = {arg[j], '\0'};
					flag = Args_getFlag(reg, flagbuf);
					if(flag != NULL)
						Args_runFlag(flag, args[i + 1]);
					else
						printf("[e] unknown flag: -%s\n", flagbuf);
						//reg->unknownFlagHandler(args, n);
				}
			break;

			case LONG_FLAG:
				//printf("LONG FLAG: '%s'\n", arg);
				flag = Args_getFlag(reg, arg + 2);
				if(flag != NULL)
					Args_runFlag(flag, args[i + 1]);
				else
					printf("[e] unknown flag: %s\n", arg);
					//reg->unknownFlagHandler(args, n);
			break;

			default: break;
			//printf("ARG: '%s'\n", arg);
		}
	}
}

