
/* C Standard includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Local include */
#include "RC4.h"

/* Swap array elements i=State[i] and b=State[j]. */
static void swap(uint8_t *i, uint8_t *j)
{
	uint8_t temp;

	temp = *i;
	*i = *j;
	*j = temp;
}

/* Key scheduling algorithm. Swap array elements based on the key. */
uint8_t *initCtx(uint8_t *key, uint32_t keylen, struct rc4_ctx *ctx)
{
	uint32_t i, j;
	memset(ctx, 0, sizeof(struct rc4_ctx));

	for (i = j = 0; i < 256; i++) {
		j = (j + ctx->state[i] + key[i % keylen]) % 256;
		swap(&ctx->state[i], &ctx->state[j]);
	}
}

/* Encrypt or Decrypt */
void rc4(struct rc4_ctx *ctx,
         uint8_t  * input,
         uint32_t   len,
         uint8_t  * output)
{
	uint32_t i = ctx->i;
	uint32_t j = ctx->j;
	uint32_t m;
	uint8_t si, sj;

	/* The RC4 algorithm */
	for (m = 0; m < len; m++) {
		i = (i + 1) & 0xff;
		si = ctx->state[i];
		j = (j + si) & 0xff;
		sj = ctx->state[j];
		/* swap(&state[i], &state[j]); */
		ctx->state[i] = sj;
		ctx->state[j] = si;
		/* Xor the key stream value into the input */
		*output++ = *input++ ^ (ctx->state[(si + sj) & 0xff]);
	}

	/* Output new S-box indices */
	ctx->i = i;
	ctx->j = j;
}