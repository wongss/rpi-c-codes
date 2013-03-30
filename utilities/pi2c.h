//pi2c.h

#include <stdint.h>

#define SEMAPHORE_USED 1

#if (SEMAPHORE_USED)
#include <semaphore.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Call this only once before any i2c activity takes place
// name should be "/dev/i2c-1" (for older rev 1 rpi board use "/dev/i2c-0")
int pi2cOpen( const char *name );

// Returns number of bytes sent, or <0 for error
int pi2cSendData( uint8_t addr7, uint16_t n,uint8_t *arr );

// Returns number of bytes received, or <0 for error
int pi2cRecData( uint8_t addr7, uint16_t n, uint8_t *arr );

// Close before exiting your program
int pi2cClose( void );

#ifdef __cplusplus
}
#endif
