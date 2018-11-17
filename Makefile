all:
	make -C 01
	make -C 02
	make -C 04

clean:
	make -C 01 clean
	make -C 02 clean
	make -C 04 clean

lint:
	make -C 01 lint
	make -C 02 lint
	make -C 04 lint
