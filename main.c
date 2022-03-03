/*Written by: Dave Dominique */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "leak_detector_c.h"

#define MAX_LENGTH 50

typedef struct monster {
	char *name;
	char *element;
	int population;
} monster;

typedef struct region {
	char *name;
	int nmonsters;
	int total_population;
	monster **monsters;
} region;

typedef struct itinerary {
	int nregions;
	region **regions;
	int captures;
} itinerary;

typedef struct trainer {
	char *name;
	itinerary *visits;
} trainer;


monster* createMonster(char *name, char *element, int population) {
	//student **courses;
	//this allocates memory for the monster itself
	monster* newMonster = NULL;
	newMonster = (monster*)malloc(sizeof(monster));

	//this allocates memory for the name of the monster
	newMonster->name = (char*)malloc((strlen(name) + 1) * sizeof(char));

	//this alloctes for the element
	newMonster->element = (char*)malloc((strlen(element) + 1) * sizeof(char));

	//this allocates memory for the population(number of monsters)
	newMonster->population = population;

	strcpy(newMonster->name, name);
	strcpy(newMonster->element, element);

	//the thing you want to change is always on the left

	return newMonster;
}


monster** readMonsters(FILE* infile, int *monsterCount) {
	//based on howerver many monsters we have we will be creating an array of monstes using the createMonster
	int pop;
	char name_of_mon[MAX_LENGTH];
	char element[MAX_LENGTH];

	char wasted_word[MAX_LENGTH];
	int mi;

	//here we are creating our "Big Array" that will hold each monster and their data
	monster **monsterList = NULL;

	fscanf(infile, "%d %s", monsterCount, wasted_word);

	monsterList = (monster**)malloc(sizeof(monster *) * *monsterCount); //should i be using monster count or the num_monsters??
	//basically just made monsterList[monsterCount/num_monsters]

	//Now we need to step into each index in the big array and put a  whole monster in each.
	for (mi = 0; mi < *monsterCount; mi++) {

		fscanf(infile, "%s %s %d", name_of_mon, element, &pop);

		//this should input a whole monster into whichever index we're on in the "Big Array"
		monsterList[mi] = createMonster(name_of_mon, element, pop);
	}



	return monsterList;
}


region** readRegions(FILE* infile, int *countRegions, monster** monsterList, int monsterCount)
{

	//number of monsters in each reagion
	int num_mon_region;
	int i;

	char monster_name[MAX_LENGTH], region_name[MAX_LENGTH], wasted_word[MAX_LENGTH], wasted_word_region[MAX_LENGTH];

	//and here weve made our "Big array" that will hold each region depending on the number of them there are
   //and within each region it will hold all the regions info
	fscanf(infile, "%d %s", countRegions, wasted_word_region);

	region **regionList = NULL;
	regionList = (region**)malloc(sizeof(region *) * *countRegions);//basically just made "region regionList[num_region];"

	//now we are stepping into each regions on our list of regions 
	//and placing in the information we want associated to said region
	for (int i = 0; i < *countRegions; i++) {
		//this makes space for a region itself to be held in
		regionList[i] = (region*)malloc(sizeof(region));
		regionList[i]->total_population = 0;
		//here we pull the name of the region from the file, so now we have the name
		fscanf(infile, "%s", region_name);
		//and here we're making a little box to hold the regions name
		regionList[i]->name = (char*)malloc(sizeof(char) * (strlen(region_name) + 1));
		//now here were placing the name into the box we just made
		strcpy(regionList[i]->name, region_name);


		//so this scans in the number of monsters within the given region 
		//so now we know how many monsters there are there
		fscanf(infile, "%d %s", &num_mon_region, wasted_word);

		//now within the said region, we know how many mosters there are there becuase weve put it into the regions set of info
		regionList[i]->nmonsters = num_mon_region;


		//Here weve made the array that will hold each monsters name aka the monster list
		regionList[i]->monsters = (monster**)malloc(sizeof(monster *) * num_mon_region);

		int mli; //monster list index     
		int mons_index;

		//now to step into the array that will be the monster list
		//this should be able to hit every box where a monsters name will be placed
		for (int mli = 0; mli < num_mon_region; mli++) {
			fscanf(infile, "%s", monster_name);

			for (mons_index = 0; mons_index < monsterCount; mons_index++)
			{
				if (strcmp(monster_name, monsterList[mons_index]->name) == 0) {

					regionList[i]->monsters[mli] = monsterList[mons_index];

					regionList[i]->total_population += regionList[i]->monsters[mli]->population;
				}
			}
		}


		// regionList[i]->total_population = ????;

		 //update the total pop in the regiojn

	}


	return regionList;
}


trainer* readTrainers(FILE* infile, int *trainerCount, region** regionList, int *countRegions){
    
    int num_of_trainers;
    char trainer_name[MAX_LENGTH];
    int num_captures;

    
    int nmi;
    char visited_region [MAX_LENGTH];
  
    char wasted_word_trainers[MAX_LENGTH];
    char wasted_word_captures[MAX_LENGTH];
    char wasted_word_regions[MAX_LENGTH];
    
    int vri;//visited regions index
    int ti;// trainer index
    
    //this scans in the number of trainers there are, and then set the number of trainers to trainerCount
    fscanf(infile, "%d %s", &num_of_trainers, wasted_word_trainers);
    *trainerCount = num_of_trainers;
    //this sets up and allocates memory for the "BIG" trainerList
    trainer *trainerList = NULL;
    trainerList = malloc(sizeof(trainer) * *trainerCount);
    
    //now that we have the trainerList ready we now step into their indexes and input information
    
     for (ti = 0; ti < *trainerCount; ti++){
       //here we pull the name of the trainer from the file, so now we have the name
        fscanf(infile, "%s", trainer_name);
        //and here we're making a little box to hold the regions name
        trainerList[ti].name = malloc(sizeof(char) * (strlen(trainer_name)+ 1));
        //(*(trainerList + ti)).name = malloc(sizeof(char) * (strlen(trainer_name)+ 1));
        //now here were placing the name into the box we just made
        strcpy(trainerList[ti].name, trainer_name);
    
        //and here we're making a little box to hold the itinerary
        itinerary* itin = NULL;
        //itin = malloc(sizeof(itinerary));
        
        
        //and here we're making a little box to hold the itinerary
        trainerList[ti].visits = malloc(sizeof(itinerary));
        //getting the number of captures they have
        itin = trainerList[ti].visits;
        
        //why am i getting five for everything??? im scanning in an pointer not an in
        fscanf(infile, "%d %s",&itin->captures, wasted_word_captures);
       
       ////////////////////////////////////////////////////////////////////////
       
       //getting and placing the number of regions they've visited
        fscanf(infile, "%d %s", &itin->nregions, wasted_word_regions);
        //trainerList[ti].visits->nregions = trainerList[ti].visits->nregions;
        ////////////////////////////////////////////////////////////////////////
     
       int caps = itin->captures;
      int *num_regions_ = &itin->nregions;
      trainerList[ti].visits->regions = malloc(*num_regions_ * sizeof(region *));
       
        //here i am inputing the rest of the itinerary data from the region list
        //for every region visited we need to scan in the name of said region based on the visited regions index
        for( vri = 0; vri < *num_regions_; vri++){// here were trying to pull the regions theyve visited from the input file
		    	fscanf(infile, "%s", visited_region);
       
			//we step into the regions to be able to put down a region within the itinerary
			for (nmi = 0; nmi < *countRegions; nmi++)
			{
				if (strcmp(visited_region, regionList[nmi]->name) == 0) {
					// and here were trying to place where theyve visited into the itinerary
						//strcpy(visited_region, regionList[reg_index]->name);
					trainerList[ti].visits->regions[vri] = regionList[nmi];
					break;     

            trainerList[ti].visits->nregions = *countRegions;
		        trainerList[ti].visits->captures = caps;
				}
         
         
      } 
          
    }		
        /////// gcc leak_detector_c.c main.c     
    }
    
    return trainerList;
    
}



void process_inputs(monster** monsterList, int monsterCount, region** regionList,int regionCount,     trainer* trainerList, int trainerCount){
    //***REMINDERS 
            // i would need to check my ti index and make sure its not equal to trainer count -1 ( if statement would be used )
    int mi, ri, ti, buff, buff2;
  	float d_result, f_result, count;
		
    FILE* outfile = fopen("out.txt", "w");
    

    for (ti = 0; ti < trainerCount; ti++) {
		// the outer one will print each individul trainer and their IT
		printf("%s\n", trainerList[ti].name);
		fprintf(outfile, "%s\n", trainerList[ti].name);

		buff = trainerList[ti].visits->nregions;
	   
	   for (ri = 0; ri < buff; ri++){

	        //this will print the number of regions each trainer has visited
	        printf("%s\n",   trainerList[ti].visits->regions[ri]->name);
	        fprintf(outfile, "%s\n", trainerList[ti].visits->regions[ri]->name);
	        
	        for(mi = 0; mi < trainerList[ti].visits->regions[ri]-> nmonsters; mi++){	
              //Here i estimate the number of a given monster M, a trainer will capture in a region R	 
              //Divide the relative population of M in R by R's total relative population.

                  
              
             
            d_result = 1.0 * 
            trainerList[ti].visits->regions[ri]->monsters[mi]->population / trainerList[ti].visits->regions[ri]->total_population;

             // printf("\n%d hello\n",trainerList[ti].visits->regions[ri]->monsters[mi]->population);
            //  printf("\n%d hello\n",regionList[ri]->total_population);
          
              //printf("\n%f hello\n",d_result);
              
              

              //Multiply the result by the total number of captures the trainer intends per region
              f_result = d_result * trainerList[ti].visits->captures;
            
              // Round this result to the nearest integer. 
              count = round(f_result);
                if ( count != 0 ){ 
              //and lastly i must print the caluations in corespodnence with each monster:
              //this should print every monster the trainer has caputred
              printf("%1.0f %s\n", count, trainerList[ti].visits->regions[ri]->monsters[mi]->name);
              fprintf(outfile, "%1.0f %s\n", count, trainerList[ti].visits->regions[ri]->monsters[mi]->name);
	      	 
           }
          }
        }
        printf("\n");
        fprintf(outfile,"\n");
     }
     
      fclose(outfile);
}



void release_memory(monster** monsterList, int monsterCount, region** regionList, int regionCount, trainer* trainerList, int trainerCount) {

	int mi, ri, ti;

	for (mi = 0; mi < monsterCount; mi++) {

		free(monsterList[mi]->name);
		free(monsterList[mi]->element);
		free(monsterList[mi]);


	}

	free(monsterList);


	for (ri = 0; ri < regionCount; ri++) {

		free(regionList[ri]->name);
		free(regionList[ri]->monsters);
		free(regionList[ri]);
	}

	free(regionList);


	for (ti = 0; ti < trainerCount; ti++) {
		free(trainerList[ti].name);
		free(trainerList[ti].visits->regions);
		free(trainerList[ti].visits);

	}

	free(trainerList);
}


int main() {

	atexit(report_mem_leak); //for memory leak detector.
	
	trainer* trainerList;
	monster** monsterList;
	region** regionList;
	int ti, mi, ri, monsterCount, trainerCount, countRegions, regionCount;
	FILE* infile, *outfile;


	
	infile = fopen("in.txt", "r");
	if (infile != NULL)
	{
    
		monsterList = readMonsters(infile, &monsterCount);
		regionList = readRegions(infile, &countRegions, monsterList, monsterCount);
		trainerList = readTrainers(infile, &trainerCount, regionList, &countRegions);

		process_inputs(monsterList, monsterCount, regionList, countRegions, trainerList, trainerCount);
		release_memory(monsterList, monsterCount, regionList, countRegions, trainerList, trainerCount);

		fclose(infile);
	}
	
	
	else
	{
		printf("Please provide correct input file");
		exit(-1);
	}

	return 0;
}
