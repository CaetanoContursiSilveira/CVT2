compilar: gcc im2col.c loadImage.c loadKernel.c -o im2col -lm
./im2col teste.png kernel2.txt 1 1
Formato: <imagem> <kernel> <divisor dos pixels> <1 para fazer padding>

