/***************************************************
* Programmer: Emil Gram Jensen                                           
* Class: Software (Gruppe 8)               
* Programming Assignment: Eksamensopgave 2										 
* Date: 28/11/21                                                        
/***************************************************/

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// CONSTANTS
#define MAX_DICE 50
#define MIN_DICE 5
#define ROUNDS 15 // YATZY HAS 15 ROUNDS


// STRUCT TO SAVE A ROUND (name, dice, points)
typedef struct Rounds {
    char* name;
    int numbers[MAX_DICE];
    int points;
} Round;

// PROTOTYPES
void roll_multiple_dice(Round rounds[], int n);
int scan_dice();
void clear_console();
void play_game(Round rounds[], int n);
char* get_round_name(int round);
int calculate_points_from_round(int round, int n, int dice[]);
int number_of_times(int round, int n, int dice[]);
int pair_check(int n, int dice[]);
int double_or_house_check_pair(int n, int dice[], int house);
int kind(int n, int dice[], int number);
int straight(int n, int dice[], int start, int end);
int sum_of_dice(int n, int dice[]);
int yatzy(int n, int dice[]);
int check_for_bonus(Round rounds[]);
int total_score(Round rounds[], int bonus);
void scoreboard(Round rounds[], int bonus);

int main(void) {

    clear_console();

    srand(time(NULL));

    // STRUCT ARRAY OF ROUNDS
    Round rounds[ROUNDS];

    int n = -1, bonus = 0;

    while(n != 0) {
        n = scan_dice();

        if(n) {
            roll_multiple_dice(rounds, n);

            play_game(rounds, n);

            if(check_for_bonus(rounds)) {
                bonus = 50;
            }

            scoreboard(rounds, bonus);

            printf("\n\nSPIL IGEN? TAST '0' FOR AT AFSLUTTE PROGRAMMET.");
        }
    }

}

 /************************************************************
 * Function: scan_dice()						     
 * Description: Asks the user how many dice the user wants to roll	         
 * Input parameters: none									 
 * Returns: how many dice the user wants to roll or 0 for stop									        
 *************************************************************/
int scan_dice() {

    int n = -1;

    while(n == -1) {
        
        printf("\nHvor mange terninger vil du kaste? (imellem %d og %d)\n", MIN_DICE, MAX_DICE);
        scanf(" %d", &n);

        if(n == 0) {
            return 0;
        }

        if(n < MIN_DICE || n > MAX_DICE) {
            n = -1;
        }

    }

    clear_console();
    return n;

}

 /************************************************************
 * Function: play_Game(Round rounds[], int n)						     
 * Description: Calculate and prints the points from each round	         
 * Input parameters: rounds struct array and n dice									 
 * Returns: round points to struct array									        
 *************************************************************/
void play_game(Round rounds[], int n) {

    printf("Printing dies:\n");

    for(int i = 0; i < ROUNDS; i++) {

        printf("%s", rounds[i].name);

        for(int ii = 0; ii < n; ii++) {

            printf(" %d ", rounds[i].numbers[ii]);

        }

        int points = calculate_points_from_round(i, n, rounds[i].numbers);

        if(points > 5*(i+1)) {
            points = 5*(i+1);
        }
        rounds[i].points = points;

        printf(" (%d point)", points);

        printf("\n");

    }

}

 /************************************************************
 * Function: score_board(Round rounds[], int bonus)						     
 * Description: Prints the scoreboard	         
 * Input parameters: rounds struct array and the bonus								 
 * Returns: none									        
 *************************************************************/
void scoreboard(Round rounds[], int bonus) {

    printf("\n\nSCORE BOARD\n");

    for(int i = 0; i < ROUNDS; i++) {

        printf("%s : %d\n", rounds[i].name, rounds[i].points);

        if(i == 5) {
            printf("BONUS : %d\n\n", bonus);
        }
        
    }

    printf("TOTAL: %d\n", total_score(rounds, bonus));

}

 /************************************************************
 * Function: total_score(Round rounds[], int bonus)						     
 * Description: Calculates the total score	         
 * Input parameters: rounds struct array and the bonus								 
 * Returns: the total score									        
 *************************************************************/
int total_score(Round rounds[], int bonus) {

    int sum = 0;

    for(int i = 0; i < ROUNDS; i++) {
        sum += rounds[i].points;
    }
    
    sum += bonus;

    return sum;

}

 /************************************************************
 * Function: check_for_bonus(Round rounds[])						     
 * Description: Checks if the player should have bonus points        
 * Input parameters: rounds struct array							 
 * Returns: the bonus									        
 *************************************************************/
int check_for_bonus(Round rounds[]) {

    int sum = 0;

    for(int i = 0; i < 6; i++) {
        sum += rounds[i].points;
    }
    
    if(sum >= 63) {
        return 1;
    }

    return 0;

}

 /************************************************************
 * Function: straight(int n, int dice[], int start, int end)						     
 * Description: Checks if the player has hit a straight (small or large)        
 * Input parameters: n dice, dice array, start value (1 or 2) and end (5 or 6)						 
 * Returns: the straight points									        
 *************************************************************/
int straight(int n, int dice[], int start, int end) {

    int match = 0;

    for(int i = start-1; i < end; i++) {

        if(number_of_times(i, n, dice) > 0) {
            match++;
        }

    }

    if(match == 5 && start == 1) {
        return 15;
    }

    if(match == 5 && start == 2) {
        return 20;
    }

    return 0;
}

 /************************************************************
 * Function: calculate_points_from_round(int round, int n, int dice[])						     
 * Description: Checks which round we have reached and calculates the points       
 * Input parameters: current round, n dice, dice array						 
 * Returns: the points from that round								        
 *************************************************************/
int calculate_points_from_round(int round, int n, int dice[]) {

    switch (round) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return number_of_times(round, n, dice) * (round+1); // The sum of all dice showing the round number (1-6)
        break;
    case 6:
        return pair_check(n, dice); // One Pair
        break;
    case 7:
        return double_or_house_check_pair(n, dice, 1); // Two Pair
        break;
    case 8:
        return kind(n, dice, 3); // Three of a Kind
        break;
    case 9:
        return kind(n, dice, 4); // Four of a Kind
        break;
    case 10:
        return straight(n, dice, 1, 5); // Small Straight (1+2+3+4+5)
        break;
    case 11:
        return straight(n, dice, 2, 6); // Large Straight (2+3+4+5+6)
        break;
    case 12:
        return double_or_house_check_pair(n, dice, 2); // Full House
        break;
    case 13:
        return sum_of_dice(n, dice);
        break;
    case 14:
        return yatzy(n, dice);
        break;
    default:
        return 0;
        break;
    }
}

 /************************************************************
 * Function: pair_check(int n, int dice[])						     
 * Description: Checks if the player has a pair      
 * Input parameters: n dice, dice array						 
 * Returns: the points if there is a pair							        
 *************************************************************/
int pair_check(int n, int dice[]) {

    int pair = 0;

    for(int i = 0; i < 6; i++) {
        
        if(number_of_times(i, n, dice) > 1) {
            pair = i+1;
        }

    }

    return pair+pair;

}

 /************************************************************
 * Function: zatzy(int n, int dice[])						     
 * Description: Checks if the player has yatzy     
 * Input parameters: n dice, dice array						 
 * Returns: the points if there is yatzy							        
 *************************************************************/
int yatzy(int n, int dice[]) {

    for(int i = 0; i < 6; i++) {
        
        if(number_of_times(i, n, dice) > 4) {
            return 50;
        }

    }

    return 0;

}

 /************************************************************
 * Function: kind(int n, int dice[], int number)						     
 * Description: Checks if the player has 3 or 4 of a kind     
 * Input parameters: n dice, dice array and number (3 or 4)					 
 * Returns: the points if there is 3 or 4 kind							        
 *************************************************************/
int kind(int n, int dice[], int number) {

    int kind = 0;

    for(int i = 0; i < 6; i++) {
        
        if(number_of_times(i, n, dice) >= number) {
            kind = i+1;
        }

    }

    return kind*number;

}

 /************************************************************
 * Function: sum_of_dice(int n, int dice[])						     
 * Description: Calculates the sum of the 5 largest dice     
 * Input parameters: n dice, dice array				 
 * Returns: the largest sum of dice							        
 *************************************************************/
int sum_of_dice(int n, int dice[]) {

    int sum = 0, needed = 5;

    for(int i = 5; i >= 0; i--) {

        int numberOfTimes = number_of_times(i, n, dice);

        if(numberOfTimes > needed) {
            
            sum += (i+1)*needed;
            needed = 0;

        } else {

            sum += (i+1)*numberOfTimes;
            needed -= numberOfTimes;

        }

    }

    return sum;

}

 /************************************************************
 * Function: double_or_house_check_pair(int n, int dice[], int biggest)						     
 * Description:      
 * Input parameters: n dice, dice array, biggest number (1 or 2)			 
 * Returns: sum of the pairs							        
 *************************************************************/
int double_or_house_check_pair(int n, int dice[], int biggest) {

    int pair1 = 0, pair2 = 0;

    for(int i = 0; i < 6; i++) {
        
        if(number_of_times(i, n, dice) > biggest) {
            pair1 = i+1;
        }

    }

    for(int i = 0; i < 6; i++) {

        if(number_of_times(i, n, dice) > 1 && i+1 != pair1) {
            pair2 = i+1;
        }

    }

    if(pair1 && pair2 && biggest == 2) {
        return pair1+pair1+pair1+pair2+pair2;
    }

    if(pair1 && pair2) {
        return pair1+pair1+pair2+pair2;
    }

    return 0;

}

 /************************************************************
 * Function: number_of_times(int n, int dice[])						     
 * Description: Checks how many times the dice have been rolled in a round
 * Input parameters: n dice, dice array			 
 * Returns: how many times the dice have been rolled 
 *************************************************************/
int number_of_times(int round, int n, int dice[]) {

    int times = 0;

    for(int i = 0; i < n; i++) {

        if(dice[i] == round+1) {
            times++;
        }

    }

    return times;

}

 /************************************************************
 * Function: roll_multiple_dice(Round rounds[], int n)						     
 * Description: Roll n number of dice and add thems to the rounds struct array
 * Input parameters: rounds struct array and n dice				 
 * Returns: round name and round dice
 *************************************************************/
void roll_multiple_dice(Round rounds[], int n) {

    for(int i = 0; i < ROUNDS; i++) {

        rounds[i].name = get_round_name(i);

        for(int ii = 0; ii < n; ii++) {

            rounds[i].numbers[ii] = (rand() % 6) +1;

        }

    }

}

 /************************************************************
 * Function: get_round_name(int round)						     
 * Description: Finds the name that matches the round
 * Input parameters: round number			 
 * Returns: the name of the round
 *************************************************************/
char* get_round_name(int round) {
  char* name[50] = {"1-ere", "2-ere", "3-ere", "4-ere", "5-ere", "6-ere", "Et par", "To par", "Tre ens", "Fire ens", "Lille", "Stor", "Fuld hus", "Chance", "YATZY"};
  
  return name[round];
}

 /************************************************************
 * Function: clear_console()					     
 * Description: Clears the console
 * Input parameters: none			 
 * Returns: none
 *************************************************************/
void clear_console() {

    #ifdef _WIN32
        system("cls");
    #else //In any other OS
        system("clear");
    #endif

}