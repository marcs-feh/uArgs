#ifndef INCLUDE_ARGS_H_
#define INCLUDE_ARGS_H_

#define UARGS_MAX_ENTRIES 128

#include <stddef.h>
#include <stdbool.h>

typedef struct uArgsFlag uArgsFlag;
typedef struct uArgsRegistry uArgsRegistry;
typedef void* (*uArgsFlagAction) (const char* arg);
typedef void* (*uArgsRegisterHandler) (const char** args, size_t n);

struct uArgsFlag {
	// Flag identifier, without any leading '-'.
	const char *id;
	// Action that this flag executes.
	uArgsFlagAction action;
};

struct uArgsRegistry {
	// Entries to registry.
	uArgsFlag entries[UARGS_MAX_ENTRIES];
	// How many entries are being used.
	size_t len;
	// Callback to when some unknown flag is found.
	uArgsRegisterHandler unknownFlagHandler;
	// Callback to when a flag that requires argument does not receive one.
	uArgsRegisterHandler missingArgHandler;
};

// Placeholder function, does nothing.
extern void* uArgs_nohandle(const char** args, size_t n);
// Creates a new registry for flags.
extern uArgsRegistry uArgs_newReg();
// Push flag to registry.
extern void uArgs_pushFlag(uArgsRegistry *reg, uArgsFlag flag);
// Pop flag from registry.
extern void uArgs_popFlag(uArgsRegistry *reg);
// Return pointer to flag of certain id in registry, returns NULL if not found.
extern const uArgsFlag* uArgs_getFlag(const uArgsRegistry *reg, const char* id);
// Executes flag action, checks for NULL.
extern void uArgs_runFlag(const uArgsFlag *f, const char* arg);
// Executes n args using a registry.
extern void uArgs_execFlags(uArgsRegistry *reg, const char** args, size_t n);
// Returns size of flag registry.
extern size_t uArgs_regSize(uArgsRegistry *reg);

#endif
