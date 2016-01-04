/******************************************************************/
/* This is a simple test program. It finds the largest number in  */
/*   a list of integers. 							*/
/******************************************************************/
#define BIGNUM (int *) 0x20000
int LIST[7] = {4, 5, 3, 6, 1, 8, 2};
void TEST_PROGRAM()
{
	int big, i;
	big = LIST[0];
	for ( i = 1; i <= 6; i++)
	{
		if ( LIST[i] > big )
			big = LIST[i];
	}
	*BIGNUM = big;
}


