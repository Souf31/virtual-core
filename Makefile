init:
	python3 compilateur.py init_test.s

init128:
	python3 compilateur.py add128_test.s

initlshift:
	python3 compilateur.py lshift64_128_test.s

run: init
	gcc core.c
	./a.out init_test.bin init_state

run_128: init128
	gcc core.c
	./a.out add128_test.bin add128_state

run_lshift: initlshift
	gcc core.c
	./a.out lshift64_128_test.bin lshift64_128_state