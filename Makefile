build:
	g++ wifisec.cpp -o test
	./test

.PHONY: clean

clean:
	rm test
