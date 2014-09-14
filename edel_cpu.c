#include <stdlib.h>
#include <stdlib.h>
#include <glib.h>
#include "settings.h"
#include "input.h"




int main(int argc, char* argv[])
{



	gint32 i     = 0;
	gint32 x     = 0;

	gint32 a     = 0;
	gint32 b     = 0;
	gint32 c     = 0;
	gint32 d     = 0;
	gint32 e     = 0;
	gint32 g     = 0;
	gint32 fil   = 0;
	double tit   = 0;


	GTimer* timer = g_timer_new();

	gint32* input_vector = (gint32*) malloc(sizeof(gint32)*(input_length +1));
	gint32* counter      = (gint32*) malloc(sizeof(gint32)* 2);
	gint32* pos          = (gint32*) malloc(sizeof(gint32)*(input_length+1));


	

	gint32 noise      = 1;
	gint32 threshhold = 7000; 
	gint32 ener       = 0;
	gint32 expected   = 1;
	gint32 output[10000];
	

	char time_file[1024] = "time_fil%d_new_cpu.txt";
	int positions[10];
	int run = 0;
	
	FILE* file;
	char timefile[1024]= "time_fil32.txt";
                        srand((unsigned) time ( NULL ));

	for(; run <= 8; run++)
	{

			switch(run)
			{
				case 0: filter_length = 2; break;
				case 1: filter_length = 4; break;
				case 2: filter_length = 8; break;
				case 3: filter_length = 16; break;
				case 4: filter_length = 32; break;
				case 5: filter_length = 64; break;
				case 6: filter_length = 128; break;
				case 7: filter_length = 256; break;
				case 8: filter_length = 512; break;
			}
	sprintf(timefile, "time_fil%d.txt", filter_length);
	printf("%d\n", filter_length);	
			d = 0;
			g = 0;
			e = 0;
			a = 0;
			b = 0;
			c = 0;
		
			//Filter
			rand_rects(5,2, input_length, 4*filter_length, 4*filter_length, 4*filter_length, 4*filter_length, base+peak,input_vector,noise,base,0,positions);	


			 gint32* secondder = (gint32*) malloc(sizeof(gint32)*(input_length - 4*filter_length +2));

			g_timer_start(timer);
			for(x=0;x <= filter_length;x++)
			{
				a += input_vector[x];
				b += input_vector[x + filter_length];
				c += input_vector[x + (filter_length << 1)];
			}
			secondder[0] = a - (b << 1) + c;

			for(i = 0; i < input_length - (filter_length << 2) ; i++)
			{
				secondder[i+1] = secondder[i] - input_vector[i] + 3*(input_vector[i + filter_length] - input_vector[i+ (filter_length << 1)])  + input_vector[i+ (filter_length << 1) + filter_length];

				if(secondder[i] >= 0 && secondder[i+1] <0)
				{
					e = 0;
					d = 0;

					for(x = 0; x <= filter_length; x ++)
					{
						d += input_vector[i + x + (filter_length >> 1)];
						e += input_vector[i + x + (filter_length >> 1) + filter_length];
					}
					ener = (e - d)/filter_length;
					if(ener >= threshhold)
					{
						output[g << 1] = ener;
						output[(g << 1) +1] = i;
						printf("energy = %d \n", output[g << 1]);
						printf("time stamp = %d\n", output[(g << 1) +1]);
						g++;
					}
				}
			}
			g_timer_stop(timer);
			file = fopen(timefile, "a+");
			if(time == NULL)
			{
				printf("could not create or open file \n");
				exit(1);

			}
			tit = g_timer_elapsed(timer,NULL);
			fprintf(file,"%f\n", 1000*tit);
			free(secondder);
			fclose(file);
		
		}
	free(input_vector);
	free(counter);
	free(pos);
}
