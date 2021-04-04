# fhdm-mtof171000c0-pico
A C library for controlling the Sharp MTOF171000C0 Time-of-flight (TOF) distance sensor using a Raspberry Pi Pico

## Quick Start

### Connect the Pico to your MTOF171000C0

- VDD of the MTOF171000C0 to 3.3V OUT of the Pico (pin 36)
- GND to GND
- RXD to GPIO 8
- TXD to GPIO 9


### Configure your CMakeLists.txt file
```
...

# Add the repository as a subdirectory
# eg.
add_subdirectory(/some-path-on-my-computer/mtof171000c0-pico/lib build)

# Include the library to target_link_libraries
# eg.
target_link_libraries(mytarget 
    fhdm-mtof171000c0-pico
    pico_stdlib 
)
...
```

### Use the library

```
int main() {

    //  Declare a variable of type 'struct fhdm_mtof171000c0_pico' and initialise it:

    struct fhdm_mtof171000c0 mtof;
    fhdm_mtof171000c0_pico_new(&mtof, uart1, 8, 9);

    // Read the distance. A return value of 8888 indicates that the distance could not be determined.

    while (1)
    {
        int distance;
        if (mtof.get_distance(&mtof, &distance)) {
            printf("Distance=%d\n", distance);
            sleep_ms(1 * 1000);
        }
    }

    return 0;
}
```

