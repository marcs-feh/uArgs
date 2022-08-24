#ifndef INCLUDE_ARGS_H_
#define INCLUDE_ARGS_H_

#define ARGS_MAX_ENTRIES 128

#include <stddef.h>
#include <stdbool.h>

typedef struct argsCLIFlag Flag;
typedef struct flagRegistry FlagRegistry;
typedef void (*FlagAction) (const char* arg);
typedef void (*FlagHandler) (const char** args, size_t n);

struct argsCLIFlag {
	const char *id;
	FlagAction action;
	bool takesArg;
};

struct flagRegistry {
	// Entries to registry.
	Flag entries[ARGS_MAX_ENTRIES];
	// How many entries are being used.
	size_t len;
	// Callback to when some unknown flag is found.
	FlagHandler unknownFlagHandler;
	// Callback to when a flag that requires argument does not receive one.
	FlagHandler missingArgHandler;
};

// Placeholder function, does nothing.
extern void Args_nop(const char* arg);
// Placeholder function, does nothing.
extern void Args_nohandle(const char** args, size_t n);
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
// Returns size of flag registry.
extern size_t Args_regSize(FlagRegistry *reg);
#endif
