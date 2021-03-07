#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LEFTROTATE(x,c) (((x) << (c)) | ((x) >> (32 - (c))))

uint32_t h0, h1, h2, h3;

void print_array(uint32_t *array){

	for(int i = 0; i < 64; i++){
		if(i%5 == 0)
			printf("\n");
		printf("%x\t",array[i]);

	}

}

void cal_k(uint32_t *k){

	for(int i = 0; i < 64; i++){
		k[i] = floor(fabs(sin(i+1)) * (pow(2,32)));
	}

}

void md5(uint8_t *initial_msg, size_t initial_len) {


	uint8_t *msg = NULL;
	uint32_t k[64];
	int pad_len;

	uint32_t r[64] = { 7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22, 5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20, 4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23, 6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21};

	h0 = 0x67452301;
	h1 = 0xEFCDAB89;
	h2 = 0x98BADCFE;
	h3 = 0x10325476;

	cal_k(k);

	//print_array(k);

	pad_len = ((((initial_len + 8) / 64) + 1 ) * 64 ) - 8;


	msg = malloc(pad_len + 64);

	memcpy(msg, initial_msg, initial_len);

	msg[initial_len] = 128;

	uint32_t bits_len = 8 * initial_len;


	memcpy(msg + pad_len, &bits_len, 4);


	int idx;
    for(idx=0; idx < pad_len; idx += 64) {

        uint32_t *w = (uint32_t *) (msg + idx);
		uint32_t a,b,c,d;
		a = h0;
		b = h1;
		c = h2;
		d = h3;

		for(uint32_t i = 0; i < 64; i++){
			uint32_t f, g;
			if(i < 16){
				f = (b & c) | ((~b) &d);
				g = i;
			}else if(i < 32){
				f = (d & b) | ((~d) & c);
				g = (5 * i + 1) %16;
			}else if(i < 48){
				f = b ^ c ^ d;
				g = (3 * i + 5) % 16;
			}else{
				f = c ^ (b | (~d));
				g = (7 * i) % 16;
			}

			uint32_t temp;
			temp = d;
			d = c;
			c = b;
			b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
			a = temp;

		}

		h0 = h0 + a;
		h1 = h1 + b;
	 	h2 = h2 + c;
		h3 = h3 + d;

	}

	free(msg);


}


int main(int argc, char **argv){

	if (argc < 2) {
        printf("usage: %s 'string'\n", argv[0]);
        return 1;
    }


	char *msg = argv[1];

	size_t len = strlen(msg);
	md5(msg, len);

	 uint8_t *p;


    p=(uint8_t *)&h0;
    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], h0);

    p=(uint8_t *)&h1;
    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], h1);

    p=(uint8_t *)&h2;
    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], h2);

    p=(uint8_t *)&h3;
    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], h3);
    puts("");

    return 0;


}