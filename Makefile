target:
	clang++ -o matching-engine main.cpp

run: target
	./matching-engine

clean:
	rm matching-engine
