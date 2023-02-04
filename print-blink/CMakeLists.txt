add_executable(print_blink
        print_blink.c
        )

target_link_libraries(print_blink
        pico_stdlib              # for core functionality
        pico_cyw43_arch_none     # we need Wifi to access the GPIO, but we don't need anything else
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(print_blink)

# add url via pico_set_program_url
example_auto_set_url(print_blink)

pico_enable_stdio_uart(print_blink 0)
pico_enable_stdio_usb(print_blink 1)
