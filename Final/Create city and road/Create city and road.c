// C program for making the city and road files
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
 
//-----------------Structures and Global variable-------------------------------
//Structure to hold cities
struct city {
   int cityCode;
   char city[20];
};

//Structure to hold roads
struct road {
   int sourceCode;
   int destinationCode;
   float distance;
};

struct city City[50]; //Global variable to hold city names
int cityCounter=-1;   //Counter for city set to -1

struct road Road[1300]; //Global variable to hold road data
int roadCounter=-1;  //Counter for road set to -1
//------------------------------------------------------------------------------

//-------------------Functions to input cities and roads------------------------
//Function to input city
void inputCity() {
   char city[20];
   int i, check=0;
   
   fflush(stdin);
   printf("Please enter the city : ");
   gets(city);
   
   for(i=0;i<=cityCounter;i++) {
      if(strcmp(city,City[i].city) == 0) {
          check=1;                 //If city is present check becomes 0
      }
   }
   
   if(check==0) {      //If city is not present
      cityCounter++; //City counter incresed by 1
      strcpy(City[cityCounter].city,city);      //City addred to structure
      City[cityCounter].cityCode = cityCounter;
   }
   else
      printf("City already present\n");
}

//Function to input road
void inputRoad() {
     int source, destination, check=0, i;
     float distance;
     printf("Please enter the source : ");
     scanf("%d",&source);
     
     printf("Please enter the destination : ");
     scanf("%d",&destination);
     
     printf("Please enter the distance : ");
     scanf("%f",&distance);
     
     if(source<=cityCounter && destination<=cityCounter && source!=destination && source>-1 && destination>-1) {
        for(i=0; i<=roadCounter; i++) {
           if((source == Road[i].sourceCode) && (destination == Road[i].destinationCode))   //Condition for if road already presen
              check=1;
        }
        
        if(check==0) {
           roadCounter++;
           Road[roadCounter].sourceCode = source;
           Road[roadCounter].destinationCode = destination;
           Road[roadCounter].distance = distance;
        }
        
        else
           printf("Road already present");
     }
     
     else if(source>cityCounter || destination>cityCounter || source<0 || destination<0)    //Conditions for wrong data input
        printf("Sorce or destination code is wrong");
     
     else if(source==destination)
        printf("Source and destination cannot be equal");
}

//Function to create text files to store cities
void CityFileMaker() {
    FILE *CityFile;
	CityFile = fopen("City.bin","wb");
	
	if(CityFile != NULL) {
		fwrite(City, sizeof(struct city), cityCounter+1, CityFile );  //Writes the structure to the file 
		fclose(CityFile);
		printf("\nCity file maker successfull. Press enter to continue.....");
        getch();
	}
    else {
		printf("ERROR WRITING\n");
		printf("File not found. Press enter to exit...");
        getch();
        exit(0);
	}
}

//Function to create text file to store roads
void RoadFileMaker() {
    FILE *RoadFile;
	RoadFile = fopen("Road.bin","wb");
	
	if(RoadFile != NULL) {
		fwrite(Road, sizeof(struct road), roadCounter+1, RoadFile );  //Writes the structure to the file 
		fclose(RoadFile);
		printf("\nRoad file maker successfull. Press enter to continue.....");
        getch();
	}
    else {
		printf("ERROR WRITING\n");
		printf("File not found. Press enter to exit...");
        getch();
        exit(0);
	}
}
//-------------------------------------------------------------------------------

//-----------------------------Main function-------------------------------------
int main() {
    int choice;
    while(1) {
       printf("\n\nPlease select an option\n");
       printf("1. Add city\n");
       printf("2. Add road\n");
       printf("3. Make file and exit\n");
       printf("Please select an option : ");
       scanf("%d",&choice);
       
       switch(choice) {
          case 1:
             inputCity();
             break;
          case 2:
             inputRoad();
             break;
          case 3:
             CityFileMaker();
             RoadFileMaker();
             exit(0);
             break;
          default:
             printf("Incorrect choice. Please select again\n");
             break;  
       }       
    }
    return 0;    
}
//--------------------------------------------------------------------------------
