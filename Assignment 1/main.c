//  main.c
//  Assignment 1
//  Created by S1ngle- on 21.02.2019.
//  Copyright © 2019 S1ngle. All rights reserved.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INFINITY 9999
#define MAX 50
#define STAR -1
FILE *fo; // global variable for output.
/*
 *  This prototype defined here because output function below provide, than Dijkstra algorithm.
 */
void output(int start, int dest, int **pred, int *counter, int flag);


/*
 *  @function getInput takes 4 parameters
 *  @param fp
 *  @param G, which is pointer for adj. matrix
 *  @param initial_city, which is also pointer to startnode in dijkstra algorithm.Initial_city == city from where we starting our way.
 *  @param destination_city, which is also pointer to city where we are want to move from initial city.
 */

int getInput(FILE *fp, int **G, int *initial_city, int *destination_city)
{
    int amount_of_cities; // here we defining our variable, and in the end of function i return variable for feel the field of memory.
    fscanf(fp, "%d", &amount_of_cities);
    if (amount_of_cities < 1)//condition for first number in input,if amount_of_cities less,than 1,it is an error,which i provided in statement
    {
        fprintf(fo, "Structure of the input is invalid");
        exit(0);
    }
    if(amount_of_cities < 5) // minimum is 5 according to assignment constraints
    {
        fprintf(fo,"Number of cities is out of range");
        exit(0);
    }
    fscanf(fp, "%d", initial_city);
    if (*initial_city < 0 || *initial_city >= amount_of_cities) // Initial city cannot be less than 0 and more or equal to amount.
    {                                                           // The same as previous, according to task.
       fprintf(fo,"Chosen initial city does not exist");
        exit(0);
    }
    
    fscanf(fp, "%d", destination_city);
    if (*destination_city < 1 || *destination_city > amount_of_cities) // maximum is 5 because the first city is 0
    {
        fprintf(fo,"Chosen destination city does not exist");
        exit(0);
    }
    
    if (*initial_city == *destination_city)// If we initialize our current position in the same city as where we want to move.
    {
        fprintf(fo, "The shortest path is 0.\n");// Then we do not need to compute it by dijkstra algorithm/
        fprintf(fo, "The number of shortest paths is 1:\n");
        fprintf(fo, "%d -> %d\n", *initial_city, *destination_city);
        exit(0);
        
    }
    char space;
    fscanf(fp, "%c", &space);
    if (space != '\n')// 2 Conditions for check empty lines between 1-st line and matrix.
    {
        fprintf(fo, "Structure of the input is invalid");
        exit(0);
    }
    fscanf(fp, "%c", &space);
    if (space != '\n')
    {
        fprintf(fo, "Structure of the input is invalid");
        exit(0);
    }
    
    for (int i = 0; i < amount_of_cities; i++) // 2 for's, for fill our matrix.
    {
        int counterForSpaces = 0; // here we update everytime our counter for spaces in first for statement.
        for(int j = 0; j < amount_of_cities; j++)
        {
            fscanf(fp,"%d", &G[j][i]); // G[j][i] and not [i][j], because our matrix is transposed.
            char temp; //buffer variable for scanning charachters such as ' ', '\n' and '*'(spaces, end of lines and stars).
            int sos = fscanf(fp, "%c", &temp); // Variable for check EOF
            /*
             * According to assignment, our elements of matrix cannot be more than 20 and less than 0 and cannot be anythning else.
             * Except of star.
             */
            
            
            
            if (i != j && temp != '*' && (G[j][i] > 20 || G[j][i] <= 0))
            {
                fprintf(fo,"The distance between some cities is out of range");
                exit(0);
            }
            if (i == j && G[j][i] != 0) {
                fprintf(fo,"The distance between some cities is out of range");
                exit(0);
            }
            if (sos == EOF) // Check EOF.
            {
                counterForSpaces--;
            }
            if (temp == ' ') //counting amount of spaces.
            {
                counterForSpaces++; // Counter which i defined above.
            }
            /*
             * If our amount of spaces more or equal to our amount of cities, than our matrix have size more, than it should be.
             * For example: if we have matrix 5x5 because we creating a matrix according to our amount_of_cities, it means that,
             * amount of spaces is 4. and if amount_of_cities == 5, and we enter matrix 6x6, than we have 5 spaces. It is forbidden.
             */
            if (counterForSpaces >= amount_of_cities)
            {
                fprintf(fo,"Matrix size does not suit to the number of cities");
                exit(0);
            }
            
            if (temp == '\t') // We cannot enter matrix with tabulation between the elements.
            {
                fprintf(fo,"Structure of the input is invalid");
                exit(0);
            }
            while(temp!= ' ' && temp !='\n' && temp != EOF) // Scanning untill we meet space or a new line
            {
                
                if(temp =='*')
                {
                    G[j][i] = STAR;
                    break;
                }
                if (temp < '0' || temp > '9') // If our matrix is anything else except of normal elements such like numbers.
                {
                    fprintf(fo,"Structure of the input is invalid");
                    exit(0);
                }
                G[j][i] = G[j][i] * 10 + temp - '0'; // Converting characters to numbers
                fscanf(fp, "%c", &temp);
            }
            
        }
    }
    
    return amount_of_cities; // I need to return my amount of cities, because i assign to this variable getInput function
}

int input(int **G, int* startnode, int *finish)
{
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL)
    {
        fprintf(fo,"Structure of the input is invalid");
        exit(0);
    }else{
        int result = getInput(fp, G, startnode, finish); // One more time i assign to variable wich i also return below.
        fclose(fp);
        return result; // Here is my return for dijkstra.
    }
    
}

void dijkstra(int **G)
{
    
    int startnode, destination_city;
    int amount_of_cities =  input(G, &startnode, &destination_city); // Yeah, here it is! Here i return my amount of cities and other.
    int cost[MAX][MAX], distance[MAX];
    int **pred = (int **) malloc(amount_of_cities * sizeof(int *));  // Allocating memory for predeccesor.
    for (int i = 0; i < amount_of_cities; ++i) {
        pred[i] = (int *) malloc(amount_of_cities * sizeof(int));
    }
    int visited[MAX], count,mindistance = 0, nextnode = 0, i, j; // Count gives the number of nodes seen so far.
    for(i = 0; i < amount_of_cities; i++) // Create the cost matrix.
    {
        for(j = 0; j < amount_of_cities; j++){
                cost[i][j] = G[i][j];
        }
    }
    // Pred[][] stores the predecessor of each node.
    // Initialize pred[],distance[] and visited[].
    for(i = 0; i < amount_of_cities; i++)
    {
        distance[i] = cost[startnode][i];
        pred[i][0]= 0;
        visited[i] = 0;
    }
    for (count  = 0; count < amount_of_cities; count++)
    {
        mindistance = INFINITY;
        for(i = 0; i < amount_of_cities; i++)// Nextnode gives the node at minimum distance.
            if(distance[i] < mindistance && !visited[i])
            {
                mindistance = distance[i];
                nextnode = i;
            }
        visited[nextnode] = 1; // Check if a better path exists through nextnode.
        for(i = 0; i < amount_of_cities; i++)
        {
            if(!visited[i] && distance[nextnode] != STAR && G[i][nextnode] != STAR)
            {
                int dist = mindistance + cost[nextnode][i];
                if (dist < distance[i])
                {
                    memset(pred[i], '\0', amount_of_cities * sizeof(int));
                    distance[i] = dist;
                    pred[i][0] = 1;
                    pred[i][1] = nextnode;
                } else if (dist == distance[i])
                {
                    int cnt =  pred[i][0]; // cnt == counter
                    pred[i][cnt + 1] = nextnode;
                    pred[i][0] = cnt + 1;
                }
            }
        }
        
    }
    // After all computations, if i have not ways to cities.
    if (distance[destination_city] == STAR)
    {
        fprintf(fo,"Initial and destination cities are not connected");
        exit(0);
    }
    
    int counterForAmountOfWays = 0;
    /*
     * Calling function for the first time, because i need to count all ways.
     */
    output(startnode, destination_city, pred, &counterForAmountOfWays, 0);
    fprintf(fo, "The shortest path is %d.\n", distance[destination_city]); // Printing minimum distance between cities.
    fprintf(fo, "The number of shortest paths is %d:\n", counterForAmountOfWays); // Printing amount of ways.
    output(startnode, destination_city, pred, &counterForAmountOfWays, 1);
    free(pred); // Deallocating memory for predeccesor.
    
}

void output(int start, int dest, int **pred, int *counter, int flag)
{

    int i, j;
    int depth = 0;
    for (i = 0; i < pred[dest][0]; ++i)
    {
        if (flag == 1)
        {
            fprintf(fo, "%d. ", i + 1);
            fprintf(fo, "%d -> ", start);
            for (j = 0 ; j <= depth; ++j) {
                fprintf(fo,"%d -> %d", pred[dest][i + 1], dest);
            }
            fprintf(fo,"\n");
        }
        (*counter)++;
    }
}


int main()
{
    int **G; // Creating pointer for 2 dimenshion matrix.
    G = (int **) malloc(MAX * sizeof(int *)); // Allocating memory for G.
    for (int i = 0; i < MAX; ++i) {
        G[i] = (int *) malloc(MAX * sizeof(int));
    }
    fo = fopen("output.txt", "w");
    dijkstra(G);
    fclose(fo);
    free(G); // Deallocating memory
    return 0;
}
