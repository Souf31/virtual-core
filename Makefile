init:
	python3 compilateur.py lshift64_128_test.s

run: init
	gcc core.c
	./a.out lshift64_128_test.bin lshift64_128_state
