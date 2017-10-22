/* dpmonitor.h  
 *
 * The dining philosophers using a monitor
 * implemented with Pthreads
 *
 */

enum {N_PHIL = 5};

void dpInit(void);        // Initialise the monitor
void dpPrintstate(int i); // Print the monitor state
void dpPickup(int i);     // Pick up the resource
void dpPutdown(int i);    // Put down the resource
