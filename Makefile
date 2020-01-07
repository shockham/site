python:
	python3 generate.py

rust:
	rustc generate.rs
	./generate

c:
	tcc generate.c
	./a.out

go:
	go run generate.go
