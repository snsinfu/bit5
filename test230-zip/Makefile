.PHONY: run clean

run: main _man.zip
	./main

clean:
	rm -f main _man.zip

_man.zip:
	zip -r $@ /usr/share/man/man1
