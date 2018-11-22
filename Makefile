all: all_01 all_02 all_03 all_04 all_05

all_01:
	make -C 01

all_02:
	make -C 02

all_03:
	make -C 03

all_04:
	make -C 04

all_05:
	make -C 05

clean:
	make -C 01 clean
	make -C 02 clean
	make -C 03 clean
	make -C 04 clean
	make -C 05 clean

lint:
	make -C 01 lint
	make -C 02 lint
	make -C 03 lint
	make -C 04 lint
	make -C 05 lint
