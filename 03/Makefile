test_main: main_public.o
	g++ -o $@ $^

main_public.cpp: main.cpp ../common/DataLoader.hpp ../common/helper.hpp
	clang-format -i $^ --
	python ../make_public_code.py main.cpp

%.o: %.cpp
	clang-format -i $^ --
	sed 's#^} // namespace .*#}#' -i $^
	g++ -std=c++14 -Wall -g -c $^ -o $@

test: test_main
	for n in data/*_input.txt ; do \
	time ./test_main $${n} > $${n/input/output} ; \
	diff $${n/input/output} $${n/input/result} ; \
	done

lint: main_public.cpp
	clang-tidy main_public.cpp --

clean:
	rm -fr *.o test_main

