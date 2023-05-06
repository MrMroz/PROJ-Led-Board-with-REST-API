add_executable(led_matrix
        led_matrix.c
        )

# pull in common dependencies
target_link_libraries(led_matrix pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(led_matrix)

# add url via pico_set_program_url
example_auto_set_url(led_matrix)
