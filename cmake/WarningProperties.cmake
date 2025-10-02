add_library(warning_properties INTERFACE)

target_compile_options(warning_properties INTERFACE 
    -Wall -Wextra -Wpedantic) 
