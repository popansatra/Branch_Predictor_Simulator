#include "cache.h"
#include "b_pred.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



int main(int argc, char *argv[]) {

	unsigned int btb_size;
	unsigned int btb_assoc;
	unsigned int m1,m2,n,k;
	unsigned int rp;
	char *tracefile;
	char *mode;
	mode	= argv[1];
	unsigned int mode_num;

	if(strcmp(mode,"bimodal")==0)
	{
	        m2      	= atoi (argv[2]);
		btb_size	= atoi (argv[3]);
		btb_assoc	= atoi (argv[4]);
		tracefile	=        argv[5];
	 	n		=	0	;
		m1		=	0	;
		k		=	0	;	
		mode_num	=	1	;		
	}

	if(strcmp(mode,"gshare")==0)
	{
	        m1      	= atoi (argv[2]);
		n		= atoi (argv[3]);
		btb_size	= atoi (argv[4]);
		btb_assoc	= atoi (argv[5]);
		tracefile	=        argv[6];
		m2		=	0	;
		k		=	0	;
		mode_num	=	2	;
	
	}

	if(strcmp(mode,"hybrid")==0)
	{
		k		= atoi (argv[2]);
	        m1      	= atoi (argv[3]);
		n		= atoi (argv[4]);
		m2		= atoi (argv[5]);
		btb_size	= atoi (argv[6]);
		btb_assoc	= atoi (argv[7]);
		tracefile	=       argv[8] ;
		mode_num	=	3	;
	
	}

	if(mode_num==1)
	{	
		unsigned int num_set = pow(2,m2); 
	        unsigned int *bpt_bimodal;
		bpt_bimodal= (unsigned int *)malloc(sizeof(unsigned int) * num_set);
		for (unsigned int i=0; i<num_set; i++){
	                       bpt_bimodal[i]= 2;
	        }
	
	
	        cache btb(4,btb_size,btb_assoc,2,0,1);
	        cache l2(4,0,0,2,0,2);
	        cache v(4,0,0,2,0,0);
			
		btb.next=NULL;
		btb.vptr=NULL;
	
		char t_n;
		unsigned int taken_nottaken;
		unsigned int addr;
		unsigned int branch_access=0;
		unsigned int mispredictions_bimodal=0;
		unsigned int mispredictions_btb_miss=0;
		float	     misprediction_rate_bimodal=0.0;
		unsigned int index_bimodal;
		unsigned int predict_bimodal;
		unsigned int btb_hits=0;
		FILE *f;
		f = fopen (tracefile,"r");
		while(!feof(f))
		{
	
			fscanf (f,"%x %c\n",&addr,&t_n);
			if(t_n == 't') taken_nottaken = 1;
			else if(t_n == 'n') taken_nottaken = 0;
			unsigned int flag=btb.read(addr);
			if(btb_size==0 && btb_assoc==0 )
			{
				
			
				index_bimodal = index_calc_bimodal(addr,m2);
				predict_bimodal = prediction_bimodal(bpt_bimodal[index_bimodal]);
				bpt_bimodal[index_bimodal] = update_bimodal(bpt_bimodal[index_bimodal],taken_nottaken);			
				mispredictions_bimodal = is_misprediction_bimodal(predict_bimodal,taken_nottaken,mispredictions_bimodal);
				branch_access++;
			
			}
			else
			{
				if(flag)
				{
                                	index_bimodal = index_calc_bimodal(addr,m2);
                                	predict_bimodal = prediction_bimodal(bpt_bimodal[index_bimodal]);
                                	bpt_bimodal[index_bimodal] = update_bimodal(bpt_bimodal[index_bimodal],taken_nottaken);
                                	mispredictions_bimodal = is_misprediction_bimodal(predict_bimodal,taken_nottaken,mispredictions_bimodal);
                                	btb_hits++;
					branch_access++;
				}
				else
				{
					mispredictions_btb_miss = is_misprediction_bimodal(0,taken_nottaken,mispredictions_btb_miss);
					branch_access++;
				}
			}
		}
		if(btb_size==0 && btb_assoc==0 )
		{
			misprediction_rate_bimodal = (mispredictions_bimodal / float(branch_access))*100 ;
			printf("COMMAND\n");
			printf("./sim %s %d %d %d %s\n",mode,m2,btb_size,btb_assoc,tracefile);
			printf("OUTPUT\n");
			printf("number of predictions:	%d\n",branch_access);
			printf("number of mispredictions:	%d\n",mispredictions_bimodal);
			printf("misprediction rate:	%0.2f\% \n",misprediction_rate_bimodal);
			printf("FINAL BIMODAL CONTENTS\n");
			
			for(unsigned int i=0; i<num_set ; i++)
			{
				printf("%d\t%d\n",i,bpt_bimodal[i]);
			}
		}

		else
		{
			float misprediction_rate_btb = (float(mispredictions_bimodal + mispredictions_btb_miss )/float(branch_access))*100;
                        printf("COMMAND\n");
                        printf("./sim %s %d %d %d %s\n",mode,m2,btb_size,btb_assoc,tracefile);
                        printf("OUTPUT\n");
			printf("size of BTB:	%d\n",btb_size);
			printf("number of branches:  %d\n",branch_access);
			printf("number of predictions from branch predictor: %d\n",btb_hits);
			printf("number of mispredictions from branch predictor: %d\n",mispredictions_bimodal);
			printf("number of branches miss in BTB and taken: %d\n",mispredictions_btb_miss);
			printf("total mispredictions: %d\n",mispredictions_bimodal + mispredictions_btb_miss );
			printf("misprediction rate: %0.2f\% \n",misprediction_rate_btb);
                	printf("\nFINAL BTB CONTENTS\n");
                	btb.print_tag_store();
                        printf("FINAL BIMODAL CONTENTS\n");

                        for(unsigned int i=0; i<num_set ; i++)
                        {
                                printf("%d\t%d\n",i,bpt_bimodal[i]);
                        }


		}
		fclose(f);
	}

	if(mode_num==2)
	{	
		unsigned int num_set = pow(2,m1); 
	        unsigned int *bpt_gshare;
		bpt_gshare= (unsigned int *)malloc(sizeof(unsigned int) * num_set);
		for (unsigned int i=0; i<num_set; i++){
	                       bpt_gshare[i]= 2;
	        }
	
	
	        cache btb(4,btb_size,btb_assoc,2,0,1);
	        cache l2(4,0,0,2,0,2);
	        cache v(4,0,0,2,0,0);
			
		btb.next=NULL;
		btb.vptr=NULL;
	
		char t_n;
		unsigned int taken_nottaken;
		unsigned int addr;
		unsigned int branch_access=0;
		unsigned int mispredictions_gshare=0;
		float	     misprediction_rate_gshare=0.0;
		unsigned int index_gshare;
		unsigned int predict_gshare;
		unsigned int gbh = 0;

		unsigned int mispredictions_btb_miss=0;
		unsigned int btb_hits=0;

		FILE *f;
		f = fopen (tracefile,"r");
		while(!feof(f))
		{
	
			fscanf (f,"%x %c\n",&addr,&t_n);
			if(t_n == 't') taken_nottaken = 1;
			else if(t_n == 'n') taken_nottaken = 0;
			unsigned int flag=btb.read(addr);
			if(btb_size==0 && btb_assoc==0 )
			{
				//printf("PC: %x %c\n",addr,t_n);
			
				index_gshare = index_calc_gshare(addr,gbh,m1,n);
				predict_gshare = prediction_gshare(bpt_gshare[index_gshare]);
				bpt_gshare[index_gshare] = update_gshare(bpt_gshare[index_gshare],taken_nottaken);			
				gbh = update_gbh(taken_nottaken,gbh,n);
				mispredictions_gshare = is_misprediction_gshare(predict_gshare,taken_nottaken,mispredictions_gshare);
				branch_access++;
			
			}
			else
			{
	
				if(flag)
                                {

					index_gshare = index_calc_gshare(addr,gbh,m1,n);
                                	predict_gshare = prediction_gshare(bpt_gshare[index_gshare]);
                                	bpt_gshare[index_gshare] = update_gshare(bpt_gshare[index_gshare],taken_nottaken);
                                	gbh = update_gbh(taken_nottaken,gbh,n);
                                	mispredictions_gshare = is_misprediction_gshare(predict_gshare,taken_nottaken,mispredictions_gshare);
                                	branch_access++;
					btb_hits++;
                                }

                                else
                                {
                                        mispredictions_btb_miss = is_misprediction_gshare(0,taken_nottaken,mispredictions_btb_miss);
                                        branch_access++;
                                }
			}
		}
		
                if(btb_size==0 && btb_assoc==0 )
                {
	
			misprediction_rate_gshare = (mispredictions_gshare / float(branch_access))*100 ;
			printf("COMMAND\n");
			printf("./sim %s %d %d %d %d %s\n",mode,m1,n,btb_size,btb_assoc,tracefile);
			printf("OUTPUT\n");
			printf("number of predictions:	%d\n",branch_access);
			printf("number of mispredictions:	%d\n",mispredictions_gshare);
			printf("misprediction rate:	%0.2f\% \n",misprediction_rate_gshare);
			printf("FINAL GSHARE CONTENTS\n");
			
			for(unsigned int i=0; i<num_set ; i++)
			{
				printf("%d\t%d\n",i,bpt_gshare[i]);
			}
		}

		else
		{
                        float misprediction_rate_btb = (float(mispredictions_gshare + mispredictions_btb_miss )/float(branch_access))*100;
                        printf("COMMAND\n");
                        printf("./sim %s %d %d %d %d %s\n",mode,m1,n,btb_size,btb_assoc,tracefile);
                        printf("OUTPUT\n");
                        printf("size of BTB:    %d\n",btb_size);
                        printf("number of branches:  %d\n",branch_access);
                        printf("number of predictions from branch predictor: %d\n",btb_hits);
                        printf("number of mispredictions from branch predictor: %d\n",mispredictions_gshare);
                        printf("number of branches miss in BTB and taken: %d\n",mispredictions_btb_miss);
                        printf("total mispredictions: %d\n",mispredictions_gshare + mispredictions_btb_miss );
                        printf("misprediction rate: %0.2f\% \n",misprediction_rate_btb);
                        printf("\nFINAL BTB CONTENTS\n");
                        btb.print_tag_store();
                        printf("FINAL GSHARE CONTENTS\n");

                        for(unsigned int i=0; i<num_set ; i++)
                        {
                                printf("%d\t%d\n",i,bpt_gshare[i]);
                        }

		}
		fclose(f);
	}


	if(mode_num==3)
	{	

		unsigned int num_set_chooser = pow(2,k)	;
		unsigned int num_set_bimodal = pow(2,m2); 
		unsigned int num_set_gshare  = pow(2,m1);

		unsigned int *chooser_hybrid	;
	        unsigned int *bpt_gshare	;
		unsigned int *bpt_bimodal	;

		chooser_hybrid	= (unsigned int *)malloc(sizeof(unsigned int) * num_set_chooser);
		bpt_gshare	= (unsigned int *)malloc(sizeof(unsigned int) * num_set_gshare);
		bpt_bimodal	= (unsigned int *)malloc(sizeof(unsigned int) * num_set_bimodal);

		for (unsigned int i=0; i<num_set_gshare; i++){
	                       bpt_gshare[i]= 2;
	        }
		for (unsigned int i=0; i<num_set_bimodal; i++){
                               bpt_bimodal[i]= 2;
                }
		for (unsigned int i=0; i<num_set_chooser; i++){
                               chooser_hybrid[i]= 1;
                }
                cache btb(4,btb_size,btb_assoc,2,0,1);
                cache l2(4,0,0,2,0,2);
                cache v(4,0,0,2,0,0);

                btb.next=NULL;
                btb.vptr=NULL;	
	
		char t_n;
		unsigned int taken_nottaken;
		unsigned int addr;
		unsigned int branch_access=0;

                unsigned int mispredictions_bimodal=0;
                float        misprediction_rate_bimodal=0.0;
                unsigned int index_bimodal;
                unsigned int predict_bimodal;

		unsigned int mispredictions_gshare=0;
		float	     misprediction_rate_gshare=0.0;
		unsigned int index_gshare;
		unsigned int predict_gshare;
		unsigned int gbh = 0;

		unsigned int index_hybrid;
		float  misprediction_rate_hybrid=0.0;
		unsigned int prediction_chosen;
		unsigned int predict_hybrid;

                unsigned int mispredictions_btb_miss=0;
                unsigned int btb_hits=0;



		FILE *f;
		f = fopen (tracefile,"r");
		while(!feof(f))
		{
	
			fscanf (f,"%x %c\n",&addr,&t_n);
			if(t_n == 't') taken_nottaken = 1;
			else if(t_n == 'n') taken_nottaken = 0;
			unsigned int flag=btb.read(addr);

			if(btb_size==0 && btb_assoc==0 )
			{
				//printf("PC: %x %c\n",addr,t_n);
		


				index_bimodal = index_calc_bimodal(addr,m2);
                        	predict_bimodal = prediction_bimodal(bpt_bimodal[index_bimodal]);


				index_gshare = index_calc_gshare(addr,gbh,m1,n);
				predict_gshare = prediction_gshare(bpt_gshare[index_gshare]);


                                index_hybrid = index_calc_hybrid(addr,k);
                                prediction_chosen = prediction_hybrid(chooser_hybrid[index_hybrid]);


				if(prediction_chosen == 1)
				{
					predict_hybrid = predict_bimodal;
	                        	bpt_bimodal[index_bimodal] = update_bimodal(bpt_bimodal[index_bimodal],taken_nottaken);
	                        	mispredictions_bimodal = is_misprediction_bimodal(predict_bimodal,taken_nottaken,mispredictions_bimodal);
					gbh = update_gbh(taken_nottaken,gbh,n);
					branch_access++;
 
				}

				if(prediction_chosen == 2)
				{
					predict_hybrid = predict_gshare;
					bpt_gshare[index_gshare] = update_gshare(bpt_gshare[index_gshare],taken_nottaken);			
					gbh = update_gbh(taken_nottaken,gbh,n);
					mispredictions_gshare = is_misprediction_gshare(predict_gshare,taken_nottaken,mispredictions_gshare);
					branch_access++;

				}

				chooser_hybrid[index_hybrid] = update_chooser(chooser_hybrid[index_hybrid],taken_nottaken, predict_bimodal, predict_gshare);				

						
			}
			else
			{

				if(flag)
				{
                                	index_bimodal = index_calc_bimodal(addr,m2);
                                	predict_bimodal = prediction_bimodal(bpt_bimodal[index_bimodal]);


                                	index_gshare = index_calc_gshare(addr,gbh,m1,n);
                                	predict_gshare = prediction_gshare(bpt_gshare[index_gshare]);


                                	index_hybrid = index_calc_hybrid(addr,k);
                                	prediction_chosen = prediction_hybrid(chooser_hybrid[index_hybrid]);


                                	if(prediction_chosen == 1)
                                	{
                                        	predict_hybrid = predict_bimodal;
                                        	bpt_bimodal[index_bimodal] = update_bimodal(bpt_bimodal[index_bimodal],taken_nottaken);
                                        	mispredictions_bimodal = is_misprediction_bimodal(predict_bimodal,taken_nottaken,mispredictions_bimodal);
                                        	gbh = update_gbh(taken_nottaken,gbh,n);
                                        	//branch_access++;
						btb_hits++;

                                	}	

                                	if(prediction_chosen == 2)
                                	{
                                        	predict_hybrid = predict_gshare;
                                        	bpt_gshare[index_gshare] = update_gshare(bpt_gshare[index_gshare],taken_nottaken);
                                        	gbh = update_gbh(taken_nottaken,gbh,n);
                                        	mispredictions_gshare = is_misprediction_gshare(predict_gshare,taken_nottaken,mispredictions_gshare);
                                        	//branch_access++;
						btb_hits++;

                                	}
					branch_access++;
                                	chooser_hybrid[index_hybrid] = update_chooser(chooser_hybrid[index_hybrid],taken_nottaken, predict_bimodal, predict_gshare);


				
	
				}

				else
				{
					if(prediction_chosen == 2)
                                        {

						mispredictions_btb_miss = is_misprediction_gshare(0,taken_nottaken,mispredictions_btb_miss);
                                        	//branch_access++;
					}
					if(prediction_chosen==1)
                                        {

                                                mispredictions_btb_miss = is_misprediction_bimodal(0,taken_nottaken,mispredictions_btb_miss);
                                        	//branch_access++;
                                        }
					
					branch_access++;
				
				}
			}

		}

		if(btb_size==0 && btb_assoc==0 )
		{
			misprediction_rate_hybrid = ((mispredictions_gshare + mispredictions_bimodal) / float(branch_access))*100 ;
			printf("COMMAND\n");
			printf("./sim %s %d %d %d %d %d %d %s\n",mode,k,m1,n,m2,btb_size,btb_assoc,tracefile);
			printf("OUTPUT\n");
			printf("number of predictions:	%d\n",branch_access);
			printf("number of mispredictions:	%d\n",mispredictions_gshare+mispredictions_bimodal);
			printf("misprediction rate:	%0.2f\% \n",misprediction_rate_hybrid);
			printf("FINAL CHOOSER CONTENTS\n");
			
			for(unsigned int i=0; i<num_set_chooser ; i++)
			{
				printf("%d\t%d\n",i,chooser_hybrid[i]);
			}

			printf("FINAL GSHARE CONTENTS\n");
			
			for(unsigned int i=0; i<num_set_gshare ; i++)
        	        {
        	                printf("%d\t%d\n",i,bpt_gshare[i]);
        	        }
			
			printf("FINAL BIMODAL CONTENTS\n");
			for(unsigned int i=0; i<num_set_bimodal ; i++)
        	        {
        	                printf("%d\t%d\n",i,bpt_bimodal[i]);
        	        }

		}
                else
                {
                        float misprediction_rate_btb = (float(mispredictions_gshare+ mispredictions_bimodal + mispredictions_btb_miss )/float(branch_access))*100;
                        printf("COMMAND\n");
                        printf("./sim %s %d %d %d %d %d %d %s\n",mode,k,m1,n,m2,btb_size,btb_assoc,tracefile);
                        printf("OUTPUT\n");
                        printf("size of BTB:    %d\n",btb_size);
                        printf("number of branches:  %d\n",branch_access);
                        printf("number of predictions from branch predictor: %d\n",btb_hits);
                        printf("number of mispredictions from branch predictor: %d\n",mispredictions_gshare+mispredictions_bimodal);
                        printf("number of branches miss in BTB and taken: %d\n",mispredictions_btb_miss);
                        printf("total mispredictions: %d\n",mispredictions_gshare+mispredictions_bimodal + mispredictions_btb_miss );
                        printf("misprediction rate: %0.2f\% \n",misprediction_rate_btb);
                        printf("\nFINAL BTB CONTENTS\n");
                        btb.print_tag_store();
                        printf("FINAL CHOOSER CONTENTS\n");

                        for(unsigned int i=0; i<num_set_chooser ; i++)
                        {
                                printf("%d\t%d\n",i,chooser_hybrid[i]);
                        }

                        printf("FINAL GSHARE CONTENTS\n");

                        for(unsigned int i=0; i<num_set_gshare ; i++)
                        {
                                printf("%d\t%d\n",i,bpt_gshare[i]);
                        }

                        printf("FINAL BIMODAL CONTENTS\n");
                        for(unsigned int i=0; i<num_set_bimodal ; i++)
                        {
                                printf("%d\t%d\n",i,bpt_bimodal[i]);
                        }
                }
		

                
		fclose(f);
	}
}
