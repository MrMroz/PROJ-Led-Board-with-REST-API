add_executable(demo
        main.c
        lib/matrix.c
        lib/symbols.c
        )

target_link_libraries(demo
	pico_stdio
        pico_stdlib
        pico_multicore
        pico_cyw43_arch_none
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(demo)

# add url via pico_set_program_url
example_auto_set_url(demo)

pico_enable_stdio_uart(demo 0)
pico_enable_stdio_usb(demo 1)
