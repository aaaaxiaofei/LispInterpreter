amr_csr_serial: lisp.o tokenizer.o sexp.o
	g++ -o lisp lisp.o tokenizer.o sexp.o 

lisp.o: lisp.cpp tokenizer.hpp sexp.hpp
	g++ -c lisp.cpp tokenizer.cpp sexp.cpp -std=gnu++0x

tokenizer.o: tokenizer.hpp
	g++ -c tokenizer.cpp -std=gnu++0x

sexp.o: sexp.hpp
	g++ -c sexp.cpp -std=gnu++0x

clean: 
	rm lisp lisp.o tokenizer.o sexp.o
