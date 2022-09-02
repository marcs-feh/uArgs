#include "uargs.h"
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

size_t
uArgs_regSize(uArgsRegistry *reg){
	return reg->len;
}

uArgsRegistry
uArgs_newReg(){
	uArgsRegistry reg;
	for(size_t i = 0; i < UARGS_MAX_ENTRIES; i++)
		reg.entries[i] = (uArgsFlag){NULL, NULL};
	reg.len = 0;
	reg.unknownFlagHandler = NULL;
	return reg;
}

// Check if flag has a valid identifier (non-NULL, no whitespace and no leanding -).
static bool
uArgs_validateFlagID(uArgsFlag f){
	if(f.id == NULL)
		return false;
	size_t idlen = strlen(f.id);
	bool foundAlphaNumeric = false;
	for(size_t i = 0; i < idlen; i++){
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
	return true;
}

void
uArgs_pushFlag(uArgsRegistry *reg, uArgsFlag f){
	// check for lenght, nulls, weather flag already exists
	// and if it has a valid id
	if(reg->len + 1>= UARGS_MAX_ENTRIES || reg == NULL || f.id == NULL
		 || uArgs_getFlag(reg, f.id) != NULL || !uArgs_validateFlagID(f))
		return ;
	reg->entries[reg->len] = f;
	reg->len++;
}

void
uArgs_popFlag(uArgsRegistry *reg){
	if(reg->len == 0 || reg == NULL)
		return;

	reg->entries[reg->len - 1] = (uArgsFlag){NULL, NULL};
	reg->len--;
}

const uArgsFlag*
uArgs_getFlag(const uArgsRegistry *reg, const char* id){
	if(id == NULL || reg == NULL) return NULL;

	for(size_t i = 0; i < reg->len; i++){
		if(strcmp(id, reg->entries[i].id) == 0)
			return (reg->entries + i);
	}

	return NULL;
}

// Executes flag action, checks for NULL.
void
uArgs_runFlag(const uArgsFlag *f, const char* arg){
	if(f == NULL) return;
	if(f->action == NULL) return;
	f->action(arg);
}

// Scan args using reg as a reference, write contents to a argtype map.
static void
uArgs_scan(uArgsRegistry *reg, TypeArgPair* map, const char** args, size_t n){
	const char* arg = NULL;
	size_t arglen = 0;

	for(size_t i = 0; i < n; i++){
		arg = args[i];
		map[i].arg = arg;
		arglen = strlen(arg);
		if(arglen > 1){
			// short flag
			if(arg[0] == '-' && arg[1] != '-'){
				if(uArgs_getFlag(reg, arg) == NULL){
					map[i].t = SHORT_FLAG;
				}
			}
			// long flag
			else if (arg[0] == '-' && arg[1] == '-' && arglen > 2){
				if(uArgs_getFlag(reg, arg) == NULL){
					map[i].t = LONG_FLAG;
				}
			}
			// arg
			else {
				map[i].t = REGULAR;
			}
		}
		// too short to be a flag
		else {
			map[i].t = REGULAR;
		}
	}
}

void
uArgs_execFlags(uArgsRegistry *reg, const char **args, size_t n){
	if(n == 0) return;
	TypeArgPair typeMap[n + 1];
	const char *arg = NULL;
	const uArgsFlag *flag = NULL;
	size_t arglen = 0;

	uArgs_scan(reg, typeMap, args, n);
	typeMap[n].arg = NULL; // padding
	typeMap[n].t = REGULAR;

	for(size_t i = 0; i < n ; i++){
	 	arg = args[i];
		arglen = strlen(arg);
		switch(typeMap[i].t){
			case SHORT_FLAG:
				for(size_t j = 1; j < arglen; j++){
					char flagbuf[] = {arg[j], '\0'};
					flag = uArgs_getFlag(reg, flagbuf);
					if(flag != NULL)
						uArgs_runFlag(flag, args[i + 1]);
					else if(reg->unknownFlagHandler != NULL)
						reg->unknownFlagHandler(args, n);
				}
			break;

			case LONG_FLAG:
				flag = uArgs_getFlag(reg, arg + 2);
				if(flag != NULL)
					uArgs_runFlag(flag, args[i + 1]);
				else if(reg->unknownFlagHandler != NULL)
						reg->unknownFlagHandler(args, n);
			break;

			default: break;
		}
	}
}

