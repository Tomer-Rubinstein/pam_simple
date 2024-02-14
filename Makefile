utils = src/utils/error_handling.o
parsers = src/parsers/argv_parser.o src/parsers/shadowfile_parser.o

pam_simple.so: pam_simple.o auth.o account.o $(parsers) $(utils)
	gcc pam_simple.o auth.o account.o $(parsers) $(utils) -shared -lcrypt -o pam_simple.so

pam_simple.o: src/pam_simple.c
	gcc src/pam_simple.c -lpam -lpam_misc -c -Wall -Werror -fpic -o pam_simple.o

$(utils): %.o: %.c
	gcc $<  -lshadow -c -Wall -Werror -fpic -o $@

$(parsers): %.o: %.c
	gcc $< -c -Wall -Werror -fpic -o $@

auth.o: src/auth.c
	gcc src/auth.c -lcrypt -c -Wall -Werror -fpic -o auth.o

account.o: src/account.c
	gcc src/account.c -c -Wall -Werror -fpic -o account.o

test: tests/test.c
	sudo gcc tests/test.c -o test -lpam -lpam_misc
	sudo chmod u+s test

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.so' -delete
