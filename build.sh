g++ -c   -g -DIN_GCC -Wall -Wextra -Wno-long-long -Wno-variadic-macros -Wold-style-definition -Wmissing-format-attribute -I. -DHAVE_CONFIG_H main.cpp -o main.o
gcc   -g -DIN_GCC   -W -Wall -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -pedantic -Wno-long-long -Wno-variadic-macros -Wold-style-definition -Wmissing-format-attribute    -DHAVE_CONFIG_H -I. -I. -I../.././gcc -I../.././gcc/. -I../.././gcc/../include -I../.././gcc/../libcpp/include     \
	-DBASEVER="\"4.1.2\"" -DDATESTAMP="\"\"" \
	-DDEVPHASE="\"\"" -c version.c -o version.o
gcc -c   -g -DIN_GCC   -W -Wall -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -pedantic -Wno-long-long -Wno-variadic-macros -Wold-style-definition -Wmissing-format-attribute    -DHAVE_CONFIG_H -I. -I. -I../.././gcc -I../.././gcc/. -I../.././gcc/../include -I../.././gcc/../libcpp/include     errors.c -o errors.o
g++   -g -DIN_GCC   -W -Wall -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -pedantic -Wno-long-long -Wno-variadic-macros -Wold-style-definition -Wmissing-format-attribute    -DHAVE_CONFIG_H main.o version.o errors.o -liberty -o gnocchi
