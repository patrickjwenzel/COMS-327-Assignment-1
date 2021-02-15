#include <stdio.h>

type enum action{
		 save_file,
		 load_file,
} action_t;

/*Valid switches:
 * --save: save file
 * --load: load file
 *
 */


int main(int argc, char *argv[]){
  FILE *f;

  struct {
    int i;
    int j;
  } s = {1, 2}; 
  action_t action;
  
  if(argc != 2){
    fprintf(stderr, "Usage: Give thing");
    return -1;
  }

  if(!strcmp(argv[1], "--save")){
    action = save_file;
  }

  else if(!strcmp(argv[1], "--load")){
    action = load_file;
  }
    
  
  if(!(f = fopen("text_file", "w"))){
    fprintf(stderr, "Failed to open file for writing");
    return -1;
  }

  fprintf(f, "%d %d\n", s.i, s.j); 



  fclose(f);

  if(!(f = fopen("text_file", "r"))){
     fprintf(stderr, "Failed to open file for reading");
     return -1;
  }
  fscanf(f, " %d %d\n", &s.i, &s.j);
  
  fclose(f);

  return 0;
}
