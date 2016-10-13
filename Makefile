calculator.bin: calculator.hex
	objcopy -I ihex -O binary ./out/calculator.hex ./out/calculator.bin
calculator.hex: calculator.ihx
	packihx ./out/calculator.ihx > ./out/calculator.hex
calculator.ihx: main.rel
	sdcc -o ./out/calculator.ihx ./out/main.rel
main.rel: main.c
	sdcc -c main.c -o ./out/main.rel