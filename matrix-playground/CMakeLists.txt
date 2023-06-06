add_executable(matrix-playground
        matrix_playground.c
        lib/matrix.c
        lib/symbols.c
        lib/handlers.c
        lib/controller.c
        opt/lwipopts.h
        )

target_link_libraries(matrix-playground
	pico_stdio
        pico_stdlib
        pico_multicore
        picow_http
        pico_cyw43_arch_lwip_threadsafe_background
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(matrix-playground)

# add url via pico_set_program_url
example_auto_set_url(matrix-playground)

pico_enable_stdio_uart(matrix-playground 0)
pico_enable_stdio_usb(matrix-playground 1)

target_include_directories(matrix-playground
	PRIVATE	opt
	PRIVATE ${CMAKE_SOURCE_DIR}/lib/picow-http
)

add_subdirectory(../lib/picow-http picow)

picow_http_gen_handlers(matrix-playground
	${CMAKE_CURRENT_SOURCE_DIR}/static/config.yaml
	${CMAKE_CURRENT_SOURCE_DIR}/static/templates
	${CMAKE_CURRENT_SOURCE_DIR}/static/templates/index.html
)