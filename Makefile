build:
	gcc tema1.c lista.c folder.c -g -o tema

clean:
	rm tema

checker:
	python3 checker.py
