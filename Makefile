
test: tests/test.c
	gcc tests/test.c -o test -lpam -lpam_misc

pam_simple.o: src/pam_simple.c
	gcc src/pam_simple.c -lpam -lpam_misc -c -Wall -Werror -fpic -o pam_simple.o

pam_simple.so: pam_simple.o
	gcc pam_simple.o -shared -o pam_simple.so

clean:
	rm *.o
	rm *.so
	rm test
