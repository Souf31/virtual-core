init:
	python3 compilateur.py init_test.s

run: init
	gcc core.c
	./a.out init_test.bin init_state
