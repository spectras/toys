/** @file
 * Colouring graphical interface.
 *
 * The interface is created to operate on a pre-existing world.
 * It requires a graphical environment supported by libSDL2.
 */
#ifndef COLOURING_H
#define COLOURING_H

#include "world.h"

typedef struct colouring Colouring;     /**< Opaque structure representing the game */
#define COLOURING_MAX_WIDTH     160     /**< Maximum row size allowed by interface */
#define COLOURING_MAX_HEIGHT    120     /**< Maximum column size allowed by interface */
#define COLOURING_MAX_COLORS    15      /**< Maximum number of colors interface can show */

/** Create a colouring graphical interface
 * @param[in] world The world to create a graphical interface for.<br>
 *                  The interface does not assume ownership of the world, the caller
 *                  is responsible for destroying it after destroying the
 *                  interface. World must remain valid during all lifespan of
 *                  the interface.
 * @param[in] turns The number of turns to keep the game running for. Game will
 *                  be ended after that number, whether won or not.
 * @return The newly created interface, or `NULL` on failure.
 * @pre SDL must be initialized, with SDL_VIDEO subsystem started before
 *      creating an interface.
 */
Colouring * colouring_create(World * world, unsigned turns);

/** Destroy a colouring graphical interface
 *
 * All resources owned by the interface are freed. The world it was operating on
 * no longer must remain valid. It is up to the caller to destroy it, though.
 * @param[in] app The interface to destroy.
 */
void colouring_destroy(Colouring * app);

/** Run the interface
 *
 * Enter an event loop. This function blocks until the user quits, an error occurs
 * or the game is finished.
 * @param[in,out] app The interface to process events for.
 * @return `EXIT_SUCCESS` on success, a non-zero error code on failure. This
 *         code is suitable as a return code for main().
 */
int colouring_exec(Colouring * app);

#endif
