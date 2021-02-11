
#include <stdio.h>
#include <stdlib.h>



#define NUM_ROOMS 12
#define MAX_SIZE 5
// I need to make a dungeon 80 x 21
// make at least 6 rooms, probably just do six
//room oso x wide 3 tall
// must not touch
// outer edges are untouchable

// corridors should connect eachother

//create the room structure



    struct room{
      int xPos;
      int yPos;
      int xsize;
      int ysize;
    };

    struct rock{
      int hardness;
      int xPos;
      int yPos;
    };


  int main(int argc, char *argv[])

  {
    

    struct room rooms[NUM_ROOMS];
    int i;
    for(i=0;i<NUM_ROOMS; i++)
      {
	rooms[i].xpos = (rand()% 80) +1;// creates a random 
	rooms[i].ypos = (rand()%24)+1;
	rooms[i].xSize = (rand()%MAX_SIZE)+4;
	rooms[i].ySize = (rand()%MAX_SIZE)+3;
	// rand gives random values from 0 to 99
	
      }
  }
  
					     
					   
