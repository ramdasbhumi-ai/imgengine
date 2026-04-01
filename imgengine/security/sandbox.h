/* security/sandbox.h */
#ifndef IMGENGINE_SECURITY_SANDBOX_H
#define IMGENGINE_SECURITY_SANDBOX_H

#include <stdbool.h>

/**
 * @brief Locks down the process using Seccomp.
 * Disables: execve, socket, open, chmod.
 * Enables: read, write (stdout/err), futex, exit.
 */
bool img_security_enter_sandbox(void);

#endif
