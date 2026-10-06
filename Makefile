CC      = gcc
CXX     = g++
NASM    = nasm
COBC    = cobc
CFLAGS  = -O2 -Wall -Wextra -Iinclude
CXXFLAGS= -O2 -Wall -Wextra -std=c++17 -Iinclude

all: build/generador build/saldos build/analisis

build/checksum.o: src/asm/checksum.asm
	$(NASM) -f elf64 $< -o $@

build/generador: src/c/generador.c build/checksum.o
	$(CC) $(CFLAGS) $^ -o $@

build/saldos: src/cobol/saldos.cob
	$(COBC) -x -O2 $< -o $@

build/analisis: src/cpp/analisis.cpp build/checksum.o
	$(CXX) $(CXXFLAGS) $^ -o $@

run: all
	@echo "=== LedgerForge: ASM + C + COBOL + C++ ==="
	./build/generador 500
	./build/saldos
	./build/analisis

clean:
	rm -f build/* data/*

.PHONY: all run clean
