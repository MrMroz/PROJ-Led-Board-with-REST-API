add_executable(multicore_test
        multicore_test.c
        )

target_link_libraries(multicore_test
        pico_stdlib              # for core functionality
        pico_cyw43_arch_none     # we need Wifi to access the GPIO, but we don't need anything 
	pico_multicore
)

# create map/bin/hex file etc.
pico_add_extra_outputs(multicore_test)

# add url via pico_set_program_url
example_auto_set_url(multicore_test)

pico_enable_stdio_uart(multicore_test 0)
pico_enable_stdio_usb(multicore_test 1)
