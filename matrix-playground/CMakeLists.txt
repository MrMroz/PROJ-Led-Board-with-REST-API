add_executable(matrix-playground
        matrix_playground.c
        lib/matrix.c
        lib/symbols.c
        )

target_link_libraries(matrix-playground
	pico_stdio
        pico_stdlib
        pico_multicore
        pico_cyw43_arch_none
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(matrix-playground)

# add url via pico_set_program_url
example_auto_set_url(matrix-playground)

pico_enable_stdio_uart(matrix-playground 0)
pico_enable_stdio_usb(matrix-playground 1)
