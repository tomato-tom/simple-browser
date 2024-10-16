#!/bin/bash

# Compile the browser
gcc -o browser browser.c `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.1`

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Starting browser..."
    
    # Run the browser
    ./browser
    
    # After the browser is closed, remove the executable
    rm -f browser
    echo "Browser closed. Executable 'browser' has been removed."
else
    echo "Compilation failed. Please check your code for errors."
fi
