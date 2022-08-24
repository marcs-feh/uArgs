#ifndef INCLUDE_ARGS_H_
#define INCLUDE_ARGS_H_

#define ARGS_MAX_ENTRIES 128

#include <stddef.h>

typedef struct argsCLIFlag Flag;
typedef struct flagRegistry FlagRegistry;
typedef void (*FlagAction) (const char* arg);

struct argsCLIFlag {
	const char *id;
	FlagAction action;
};

struct flagRegistry {
	Flag entries[ARGS_MAX_ENTRIES];
	size_t len;
};

// Creates a new registry for flags.
extern FlagRegistry Args_newReg();
// Push flag to registry.
extern void Args_pushFlag(FlagRegistry *reg, Flag flag);
// Pop flag from registry.
extern void Args_popFlag(FlagRegistry *reg);
// Return pointer to flag of certain id in registry, returns NULL if not found.
extern Flag* Args_getFlag(FlagRegistry *reg, const char* id);
// Executes n args using a registry.
extern void Args_execFlags(FlagRegistry *reg, const char** args, size_t n);

#endif
