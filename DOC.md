# μArgs Documentation

## Runtime Errors

Any write operation to either flag or registry object is an **unchecked runtime
error**. All operations that cause changes must be handled through the provided
library functions.

Initializing a registry without assigning it to `uArgs_newReg()` is an
**unchecked runtime error**.

Reading from registries or flags is OK.

Each flag has a `uArgsFlagAction` function pointer which takes a `const char*`
as an argument, checking for `NULL` in cases where it is intended to use this
argument must be done *by the user defined function*, because - will **not** do
it.

## Functions

### `uArgsRegistry uArgs_newReg();`

Creates a new registry that can hold up to 128 flags.

### `void uArgs_pushFlag(uArgsRegistry *reg, uArgsFlag flag);`

Push flag to registry, if the flag is invalid, NULL or the registry is at max
capacity, it won't be inserted.

### `void uArgs_popFlag(uArgsRegistry *reg);`

Pop last flag from registry, if registry is empty, nothing happens.

### `const uArgsFlag* uArgs_getFlag(const uArgsRegistry *reg, const char* id);`

Return pointer to flag of certain id in registry, returns `NULL` if not found.

### `void uArgs_runFlag(const uArgsFlag *f, const char* arg);`

Executes flag action with `arg` as its argument, this function will not execute
if `f` is `NULL` or if `f->action` is `NULL`.

### `void uArgs_execFlags(uArgsRegistry *reg, const char** args, size_t n);`

Executes n args using a registry. This will loop over `args` `n` times and for
each flag encoutered it will check if it is available in `reg`, if it is, it
will use `uArgs_runFlag()` and pass the next argument to it, if it is a flag
that is not in the registry, it will attempt to call the registry's
`unknownFlagHandler`, if the handler is `NULL`, nothing happens.

### `size_t uArgs_regSize(uArgsRegistry *reg);`

Returns size of flag registry.

## Definitions

```c
#define UARGS_MAX_ENTRIES 128

typedef struct uArgsFlag uArgsFlag;
typedef struct uArgsRegistry uArgsRegistry;
typedef void (*uArgsFlagAction) (const char* arg);
typedef void (*uArgsRegisterHandler) (const char** args, size_t n);

struct uArgsFlag {
	const char *id;
	uArgsFlagAction action;
};

struct uArgsRegistry {
	uArgsFlag entries[UARGS_MAX_ENTRIES];
	size_t len;
	uArgsRegisterHandler unknownFlagHandler;
};
```

