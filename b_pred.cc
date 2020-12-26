#include "b_pred.h"
#include <stdio.h>
#include <string.h>

//-------------------------------------------------------------------------------------------------------------------------------------------------
unsigned int index_calc_bimodal(unsigned int addr,unsigned int m2)
{
             unsigned int index_bimodal=  ( addr << (30-m2) ) >> (32-m2);
             return index_bimodal;
}

unsigned int prediction_bimodal(unsigned int counter_val)
{
        if(counter_val>=2) return 1;
        else               return 0;
}

unsigned int update_bimodal(unsigned int counter_val, unsigned int taken_nottaken)
{
        if(taken_nottaken)
        {
                if(counter_val<3)       counter_val++;
        }
        if(!taken_nottaken)
        {
                if(counter_val>0)       counter_val--;
        }

        return counter_val;
}
unsigned int is_misprediction_bimodal(unsigned int predict_bimodal, unsigned int taken_nottaken,unsigned int misprediction_count_bimodal)
{

        if(predict_bimodal != taken_nottaken)
                misprediction_count_bimodal++;

        return misprediction_count_bimodal;


}
//-------------------------------------------------------------------------------------------------------------------------------------------------

unsigned int index_calc_gshare(unsigned int addr,unsigned int gbh,unsigned int m1,unsigned int n)
{
             unsigned int index_gshare		=  ( addr << (30-m1) ) >> (32-m1);
	     unsigned int upper_n_gshare	=  index_gshare >> (m1-n);
	     unsigned int xored_index		=  upper_n_gshare ^ gbh ;
	     unsigned int final_index		=  (xored_index << (m1-n) ) | ((index_gshare << (32-m1+n))>>(32-m1+n));

	     //printf("GSHARE index: %d",final_index);
             return final_index;
}

unsigned int prediction_gshare(unsigned int counter_val)
{
        if(counter_val>=2) return 1;
        else               return 0;
}

unsigned int update_gbh(unsigned int taken_nottaken,unsigned int gbh,unsigned int n)
{
	unsigned int right_shift = gbh >> 1			;
	unsigned int temp        = taken_nottaken		;
	unsigned int temp_shift  = temp << (n-1)		;

	//printf("BHR UPDATED: %d\n",temp_shift | right_shift)	;
	return (temp_shift | right_shift)			;

}
unsigned int update_gshare(unsigned int counter_val, unsigned int taken_nottaken)
{
        if(taken_nottaken)
        {
		//printf("old value: %d",counter_val);

                if(counter_val<3)       counter_val++;

		//printf("new value %d\n",counter_val);
        }
        if(!taken_nottaken)
        {
		//printf("old value: %d",counter_val);

                if(counter_val>0)       counter_val--;

		//printf("new value: %d\n",counter_val);
        }

        return counter_val;
}
unsigned int is_misprediction_gshare(unsigned int predict_gshare, unsigned int taken_nottaken,unsigned int misprediction_count_gshare)
{

        if(predict_gshare != taken_nottaken)
                misprediction_count_gshare++;

        return misprediction_count_gshare;


}

//-------------------------------------------------------------------------------------------------------------------------------------------------


unsigned int index_calc_hybrid(unsigned int addr,unsigned int k)
{
             unsigned int index_hybrid=  ( addr << (30-k) ) >> (32-k);
             return index_hybrid;
}

unsigned int prediction_hybrid(unsigned int counter_val)
{
	if(counter_val >=  2)	return 2;
	else			return 1;
}

unsigned int update_chooser(unsigned int counter_val, unsigned int taken_nottaken, unsigned int predict_bimodal, unsigned int predict_gshare)
{
	if(predict_bimodal == taken_nottaken && predict_gshare != taken_nottaken )
	{
		if(counter_val>0)	counter_val--; 
	}
	if(predict_bimodal != taken_nottaken && predict_gshare == taken_nottaken)
	{
		if(counter_val<3)	counter_val++;
	}
	return counter_val;
}
