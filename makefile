amr_csr_serial: lisp.o tokenizer.o sexp.o parser.o
	g++ -o lisp lisp.o tokenizer.o sexp.o parser.o

lisp.o: lisp.cpp tokenizer.hpp sexp.hpp parser.hpp
	g++ -c lisp.cpp tokenizer.cpp sexp.cpp parser.cpp -std=gnu++0x

tokenizer.o: tokenizer.cpp tokenizer.hpp
	g++ -c tokenizer.cpp -std=gnu++0x

sexp.o: sexp.cpp sexp.hpp
	g++ -c sexp.cpp -std=gnu++0x

parser.o: parser.cpp parser.hpp
	g++ -c parser.cpp -std=gnu++0x

clean: 
	rm lisp lisp.o tokenizer.o sexp.o parser.o
