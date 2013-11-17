#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/io.h>

/* Thanks to osdev: 
 * http://wiki.osdev.org/PCI#Configuration_Space
 */

/* Configuration address format (32 bits):
 * Enable Bit        [31] 
 * Reserved          [30-24]
 * Bus               [23-16]
 * Device (slot)     [15-11]
 * Function          [10-8]
 * Register (offset) [7-2]
 * Zero              [1]
 * Zero              [0]
 */

#define BUS_SHIFT 16
#define DEV_SHIFT 11 /* Slot     */
#define FUN_SHIFT 8  /* Function */

#define MAX_BUS   256 /* 2^8 */
#define MAX_DEV   32  /* 2^5 */
#define MAX_FUN   8   /* 2^3 */

uint32_t exists(const uint32_t bus, const uint32_t dev, const uint32_t fun)
{
    uint32_t res;
    const uint32_t reg = 0; /* Offset: 0 is "vendor:device" id */
    uintptr_t addr = (0x80000000        |
                     (bus << BUS_SHIFT) |
                     (dev << DEV_SHIFT) |
                     (fun << FUN_SHIFT) |
                     (reg & 0xFC)); /* Low 2 bits are zero */

    /* Query OS's configuration address port */
    outl(addr, 0xCF8);

    /* Read the OS's response */
    res = inl(0xCFC);

    /* If not an error, return the device:vendor ID register value */
    return (res != ((uint32_t)-1)) ? res : 0x0;
}

int main(void)
{
    int i, err;
    uint32_t bus, dev, fun, devven;

    /* Set the low two-bit allowing full access to IO ports */
    if ((err = iopl(3)) < 0)
    {
        perror("Could not set I/O access level");
        exit(err);
    }

    i = 0;
    for (bus=0; bus<MAX_BUS; ++bus)
      for (dev=0; dev<MAX_DEV; ++dev)
        for (fun=0; fun<MAX_FUN; ++fun)
          if ((devven = exists(bus, dev, fun)))
            printf("%d) Bus: 0x%x, Dev: 0x%x. Fun: 0x%x (0x%x:0x%x)\n",
                   ++i, bus, dev, fun, devven >> 16, devven & 0x0000FFFF);

    return 0;
}
