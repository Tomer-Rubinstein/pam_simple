
utils = src/utils/argv_parser.o src/utils/shadowfile_parser.o src/utils/error_handling.o

$(utils): %.o: %.c
	gcc $< -c -Wall -Werror -fpic -o $@

pam_simple.o: src/pam_simple.c
	gcc src/pam_simple.c -lpam -lpam_misc -c -Wall -Werror -fpic -o pam_simple.o

pam_simple.so: pam_simple.o $(utils)
	gcc pam_simple.o $(utils) -shared -o pam_simple.so

test: tests/test.c
	sudo gcc tests/test.c -o test -lpam -lpam_misc
	sudo chmod u+s test

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.so' -delete
	find . -type f -name 'test' -delete
