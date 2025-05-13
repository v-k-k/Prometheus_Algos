# Prometheus algorithms implemented in C with GoogleTest testing

Install the necessary:
*sudo apt install -y libzip-dev libcurl4-openssl-dev graphviz libsqlite3-dev*

## From Makefile

*make prepare*
*make dependency*
*cmake --build build --config Debug*
*cd build*
*ctest -V .*

## To perform the memory leak check

*valgrind --leak-check=full ./tests/runUnitTests*
