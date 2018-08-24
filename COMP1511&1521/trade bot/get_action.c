#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "trader_bot.h"

#define COMMODITY_INDEX  0
#define COMMODITY_QUANTITY  1
#define COMMODITY_PRICE  2

#define COMMODITY_WEIGHT  3
#define COMMODITY_VOLUME  4
#define BUYER_DISTANCE  3
#define SELLER_DISTANCE  5
#define SELLER_AVA_QUANTITY  6
#define BUYER_AVA_QUANTITY  4
#define SELLER_PROFIT  7
#define RESPECTIVE_BUYER_DISTANCE 8

//FOR THE FUEL STATIONS
#define FUEL_QUANTITY  0
#define FUEL_PRICE  1
#define STATION_DISTANCE  2

#define COMMAND_TYPE  0
#define COMMAND_QUANTITY  1
#define COMMAND_MOVE  2
#define ERROR  0
#define BUY  1
#define SELL  2
#define REFUEL  3
#define MOVE  4

char *get_bot_name(void);
int nearest_fuel(struct bot *b);

char *get_bot_name(void){
	return "Pleb";
}

void get_action(struct bot *b, int *action, int *n){


    //--------------------------MAP----------------------------//

    //DUMMIES FOR PLANNING

    //Location dummy - get reset after each circle of loop. 
    struct location *dummy_loc = b -> location;
    struct bot_list *dummy_bots = dummy_loc -> bots;

    //Cash dummy - get reset after each loop
    int dummy_cash = b -> cash;

    //Name of the start location(not to be confused with the type 0 location) - get reset after each loop
    char*dummy_start_location_name = dummy_loc -> name;

    //Your generic counter for index
    int seller_counter = 0;
    int buyer_counter = 0;
    int station_counter = 0;

    //Let's see how far I can go before I have to refuel, I should also write down the map...
    int dummy_fuel = b -> fuel;
    int dummy_distance = 0;
    int total_distance = 0;
    int last_station = 0;

    //Avoid Running in circle
    int full_circle_warning_flag = 0;
    int commodity_match_flag = 0;
    int enemy_bot_demand = 0;
    int total_item_possible = 0;

    //Record commodity of buyers and sellers for later matching
    char fcommodity_list[MAX_COMMODITIES][MAX_COMMODITY_NAME_CHARS];
    memset(fcommodity_list,'\0',sizeof(fcommodity_list));
    char bcommodity_list[MAX_COMMODITIES][MAX_COMMODITY_NAME_CHARS];
    memset(bcommodity_list,'\0',sizeof(bcommodity_list));
    int commodity_list_index_1 = 0;
    int commodity_list_index_2 = 0;
       
    //==================IF WE ARE GOING FORWARDS===============//

    //Details of all the sellers on the way... And their location
    int fseller_detail[MAX_LOCATIONS + 1][9];
    memset(fseller_detail,-1,sizeof(fseller_detail));

    //Details of all the buyers on the way... And their location
    int fbuyer_detail[MAX_LOCATIONS + 1][4];
    memset(fbuyer_detail,-1,sizeof(fbuyer_detail));

    //Details of all the gas station on the way... And their location
    int fstation_detail[MAX_LOCATIONS + 1][3];
    memset(fstation_detail,-1,sizeof(fstation_detail));


    //==================IF WE ARE GOING BACKWARDS=============-//

    //Details of all the sellers on the way... And their location
    int bseller_detail[MAX_LOCATIONS + 1][9];
    memset(bseller_detail,-1,sizeof(bseller_detail));

    //Details of all the buyers on the way... And their location
    int bbuyer_detail[MAX_LOCATIONS + 1][4];
    memset(bbuyer_detail,-1,sizeof(bbuyer_detail));

    //Details of all the gas station on the way... And their location
    int bstation_detail[MAX_LOCATIONS + 1][3];
    memset(bstation_detail,-1,sizeof(bstation_detail));


    
    //--------------------DRAW THE MAP------------------------//

    //===============Let's go forwards first==================//
    
    //Get infos regarding all the gas stations first, determine the last gas station location on the maximum range I can go.
    while(dummy_fuel > dummy_distance + 1 && full_circle_warning_flag < 2){

        dummy_bots = dummy_loc -> bots;

    	//Make sure if I can run a full circle without ever need to refuel, the loop can still terminate correctly.
    	if(strcmp(dummy_loc -> name, dummy_start_location_name) == 0){
    		full_circle_warning_flag += 1;
    	}
        
        //This is here so I generate a correct new amount of demand each time
        enemy_bot_demand = 0;

        //Check how many bots are in the location which have the ability to buy the item and how much they may want to buy at the current location
        if(dummy_loc -> type == 3){
            //If there is bot
            if(dummy_bots != NULL){

                while(dummy_bots != NULL){
                    //If the bot there is not myself.
                    if(dummy_bots -> bot != b){
                        //If its fuel tank is not full
                        if(dummy_bots -> bot -> fuel < dummy_bots -> bot -> fuel_tank_capacity){
                            //If the poor fella doesn't have enough money to refuel the tank to full
                            if(dummy_bots -> bot -> cash / dummy_loc -> price < (dummy_bots -> bot -> fuel_tank_capacity - dummy_bots -> bot -> fuel)){
                                enemy_bot_demand += dummy_bots -> bot -> cash / dummy_loc -> price;
                            }
                            else{
                                enemy_bot_demand += dummy_bots -> bot -> fuel_tank_capacity - dummy_bots -> bot -> fuel;
                            }
                        }
                    }

                    dummy_bots = dummy_bots -> next;

                }
            }         
        }
        
    	//WOW, I see a gas station! And they still have some fuels left
        //This station must be able to support all the demand of bots at the location as well as the amount of fuel that could cost on my way there. 
        //And I must be able to afford the travel fee to that location.
    	if(dummy_loc -> type == 3 && dummy_loc -> quantity - enemy_bot_demand > dummy_distance 
            && dummy_distance * (dummy_loc -> price) < dummy_cash){
    		//Amount Availabe -> the total quantity minus the demand
    		fstation_detail[station_counter][FUEL_QUANTITY] = dummy_loc -> quantity - enemy_bot_demand;
    		//Gas Price
    		fstation_detail[station_counter][FUEL_PRICE] = dummy_loc -> price;
    		//Distance from the initial location
    		fstation_detail[station_counter][STATION_DISTANCE] = total_distance;      	

    		station_counter += 1;

            //This is here to record the furthest fuel station from my current location
            last_station = total_distance;

    	    //Awesome, here is a gas station and I'm yet eager to return, let's see how much further I can go if I replenish my fuel here.
        	dummy_cash -= dummy_distance * (dummy_loc -> price);
        	dummy_fuel = b -> fuel_tank_capacity;
        	dummy_distance = 0;
    	    
    	}
        
        //move to the next location
    	dummy_loc = dummy_loc -> next;
    	//+1 on the comparative distance
    	dummy_distance += 1;
    	//+1 on the absolute distance
    	total_distance += 1;
    	//-1 fuel;
    	dummy_fuel -= 1;

    }
    
    //-------------------------------------------------------------//

    //Reset location
    dummy_loc = b -> location;
    //Reset absolute distance
    total_distance = 0;
    //Reset warning flag
    full_circle_warning_flag = 0;
   
    //So this time as we already calculated, the bot can go the distance of the last station plus half of its max fuel before it has to go back to the last station and refuel.
    while(total_distance < last_station + (b -> fuel_tank_capacity - 1) / 2 && full_circle_warning_flag < 2){

        enemy_bot_demand = 0;
        total_item_possible = 0;
        commodity_list_index_1 = 0;
        commodity_match_flag = 0;
        dummy_bots = dummy_loc -> bots;

        //Make sure if I can run a full circle without ever need to refuel, the loop can still terminate correctly.
    	if(strcmp(dummy_loc -> name, dummy_start_location_name) == 0){
    		full_circle_warning_flag += 1;
    	}

        //If there is bot in the location
        if(dummy_bots != NULL){
            //Estimate how many items others' bots would buy, and deduce them from the avaliable quantity.
            if(dummy_loc -> type == 1){

                while(dummy_bots != NULL){
                    //If it is not myself
                    if(dummy_bots -> bot != b){
                       enemy_bot_demand += dummy_bots -> bot -> cash / dummy_loc -> price; 
                    }

                    dummy_bots = dummy_bots -> next;

                }

            }
            //Estimate how many items others' bots would sell, and deduce them from the avaliable quantity.
            else if(dummy_loc -> type == 2){
                while(dummy_bots != NULL){
                    //If it is not myself
                    if(dummy_bots -> bot != b){

                        while(dummy_bots -> bot -> cargo != NULL){
                            //If there is same commodity in enemy bot cargo as the ones at the location
                            if(strcmp(dummy_bots -> bot -> cargo -> commodity -> name, dummy_loc -> commodity -> name) == 0){
                                enemy_bot_demand += dummy_bots -> bot -> cargo -> quantity;
                                break;
                            }
                            dummy_bots -> bot -> cargo = dummy_bots -> bot -> cargo -> next;
                        }

                    }
                    dummy_bots = dummy_bots -> next;     
                }
                
            }
        }

        //How many this items can be fit into one's inventory, this is to prevent me from purchasing more than I could sell.
        if(dummy_loc -> type == 1 || dummy_loc -> type == 2){
            total_item_possible = b -> maximum_cargo_weight/dummy_loc -> commodity -> weight;
            if(b -> maximum_cargo_volume/dummy_loc -> commodity -> volume < total_item_possible){
                total_item_possible = b -> maximum_cargo_volume/dummy_loc -> commodity -> volume;
            }
         
            while(fcommodity_list[commodity_list_index_1][0] != '\0'){
                
                if(strcmp(fcommodity_list[commodity_list_index_1],dummy_loc -> commodity -> name) == 0){
                    commodity_match_flag = 1;
                    break;
                }
                commodity_list_index_1 += 1;
            }

            if(commodity_match_flag == 0){
               strcpy(fcommodity_list[commodity_list_index_2],dummy_loc -> commodity -> name);
               commodity_list_index_1 = commodity_list_index_2;
               commodity_list_index_2 += 1;
            }

        }   

    	//WOW, I see a seller! And they still have stuff to sell
    	if(dummy_loc -> type == 1 && dummy_loc -> quantity > enemy_bot_demand){
    		//Index of the Item
    		fseller_detail[seller_counter][COMMODITY_INDEX] = commodity_list_index_1;
    		//Amount Availabe
    		fseller_detail[seller_counter][COMMODITY_QUANTITY] = dummy_loc -> quantity - enemy_bot_demand;
    		//Item Price
    		fseller_detail[seller_counter][COMMODITY_PRICE] = dummy_loc -> price;
    		//Item Weight
    		fseller_detail[seller_counter][COMMODITY_WEIGHT] = dummy_loc -> commodity -> weight;
    		//Item Volume
    		fseller_detail[seller_counter][COMMODITY_VOLUME] = dummy_loc -> commodity -> volume;
    		//Distance from the initial location
    		fseller_detail[seller_counter][SELLER_DISTANCE] = total_distance;
    		seller_counter += 1;
    	}
        
        //WOW, I see a buyer! And they still want to buy something
    	if(dummy_loc -> type == 2 && dummy_loc -> quantity > enemy_bot_demand){
    		//Index of the Item
    		fbuyer_detail[buyer_counter][COMMODITY_INDEX] = commodity_list_index_1;
    		//Amount Availabe
    		fbuyer_detail[buyer_counter][COMMODITY_QUANTITY] = dummy_loc -> quantity - enemy_bot_demand;
    		//Item Price
    		fbuyer_detail[buyer_counter][COMMODITY_PRICE] = dummy_loc -> price;
    		//Distance from the initial location
    		fbuyer_detail[buyer_counter][BUYER_DISTANCE] = total_distance;
    		buyer_counter += 1;
    	}

        //Keep moving
    	dummy_loc = dummy_loc -> next;
    	total_distance += 1;

    }


    //-------------------------------------------------------//


    //===============Let's go backwards now==================//

    //Reset 
    dummy_loc = b -> location;
    dummy_fuel = b -> fuel;
    dummy_cash = b -> cash;

    dummy_distance = 0;
    total_distance = 0;
    last_station = 0;

    seller_counter = 0;
    buyer_counter = 0;
    station_counter = 0;
    
    full_circle_warning_flag = 0;

    commodity_list_index_2 = 0;

    //Get infos regarding all the gas stations first, determine the last gas station location on the maximum range I can go.
    while(dummy_fuel > dummy_distance + 1 && full_circle_warning_flag < 2){

        dummy_bots = dummy_loc -> bots;

        //Make sure if I can run a full circle without ever need to refuel, the loop can still terminate correctly.
        if(strcmp(dummy_loc -> name, dummy_start_location_name) == 0){
            full_circle_warning_flag += 1;
        }
        
        //This is here so I generate a correct new amount of demand each time
        enemy_bot_demand = 0;

        //Check how many bots are in the location which have the ability to buy the item and how much they may want to buy at the current location
        if(dummy_loc -> type == 3){
            //If there is bot
            if(dummy_bots != NULL){

                while(dummy_bots != NULL){
                    //If the bot there is not myself.
                    if(dummy_bots -> bot != b){
                        //If its fuel tank is not full
                        if(dummy_bots -> bot -> fuel < dummy_bots -> bot -> fuel_tank_capacity){
                            //If the poor fella doesn't have enough money to refuel the tank to full
                            if(dummy_bots -> bot -> cash / dummy_loc -> price < (dummy_bots -> bot -> fuel_tank_capacity - dummy_bots -> bot -> fuel)){
                                enemy_bot_demand += dummy_bots -> bot -> cash / dummy_loc -> price;
                            }
                            else{
                                enemy_bot_demand += dummy_bots -> bot -> fuel_tank_capacity - dummy_bots -> bot -> fuel;
                            }
                        }
                    }

                    dummy_bots = dummy_bots -> next;

                }
            }         
        }
        
        //WOW, I see a gas station! And they still have some fuels left
        //This station must be able to support all the demand of bots at the location as well as the amount of fuel that could cost on my way there. 
        //And I must be able to afford the travel fee to that location.
        if(dummy_loc -> type == 3 && dummy_loc -> quantity - enemy_bot_demand > dummy_distance 
            && dummy_distance * (dummy_loc -> price) < dummy_cash){
            //Amount Availabe -> the total quantity minus the demand
            bstation_detail[station_counter][FUEL_QUANTITY] = dummy_loc -> quantity - enemy_bot_demand;
            //Gas Price
            bstation_detail[station_counter][FUEL_PRICE] = dummy_loc -> price;
            //Distance from the initial location
            bstation_detail[station_counter][STATION_DISTANCE] = total_distance;        

            station_counter += 1;

            //This is here to record the furthest fuel station from my current location
            last_station = total_distance;

            //Awesome, here is a gas station and I'm yet eager to return, let's see how much further I can go if I replenish my fuel here.
            dummy_cash -= dummy_distance * (dummy_loc -> price);
            dummy_fuel = b -> fuel_tank_capacity;
            dummy_distance = 0;
            
        }
        
        //move to the next location
        dummy_loc = dummy_loc -> previous;
        //+1 on the comparative distance
        dummy_distance += 1;
        //+1 on the absolute distance
        total_distance += 1;
        //-1 fuel;
        dummy_fuel -= 1;

    }

    //-------------------------------------------------------------//

    //Reset location
    dummy_loc = b -> location;
    //Reset fuel
    dummy_fuel = b -> fuel;
    //Reset absolute distance
    total_distance = 0;
    //Reset station counter
    station_counter = 0;
    //Reset warning flag
    full_circle_warning_flag = 0;
    
    //So this time as we already calculated, the bot can go the distance of the last station plus half of its max fuel before it has to go back to the last station and refuel.
    while(total_distance < last_station + (b -> fuel_tank_capacity - 1) / 2 && full_circle_warning_flag < 2){

        enemy_bot_demand = 0;
        total_item_possible = 0;
        commodity_list_index_1 = 0;
        commodity_match_flag = 0;
        dummy_bots = dummy_loc -> bots;

        //Make sure if I can run a full circle without ever need to refuel, the loop can still terminate correctly.
        if(strcmp(dummy_loc -> name, dummy_start_location_name) == 0){
            full_circle_warning_flag += 1;
        }

        //If there is bot in the location
        if(dummy_bots != NULL){
            //Estimate how many items others' bots would buy, and deduce them from the avaliable quantity.
            if(dummy_loc -> type == 1){

                while(dummy_bots != NULL){
                    //If it is not myself
                    if(dummy_bots -> bot != b){
                       enemy_bot_demand += dummy_bots -> bot -> cash / dummy_loc -> price; 
                    }

                    dummy_bots = dummy_bots -> next;

                }

            }
            //Estimate how many items others' bots would sell, and deduce them from the avaliable quantity.
            else if(dummy_loc -> type == 2){
                while(dummy_bots != NULL){
                    //If it is not myself
                    if(dummy_bots -> bot != b){

                        while(dummy_bots -> bot -> cargo != NULL){
                            //If there is same commodity in enemy bot cargo as the ones at the location
                            if(strcmp(dummy_bots -> bot -> cargo -> commodity -> name, dummy_loc -> commodity -> name) == 0){
                                enemy_bot_demand += dummy_bots -> bot -> cargo -> quantity;
                                break;
                            }
                            dummy_bots -> bot -> cargo = dummy_bots -> bot -> cargo -> next;
                        }

                    }
                    dummy_bots = dummy_bots -> next;     
                }
                
            }
        }

        //How many this items can be fit into one's inventory, this is to prevent me from purchasing more than I could sell.
        if(dummy_loc -> type == 1 || dummy_loc -> type == 2){
            total_item_possible = b -> maximum_cargo_weight/dummy_loc -> commodity -> weight;
            if(b -> maximum_cargo_volume/dummy_loc -> commodity -> volume < total_item_possible){
                total_item_possible = b -> maximum_cargo_volume/dummy_loc -> commodity -> volume;
            }

            while(bcommodity_list[commodity_list_index_1][0] != '\0'){
                if(strcmp(bcommodity_list[commodity_list_index_1],dummy_loc -> commodity -> name) == 0){
                    commodity_match_flag = 1;
                    break;
                }
                commodity_list_index_1 += 1;
            }

            if(commodity_match_flag == 0){
               strcpy(bcommodity_list[commodity_list_index_2],dummy_loc -> commodity -> name);
               commodity_list_index_1 = commodity_list_index_2;
               commodity_list_index_2 += 1;
            }

        }   

        //WOW, I see a seller! And they still have stuff to sell
        if(dummy_loc -> type == 1 && dummy_loc -> quantity > enemy_bot_demand){
            //Index of the Item
            bseller_detail[seller_counter][COMMODITY_INDEX] = commodity_list_index_1;
            //Amount Availabe
            bseller_detail[seller_counter][COMMODITY_QUANTITY] = dummy_loc -> quantity - enemy_bot_demand;
            //Item Price
            bseller_detail[seller_counter][COMMODITY_PRICE] = dummy_loc -> price;
            //Item Weight
            bseller_detail[seller_counter][COMMODITY_WEIGHT] = dummy_loc -> commodity -> weight;
            //Item Volume
            bseller_detail[seller_counter][COMMODITY_VOLUME] = dummy_loc -> commodity -> volume;
            //Distance from the initial location
            bseller_detail[seller_counter][SELLER_DISTANCE] = total_distance;
            seller_counter += 1;
        }
        
        //WOW, I see a buyer! And they still want to buy something
        if(dummy_loc -> type == 2 && dummy_loc -> quantity > enemy_bot_demand){
            //Index of the Item
            bbuyer_detail[buyer_counter][COMMODITY_INDEX] = commodity_list_index_1;
            //Amount Availabe
            bbuyer_detail[buyer_counter][COMMODITY_QUANTITY] = dummy_loc -> quantity - enemy_bot_demand;
            //Item Price
            bbuyer_detail[buyer_counter][COMMODITY_PRICE] = dummy_loc -> price;
            //Distance from the initial location
            bbuyer_detail[buyer_counter][BUYER_DISTANCE] = total_distance;
            buyer_counter += 1;
        }

        //Keep moving
        dummy_loc = dummy_loc -> previous;
        total_distance += 1;

    }


    
    //--------------------------LONG TERM PLAN--------------------------//

    //Reset
    dummy_loc = b -> location;
    dummy_cash = b -> cash;
    seller_counter = 0;
    buyer_counter = 0;
    station_counter = 0;

    //DUMMY!
    int dummy_quantity = 0;
    int dummy_profit = 0;
    int dummy_travel_cost = 0;
    int dummy_distance_far = 0;
    int dummy_last_station_index = 0;
    int dummy_total_turns = 0;
    struct cargo *dummy_cargo = b -> cargo;

    //Thou shall not be changed
    int fhighest_profit = 0;
    int fhighest_profit_index = 0;
    int bhighest_profit = 0;
    int bhighest_profit_index = 0;
    int ftotal_profit = 0;
    int btotal_profit = 0;
    int f_quantity = 0;
    int b_quantity = 0;
    int avaliable_volume = b -> maximum_cargo_volume;
    int avaliable_weight = b -> maximum_cargo_weight;
    int distance_limit_far = 0;
    int distance_limit_close = 0;
    int dummy_highest_buyer_profit = 0;
    int distance_upper_limit = 0;
    int fdistance_lower_limit = MAX_LOCATIONS;
    int bdistance_lower_limit = MAX_LOCATIONS;
    int closest_fitem_quantity = 0;
    int closest_bitem_quantity = 0;
    int sell_flag = 0;

    //===========================Without Cargo=============================//

    if(b -> cargo == NULL){	

        //==============================Forward============================//

    	while(fseller_detail[seller_counter][COMMODITY_INDEX] != -1){
	    	while(fbuyer_detail[buyer_counter][COMMODITY_INDEX] != -1){

	            dummy_quantity = 0;
                dummy_distance_far = 0;
                dummy_distance = 0;
                dummy_last_station_index = 0;
                station_counter = 0;
                dummy_travel_cost = 0; 
                dummy_profit = 0;
                dummy_cash = b -> cash;
                dummy_total_turns = 0;      

	    		//Matching seller and buyer
                if(fseller_detail[seller_counter][COMMODITY_INDEX] == fbuyer_detail[buyer_counter][COMMODITY_INDEX] 
                   && fseller_detail[seller_counter][SELLER_DISTANCE] < fbuyer_detail[buyer_counter][BUYER_DISTANCE]){

                    //How many of this items I can fit in according to the weight
                    dummy_quantity = b -> maximum_cargo_weight / fseller_detail[seller_counter][COMMODITY_WEIGHT];

                    //How many of this items I can fit in according to the volume
                    if((b -> maximum_cargo_volume / fseller_detail[seller_counter][COMMODITY_VOLUME]) < dummy_quantity){
                        dummy_quantity = b -> maximum_cargo_volume / fseller_detail[seller_counter][COMMODITY_VOLUME];
                    }
                     
                    //If the seller has less than I can fit in              
                    if(fseller_detail[seller_counter][COMMODITY_QUANTITY] < dummy_quantity){
                        dummy_quantity = fseller_detail[seller_counter][COMMODITY_QUANTITY];
                    }

                    //If the buyer require less than I can fit in
                    if(fbuyer_detail[buyer_counter][COMMODITY_QUANTITY] < dummy_quantity){
                        dummy_quantity = fbuyer_detail[buyer_counter][COMMODITY_QUANTITY];
                    }

                    //How far do I have to travel to finish this transaction, and how much I would have to pay for the travel                   
                    dummy_distance_far = fbuyer_detail[buyer_counter][BUYER_DISTANCE];

                    //Calculate the cost of travel if refuel at every single fuel station on the way.
                    while(fstation_detail[station_counter][FUEL_QUANTITY] != -1 && fstation_detail[station_counter][STATION_DISTANCE] < dummy_distance_far){

                        dummy_travel_cost += (fstation_detail[station_counter][STATION_DISTANCE] - dummy_distance) * fstation_detail[station_counter][FUEL_PRICE];
                        dummy_last_station_index = station_counter;
                        
                        //This is here so it calculates the right amount of distance after charged.
                        dummy_distance = fstation_detail[station_counter][STATION_DISTANCE];
                        station_counter += 1;

                    }

                    //Estimated total turns for this route
                    dummy_total_turns += dummy_distance_far + station_counter;

                    //I'll also have to be able to fall back to the closest fuel station after I finish the transaction
                    dummy_travel_cost += (dummy_distance_far - fstation_detail[dummy_last_station_index][STATION_DISTANCE]) * fstation_detail[dummy_last_station_index][FUEL_PRICE];         
                    
                    //Check whether I can afford all I can fit after deduce the travel cost from my cash
                    if((dummy_cash - dummy_travel_cost) / fseller_detail[seller_counter][COMMODITY_PRICE] < dummy_quantity){
                        dummy_quantity = (dummy_cash - dummy_travel_cost) / fseller_detail[seller_counter][COMMODITY_PRICE];
                    }                   

                    //Sweet Sweet profit :D
                    //If I can carry out this multiple times in a game
                    if(b -> turns_left / dummy_total_turns > 0){
                        dummy_profit = (b -> turns_left / dummy_total_turns) * 
                        (dummy_quantity * (fbuyer_detail[buyer_counter][COMMODITY_PRICE] 
                        - fseller_detail[seller_counter][COMMODITY_PRICE]) - dummy_travel_cost);
                    }
                    //If not
                    else if(b -> turns_left < dummy_total_turns){
                        dummy_profit = 0;
                    }
                    else{
                        dummy_profit = dummy_quantity * (fbuyer_detail[buyer_counter][COMMODITY_PRICE] 
                        - fseller_detail[seller_counter][COMMODITY_PRICE]) - dummy_travel_cost; 
                    }
                    
                    //Record the highest profit and its respective quantity
                    if(dummy_profit > fseller_detail[seller_counter][SELLER_PROFIT] && dummy_profit > 0){
                        fseller_detail[seller_counter][SELLER_PROFIT] = dummy_profit;
                        fseller_detail[seller_counter][SELLER_AVA_QUANTITY] = dummy_quantity;
                        fseller_detail[seller_counter][RESPECTIVE_BUYER_DISTANCE] = fbuyer_detail[buyer_counter][BUYER_DISTANCE];
                    }                     
                }

	    		buyer_counter += 1;
	    	
            }

            //Calculate the total profit of this route for route picking
            if(fseller_detail[seller_counter][SELLER_PROFIT] > 0){
                ftotal_profit += fseller_detail[seller_counter][SELLER_PROFIT];
            }
            
            //Keep the highest profit and its route for later use
            if(fseller_detail[seller_counter][SELLER_PROFIT] > fhighest_profit){
            	fhighest_profit = fseller_detail[seller_counter][SELLER_PROFIT];
            	fhighest_profit_index = seller_counter;
                f_quantity = fseller_detail[seller_counter][SELLER_AVA_QUANTITY];
            }

	    	seller_counter += 1;

	    }
        
      
	    //==============================Backward============================//

        //Initialize the counters once again
        seller_counter = 0;
        buyer_counter = 0;
        station_counter = 0;

	    while(bseller_detail[seller_counter][COMMODITY_INDEX] != -1){
            while(bbuyer_detail[buyer_counter][COMMODITY_INDEX] != -1){

                dummy_quantity = 0;
                dummy_distance_far = 0;
                dummy_distance = 0;
                dummy_last_station_index = 0;
                station_counter = 0;
                dummy_travel_cost = 0; 
                dummy_profit = 0;
                dummy_cash = b -> cash;
                dummy_total_turns = 0;      

                //Matching seller and buyer
                if(bseller_detail[seller_counter][COMMODITY_INDEX] == bbuyer_detail[buyer_counter][COMMODITY_INDEX] 
                   && bseller_detail[seller_counter][SELLER_DISTANCE] < bbuyer_detail[buyer_counter][BUYER_DISTANCE]){

                    //How many of this items I can fit in according to the weight
                    dummy_quantity = b -> maximum_cargo_weight / bseller_detail[seller_counter][COMMODITY_WEIGHT];

                    //How many of this items I can fit in according to the volume
                    if((b -> maximum_cargo_volume / bseller_detail[seller_counter][COMMODITY_VOLUME]) < dummy_quantity){
                        dummy_quantity = b -> maximum_cargo_volume / bseller_detail[seller_counter][COMMODITY_VOLUME];
                    }
                     
                    //If the seller has less than I can fit in              
                    if(bseller_detail[seller_counter][COMMODITY_QUANTITY] < dummy_quantity){
                        dummy_quantity = bseller_detail[seller_counter][COMMODITY_QUANTITY];
                    }

                    //If the buyer require less than I can fit in
                    if(bbuyer_detail[buyer_counter][COMMODITY_QUANTITY] < dummy_quantity){
                        dummy_quantity = bbuyer_detail[buyer_counter][COMMODITY_QUANTITY];
                    }

                    //How far do I have to travel to finish this transaction, and how much I would have to pay for the travel                   
                    dummy_distance_far = bbuyer_detail[buyer_counter][BUYER_DISTANCE];

                    //Calculate the cost of travel if refuel at every single fuel station on the way.
                    while(bstation_detail[station_counter][FUEL_QUANTITY] != -1 && bstation_detail[station_counter][STATION_DISTANCE] < dummy_distance_far){

                        dummy_travel_cost += (bstation_detail[station_counter][STATION_DISTANCE] - dummy_distance) * bstation_detail[station_counter][FUEL_PRICE];
                        dummy_last_station_index = station_counter;
                        
                        //This is here so it calculates the right amount of distance after charged.
                        dummy_distance = bstation_detail[station_counter][STATION_DISTANCE];
                        station_counter += 1;

                    }

                    //Estimated total turns for this route
                    dummy_total_turns += dummy_distance_far + station_counter;

                    //I'll also have to be able to fall back to the closest fuel station after I finish the transaction
                    dummy_travel_cost += (dummy_distance_far - bstation_detail[dummy_last_station_index][STATION_DISTANCE]) * bstation_detail[dummy_last_station_index][FUEL_PRICE];         
                    
                    //Check whether I can afford all I can fit after deduce the travel cost from my cash
                    if((dummy_cash - dummy_travel_cost) / bseller_detail[seller_counter][COMMODITY_PRICE] < dummy_quantity){
                        dummy_quantity = (dummy_cash - dummy_travel_cost) / bseller_detail[seller_counter][COMMODITY_PRICE];
                    }                   

                    //Sweet Sweet profit :D
                    //If I can carry out this multiple times in a game
                    if(b -> turns_left / dummy_total_turns > 0){
                        dummy_profit = (b -> turns_left / dummy_total_turns) * 
                        (dummy_quantity * (bbuyer_detail[buyer_counter][COMMODITY_PRICE] 
                        - bseller_detail[seller_counter][COMMODITY_PRICE]) - dummy_travel_cost);
                    }
                    //If not
                    else if(b -> turns_left < dummy_total_turns){
                        dummy_profit = 0;
                    }
                    else{
                        dummy_profit = dummy_quantity * (bbuyer_detail[buyer_counter][COMMODITY_PRICE] 
                        - bseller_detail[seller_counter][COMMODITY_PRICE]) - dummy_travel_cost; 
                    }
                    
                    //Record the highest profit and its respective quantity
                    if(dummy_profit > bseller_detail[seller_counter][SELLER_PROFIT] && dummy_profit > 0){
                        bseller_detail[seller_counter][SELLER_PROFIT] = dummy_profit;
                        bseller_detail[seller_counter][SELLER_AVA_QUANTITY] = dummy_quantity;
                        bseller_detail[seller_counter][RESPECTIVE_BUYER_DISTANCE] = bbuyer_detail[buyer_counter][BUYER_DISTANCE];
                    }                     
                }

                buyer_counter += 1;
            
            }

            //Calculate the total profit of this route for route picking
            if(bseller_detail[seller_counter][SELLER_PROFIT] > 0){
                btotal_profit += bseller_detail[seller_counter][SELLER_PROFIT];
            }
            
            //Keep the highest profit and its route for later use
            if(bseller_detail[seller_counter][SELLER_PROFIT] > bhighest_profit){
                bhighest_profit = bseller_detail[seller_counter][SELLER_PROFIT];
                bhighest_profit_index = seller_counter;
                b_quantity = bseller_detail[seller_counter][SELLER_AVA_QUANTITY];
            }

            seller_counter += 1;

        }
   
    }

    //============================With Cargo=============================//
    else{

        //==============================Forward============================//

        buyer_counter = 0;
        distance_upper_limit = 0;
        dummy_cargo = b -> cargo;

        while(dummy_cargo != NULL){
            dummy_highest_buyer_profit = 0;
            sell_flag = 0;
            while(fbuyer_detail[buyer_counter][COMMODITY_INDEX] != -1){

                dummy_quantity = 0;
                dummy_distance_far = 0;
                station_counter = 0;
                //distance_limit_far = 0;
                //distance_limit_close = 0;
                dummy_last_station_index = 0;
                dummy_cash = b -> cash;
                dummy_profit = 0;
                dummy_distance = 0;
                dummy_travel_cost = 0;
                
                                                                                                                    
                //Matching item and buyer
                if(strcmp(dummy_cargo -> commodity -> name,fcommodity_list[fbuyer_detail[buyer_counter][COMMODITY_INDEX]]) == 0){
                    sell_flag = 1;
                    //This is how much I have
                    dummy_quantity = dummy_cargo -> quantity;

                    //If the buyer want less
                    if(fbuyer_detail[buyer_counter][COMMODITY_QUANTITY] < dummy_quantity){
                        dummy_quantity = fbuyer_detail[buyer_counter][COMMODITY_QUANTITY];
                    }

                    //How far do I have to travel to finish this transaction
                    dummy_distance_far = fbuyer_detail[buyer_counter][BUYER_DISTANCE];

                    //Calculate the cost of travel if refuel at every single fuel station on the way.
                    while(fstation_detail[station_counter][FUEL_QUANTITY] != -1 && fstation_detail[station_counter][STATION_DISTANCE] < dummy_distance_far){

                        dummy_travel_cost += (fstation_detail[station_counter][STATION_DISTANCE] - dummy_distance) * fstation_detail[station_counter][FUEL_PRICE];
                        dummy_last_station_index = station_counter;
                        
                        //This is here so it calculates the right amount of distance after charged.
                        dummy_distance = fstation_detail[station_counter][STATION_DISTANCE];
                        station_counter += 1;

                    }
                    
                    //I'll also have to be able to fall back to the closest fuel station after I finish the transaction
                    dummy_travel_cost += (dummy_distance_far - fstation_detail[dummy_last_station_index][STATION_DISTANCE]) * fstation_detail[dummy_last_station_index][FUEL_PRICE];         
                    
                    //Sweet Sweet profit :D
                    dummy_profit = dummy_quantity * fbuyer_detail[buyer_counter][COMMODITY_PRICE] - dummy_travel_cost; 
                    printf("Here are the fb targets: \n");
                    printf("%d %d\n",fbuyer_detail[buyer_counter][BUYER_DISTANCE],dummy_profit);
                    //Found the buyer with the highest profit
                    if(dummy_profit >= dummy_highest_buyer_profit){
                        dummy_highest_buyer_profit = dummy_profit;
                        distance_limit_far = fbuyer_detail[buyer_counter][BUYER_DISTANCE];
                        distance_limit_close = fbuyer_detail[buyer_counter][BUYER_DISTANCE];
                        
                    }                     
                }

                buyer_counter += 1;
            
            }
            
            //Record the furthest buyer location
            if(distance_limit_far > distance_upper_limit && sell_flag == 1){
                distance_upper_limit = distance_limit_far;
            }            
            
            //Record the closest buyer location
            if(distance_limit_close < fdistance_lower_limit && sell_flag == 1){
                fdistance_lower_limit = distance_limit_close;
                printf("*%d*\n",fdistance_lower_limit);
                closest_fitem_quantity = dummy_cargo -> quantity;
            }

            //Calculate the total profit of this route for route picking
            if(dummy_highest_buyer_profit > 0){
                ftotal_profit += dummy_highest_buyer_profit;
            }
            
            //Recondition the avaliable inventory space
            avaliable_weight -=  dummy_cargo -> quantity * dummy_cargo -> commodity -> weight;
            avaliable_volume -=  dummy_cargo -> quantity * dummy_cargo -> commodity -> volume; 

            dummy_cargo = dummy_cargo -> next;

        }
        
        //---------------------------------------------------------//

        //Initialize counters
        seller_counter = 0;
        buyer_counter = 0;
        station_counter = 0;
        fhighest_profit = 0;
        fhighest_profit_index = 0;

        //Searching for one additional possible route in this range
        while(fseller_detail[seller_counter][COMMODITY_INDEX] != -1 && fseller_detail[seller_counter][SELLER_DISTANCE] < distance_upper_limit){
            while(fbuyer_detail[buyer_counter][COMMODITY_INDEX] != -1 && fbuyer_detail[seller_counter][BUYER_DISTANCE] < distance_upper_limit){
                
                dummy_quantity = 0;
                dummy_distance_far = 0;
                dummy_distance = 0;
                dummy_last_station_index = 0;
                station_counter = 0;
                dummy_travel_cost = 0; 
                dummy_profit = 0;
                dummy_cash = b -> cash;

                //Matching seller and buyer
                if(fseller_detail[seller_counter][COMMODITY_INDEX] == fbuyer_detail[buyer_counter][COMMODITY_INDEX] 
                    && fseller_detail[seller_counter][SELLER_DISTANCE] < fbuyer_detail[buyer_counter][BUYER_DISTANCE] 
                    && fseller_detail[seller_counter][SELLER_DISTANCE] != 0){

                    //How many of this items I can fit in according to the weight
                    dummy_quantity = avaliable_weight / fseller_detail[seller_counter][COMMODITY_WEIGHT];

                    //How many of this items I can fit in according to the volume
                    if((avaliable_volume / fseller_detail[seller_counter][COMMODITY_VOLUME]) < dummy_quantity){
                        dummy_quantity = avaliable_volume / fseller_detail[seller_counter][COMMODITY_VOLUME];
                    }
                     
                    //If the seller has less than I can fit in              
                    if(fseller_detail[seller_counter][COMMODITY_QUANTITY] < dummy_quantity){
                        dummy_quantity = fseller_detail[seller_counter][COMMODITY_QUANTITY];
                    }

                    //If the buyer require less than I can fit in
                    if(fbuyer_detail[buyer_counter][COMMODITY_QUANTITY] < dummy_quantity){
                        dummy_quantity = fbuyer_detail[buyer_counter][COMMODITY_QUANTITY];
                    }

                    //How far do I have to travel to finish this transaction, and how much I would have to pay for the travel                   
                    dummy_distance_far = fbuyer_detail[buyer_counter][BUYER_DISTANCE];                 

                    //Calculate the cost of travel if refuel at every single fuel station on the way.
                    while(fstation_detail[station_counter][FUEL_QUANTITY] != -1 && fstation_detail[station_counter][STATION_DISTANCE] < dummy_distance_far){

                        dummy_travel_cost += (fstation_detail[station_counter][STATION_DISTANCE] - dummy_distance) * fstation_detail[station_counter][FUEL_PRICE];
                        dummy_last_station_index = station_counter;
                        
                        //This is here so it calculates the right amount of distance after charged.
                        dummy_distance = fstation_detail[station_counter][STATION_DISTANCE];
                        station_counter += 1;

                    }
                    

                    //I'll also have to be able to fall back to the closest fuel station after I finish the transaction
                    dummy_travel_cost += (dummy_distance_far - fstation_detail[dummy_last_station_index][STATION_DISTANCE]) * fstation_detail[dummy_last_station_index][FUEL_PRICE];         
                    
                    //Check whether I can afford all I can fit after deduce the travel cost from my cash
                    if((dummy_cash - dummy_travel_cost) / fseller_detail[seller_counter][COMMODITY_PRICE] < dummy_quantity){
                        dummy_quantity = (dummy_cash - dummy_travel_cost) / fseller_detail[seller_counter][COMMODITY_PRICE];
                    }                 

                    //Sweet Sweet profit :D
                    dummy_profit = dummy_quantity * (fbuyer_detail[buyer_counter][COMMODITY_PRICE] - fseller_detail[seller_counter][COMMODITY_PRICE]) - dummy_travel_cost; 
                    
                    //Record the highest profit and its respective quantity
                    if(dummy_profit > fseller_detail[seller_counter][SELLER_PROFIT] && dummy_profit > 0){
                        fseller_detail[seller_counter][SELLER_PROFIT] = dummy_profit;
                        fseller_detail[seller_counter][SELLER_AVA_QUANTITY] = dummy_quantity;
                        fseller_detail[seller_counter][RESPECTIVE_BUYER_DISTANCE] = fbuyer_detail[buyer_counter][BUYER_DISTANCE];
                    }                     
                }

                buyer_counter += 1;
            
            }

            //Calculate the total profit of this route for route picking
            if(fseller_detail[seller_counter][SELLER_PROFIT] > 0 && fseller_detail[seller_counter][SELLER_DISTANCE] != 0){
                ftotal_profit += fseller_detail[seller_counter][SELLER_PROFIT];
            }
            
            //Keep the highest profit and its route for later use
            if(fseller_detail[seller_counter][SELLER_PROFIT] > fhighest_profit && fseller_detail[seller_counter][SELLER_DISTANCE] != 0){
                fhighest_profit = fseller_detail[seller_counter][SELLER_PROFIT];
                fhighest_profit_index = seller_counter;
                f_quantity = fseller_detail[seller_counter][SELLER_AVA_QUANTITY];
            }

            seller_counter += 1;

        }


        //==============================Backward============================//

        buyer_counter = 0;
        distance_upper_limit = 0;
        dummy_cargo = b -> cargo;

        while(dummy_cargo != NULL){
            dummy_highest_buyer_profit = 0;
            sell_flag = 0;
            while(bbuyer_detail[buyer_counter][COMMODITY_INDEX] != -1){

                dummy_quantity = 0;
                dummy_distance_far = 0;
                station_counter = 0;
                //distance_limit_far = 0;
                //distance_limit_close = 0;
                dummy_last_station_index = 0;
                dummy_cash = b -> cash;
                dummy_profit = 0;
                dummy_distance = 0;
                dummy_travel_cost = 0;
                
                                                                                                                    
                //Matching item and buyer
                if(strcmp(dummy_cargo -> commodity -> name,bcommodity_list[bbuyer_detail[buyer_counter][COMMODITY_INDEX]]) == 0){
                    sell_flag = 1;
                    //This is how much I have
                    dummy_quantity = dummy_cargo -> quantity;

                    //If the buyer want less
                    if(bbuyer_detail[buyer_counter][COMMODITY_QUANTITY] < dummy_quantity){
                        dummy_quantity = bbuyer_detail[buyer_counter][COMMODITY_QUANTITY];
                    }

                    //How far do I have to travel to finish this transaction
                    dummy_distance_far = bbuyer_detail[buyer_counter][BUYER_DISTANCE];

                    //Calculate the cost of travel if refuel at every single fuel station on the way.
                    while(bstation_detail[station_counter][FUEL_QUANTITY] != -1 && bstation_detail[station_counter][STATION_DISTANCE] < dummy_distance_far){

                        dummy_travel_cost += (bstation_detail[station_counter][STATION_DISTANCE] - dummy_distance) * bstation_detail[station_counter][FUEL_PRICE];
                        dummy_last_station_index = station_counter;
                        
                        //This is here so it calculates the right amount of distance after charged.
                        dummy_distance = bstation_detail[station_counter][STATION_DISTANCE];
                        station_counter += 1;

                    }

                    //I'll also have to be able to fall back to the closest fuel station after I finish the transaction
                    dummy_travel_cost += (dummy_distance_far - bstation_detail[dummy_last_station_index][STATION_DISTANCE]) * bstation_detail[dummy_last_station_index][FUEL_PRICE];         
                    
                    //Sweet Sweet profit :D
                    dummy_profit = dummy_quantity * bbuyer_detail[buyer_counter][COMMODITY_PRICE] - dummy_travel_cost; 
                    printf("Here are the bb targets: \n");
                    printf("%d %d\n",bbuyer_detail[buyer_counter][BUYER_DISTANCE],dummy_profit);
                    //Found the buyer with the highest profit
                    if(dummy_profit >= dummy_highest_buyer_profit){
                        dummy_highest_buyer_profit = dummy_profit;
                        distance_limit_far = bbuyer_detail[buyer_counter][BUYER_DISTANCE];
                        distance_limit_close = bbuyer_detail[buyer_counter][BUYER_DISTANCE];
                        //printf("+-%d+\n",bbuyer_detail[buyer_counter][BUYER_DISTANCE]);
                    }                     
                }

                buyer_counter += 1;
            
            }

            //Record the furthest buyer location
            if(distance_limit_far > distance_upper_limit && sell_flag == 1){
                distance_upper_limit = distance_limit_far;
            }            
            
            //Record the closest buyer location
            if(distance_limit_close < bdistance_lower_limit && sell_flag == 1){
                bdistance_lower_limit = distance_limit_close;
                printf("*-%d*\n",bdistance_lower_limit);
                closest_bitem_quantity = dummy_cargo -> quantity;
            }

            //Calculate the total profit of this route for route picking
            if(dummy_highest_buyer_profit > 0){
                btotal_profit += dummy_highest_buyer_profit;
            }
            
            //Recondition the avaliable inventory space
            avaliable_weight -=  dummy_cargo -> quantity * dummy_cargo -> commodity -> weight;
            avaliable_volume -=  dummy_cargo -> quantity * dummy_cargo -> commodity -> volume; 

            dummy_cargo = dummy_cargo -> next;

        }
          
        //---------------------------------------------------------//

        //Initialize counters
        seller_counter = 0;
        buyer_counter = 0;
        station_counter = 0;
        bhighest_profit = 0;
        bhighest_profit_index = 0;

        //Searching for one additional possible route in this range
        while(bseller_detail[seller_counter][COMMODITY_INDEX] != -1 && bseller_detail[seller_counter][SELLER_DISTANCE] < distance_upper_limit){
            while(bbuyer_detail[buyer_counter][COMMODITY_INDEX] != -1 && bbuyer_detail[seller_counter][BUYER_DISTANCE] < distance_upper_limit){
                
                dummy_quantity = 0;
                dummy_distance_far = 0;
                dummy_distance = 0;
                dummy_last_station_index = 0;
                station_counter = 0;
                dummy_travel_cost = 0; 
                dummy_profit = 0;
                dummy_cash = b -> cash;

                //Matching seller and buyer
                if(bseller_detail[seller_counter][COMMODITY_INDEX] == bbuyer_detail[buyer_counter][COMMODITY_INDEX] 
                    && bseller_detail[seller_counter][SELLER_DISTANCE] < bbuyer_detail[buyer_counter][BUYER_DISTANCE] 
                    && bseller_detail[seller_counter][SELLER_DISTANCE] != 0){

                    //How many of this items I can fit in according to the weight
                    dummy_quantity = avaliable_weight / bseller_detail[seller_counter][COMMODITY_WEIGHT];

                    //How many of this items I can fit in according to the volume
                    if((avaliable_volume / bseller_detail[seller_counter][COMMODITY_VOLUME]) < dummy_quantity){
                        dummy_quantity = avaliable_volume / bseller_detail[seller_counter][COMMODITY_VOLUME];
                    }
                     
                    //If the seller has less than I can fit in              
                    if(bseller_detail[seller_counter][COMMODITY_QUANTITY] < dummy_quantity){
                        dummy_quantity = bseller_detail[seller_counter][COMMODITY_QUANTITY];
                    }

                    //If the buyer require less than I can fit in
                    if(bbuyer_detail[buyer_counter][COMMODITY_QUANTITY] < dummy_quantity){
                        dummy_quantity = bbuyer_detail[buyer_counter][COMMODITY_QUANTITY];
                    }

                    //How far do I have to travel to finish this transaction, and how much I would have to pay for the travel                   
                    dummy_distance_far = bbuyer_detail[buyer_counter][BUYER_DISTANCE];                 

                    //Calculate the cost of travel if refuel at every single fuel station on the way.
                    while(bstation_detail[station_counter][FUEL_QUANTITY] != -1 && bstation_detail[station_counter][STATION_DISTANCE] < dummy_distance_far){

                        dummy_travel_cost += (bstation_detail[station_counter][STATION_DISTANCE] - dummy_distance) * bstation_detail[station_counter][FUEL_PRICE];
                        dummy_last_station_index = station_counter;
                        
                        //This is here so it calculates the right amount of distance after charged.
                        dummy_distance = bstation_detail[station_counter][STATION_DISTANCE];
                        station_counter += 1;

                    }
                    

                    //I'll also have to be able to fall back to the closest fuel station after I finish the transaction
                    dummy_travel_cost += (dummy_distance_far - bstation_detail[dummy_last_station_index][STATION_DISTANCE]) * bstation_detail[dummy_last_station_index][FUEL_PRICE];         
                    
                    //Check whether I can afford all I can fit after deduce the travel cost from my cash
                    if((dummy_cash - dummy_travel_cost) / bseller_detail[seller_counter][COMMODITY_PRICE] < dummy_quantity){
                        dummy_quantity = (dummy_cash - dummy_travel_cost) / bseller_detail[seller_counter][COMMODITY_PRICE];
                    }                 

                    //Sweet Sweet profit :D
                    dummy_profit = dummy_quantity * (bbuyer_detail[buyer_counter][COMMODITY_PRICE] - bseller_detail[seller_counter][COMMODITY_PRICE]) - dummy_travel_cost; 
                    
                    //Record the highest profit and its respective quantity
                    if(dummy_profit > bseller_detail[seller_counter][SELLER_PROFIT] && dummy_profit > 0){
                        bseller_detail[seller_counter][SELLER_PROFIT] = dummy_profit;
                        bseller_detail[seller_counter][SELLER_AVA_QUANTITY] = dummy_quantity;
                        bseller_detail[seller_counter][RESPECTIVE_BUYER_DISTANCE] = bbuyer_detail[buyer_counter][BUYER_DISTANCE];
                    }                     
                }

                buyer_counter += 1;
            
            }

            //Calculate the total profit of this route for route picking
            if(bseller_detail[seller_counter][SELLER_PROFIT] > 0 && bseller_detail[seller_counter][SELLER_DISTANCE] != 0){
                btotal_profit += bseller_detail[seller_counter][SELLER_PROFIT];
            }
            
            //Keep the highest profit and its route for later use
            if(bseller_detail[seller_counter][SELLER_PROFIT] > fhighest_profit && bseller_detail[seller_counter][SELLER_DISTANCE] != 0){
                bhighest_profit = bseller_detail[seller_counter][SELLER_PROFIT];
                bhighest_profit_index = seller_counter;
                b_quantity = bseller_detail[seller_counter][SELLER_AVA_QUANTITY];
            }

            seller_counter += 1;

        }  

    }


printf("fprofit %d bprotft %d\n",ftotal_profit,btotal_profit);
    //--------------------------DECISION MAKING--------------------------//

    int final_command[3] = {-1};
    int direction_flag = 0;

    //=============================No Cargo=============================//

    //If I don't have cargo, which means I have followed protocol one
    if(b -> cargo == NULL){

        //If there are more fortune to be found in the backwards direction
        if(ftotal_profit < btotal_profit && btotal_profit > 0 && bseller_detail[bhighest_profit_index][RESPECTIVE_BUYER_DISTANCE] < b -> turns_left){
            
            direction_flag = -1;

            //Final command -> buy
            final_command[COMMAND_TYPE] = BUY;

            //Final command -> quantity
            final_command[COMMAND_QUANTITY] = b_quantity;

            //Final command -> move
            //If I can't get to it in this turn
            if(bseller_detail[bhighest_profit_index][SELLER_DISTANCE] >= b -> maximum_move){
                final_command[COMMAND_MOVE] = -1 * b -> maximum_move;
            }
            //If I can
            else{
                final_command[COMMAND_MOVE] = -1 * bseller_detail[bhighest_profit_index][SELLER_DISTANCE];
            }

        }

        //If there are more if I go forwards
        else if(ftotal_profit >= btotal_profit && ftotal_profit > 0 && fseller_detail[fhighest_profit_index][RESPECTIVE_BUYER_DISTANCE] < b -> turns_left){
            
            direction_flag = 1;

            //Final command -> buy
            final_command[COMMAND_TYPE] = BUY;

            //Final command -> quantity
            final_command[COMMAND_QUANTITY] = f_quantity;

            //Final command -> move
            //If I can't get to it in this turn
            if(fseller_detail[fhighest_profit_index][SELLER_DISTANCE] >= b -> maximum_move){
                final_command[COMMAND_MOVE] = b -> maximum_move;
            }
            //If I can
            else{
                final_command[COMMAND_MOVE] = fseller_detail[fhighest_profit_index][SELLER_DISTANCE];
            }

        }

        //If I encounter an analysis error
        else{

            //Final command -> error
            final_command[COMMAND_TYPE] = ERROR;

            //Final command -> move
            final_command[COMMAND_MOVE] = 1;

        }
    }

    //=============================With Cargo=============================//
    else{
        
        //If there are more fortune to be found in the backwards direction
        if(ftotal_profit < btotal_profit && btotal_profit > 0){

            direction_flag = -1;

            //If the additional seller is closer to me than all my target buyer
            if(bdistance_lower_limit >= bseller_detail[bhighest_profit_index][SELLER_DISTANCE] && bseller_detail[bhighest_profit_index][RESPECTIVE_BUYER_DISTANCE] < b -> turns_left && (bhighest_profit_index != 0 || bseller_detail[bhighest_profit_index][SELLER_DISTANCE] != 0)){

                //Final command -> buy
                final_command[COMMAND_TYPE] = BUY;

                //Final command -> quantity 
                final_command[COMMAND_QUANTITY] = b_quantity;

                //Final command -> move
                //If I can't get to it in this turn
                if(bseller_detail[bhighest_profit_index][SELLER_DISTANCE] >= b -> maximum_move){
                    final_command[COMMAND_MOVE] = -1 * b -> maximum_move;
                }
                //If I can
                else{
                    final_command[COMMAND_MOVE] = -1 * bseller_detail[bhighest_profit_index][SELLER_DISTANCE];
                }
            }

            //If the additional seller is further to me than all my target buyer
            else{

                //Final command -> sell
                final_command[COMMAND_TYPE] = SELL;

                //Final command -> quantity 
                final_command[COMMAND_QUANTITY] = closest_bitem_quantity;

                //Final command -> move
                //If I can't get to it in this turn
                if(bdistance_lower_limit >= b -> maximum_move){
                    final_command[COMMAND_MOVE] = -1 * b -> maximum_move;
                }
                //If I can
                else{
                    final_command[COMMAND_MOVE] = -1 * bdistance_lower_limit;
                }

            }         

        }
        //If there are more if I go forwards
        else if(ftotal_profit >= btotal_profit && ftotal_profit > 0){

            direction_flag = 1;

            //If the additional seller is closer to me than all my target buyer
            if(fdistance_lower_limit >= fseller_detail[fhighest_profit_index][SELLER_DISTANCE] && fseller_detail[fhighest_profit_index][RESPECTIVE_BUYER_DISTANCE] < b -> turns_left && (fhighest_profit_index != 0 || fseller_detail[fhighest_profit_index][SELLER_DISTANCE] != 0)){

                //Final command -> buy
                final_command[COMMAND_TYPE] = BUY;

                //Final command -> quantity
                final_command[COMMAND_QUANTITY] = f_quantity;

                //Final command -> move
                //If I can't get to it in this turn
                if(fseller_detail[fhighest_profit_index][SELLER_DISTANCE] >= b -> maximum_move){
                    final_command[COMMAND_MOVE] = b -> maximum_move;
                }
                //If I can
                else{
                    final_command[COMMAND_MOVE] = fseller_detail[fhighest_profit_index][SELLER_DISTANCE];
                }
            }

            //If the additional seller is further to me than all my target buyer
            else{

                //Final command -> sell
                final_command[COMMAND_TYPE] = SELL;

                //Final command -> quantity
                final_command[COMMAND_QUANTITY] = closest_fitem_quantity ;

                //Final command -> move
                //If I can't get to it in this turn
                if(fdistance_lower_limit >= b -> maximum_move){
                    final_command[COMMAND_MOVE] = b -> maximum_move;
                }
                //If I can
                else{
                    final_command[COMMAND_MOVE] = fdistance_lower_limit;
                }

            }   

        }
        //If I encounter an analysis error
        else{

            //Final command -> error
            final_command[COMMAND_TYPE] = 0;

            //Final command -> move
            final_command[COMMAND_MOVE] = 1;

        }
    }

    //=============================Check My Fuel=============================//

    //If only half of my fuel is left
    if(direction_flag > 0){
        if(b -> fuel <= fstation_detail[0][STATION_DISTANCE] && fstation_detail[0][STATION_DISTANCE] > b -> turns_left){
            //If my target is further than the closest fuel station
            if(final_command[COMMAND_MOVE] > fstation_detail[0][STATION_DISTANCE]){

                //Final command -> fuel
                final_command[COMMAND_TYPE] = REFUEL;

                //Final command -> quantity
                final_command[COMMAND_QUANTITY] = b -> fuel_tank_capacity - b -> fuel;

                //Final command -> move
                //If I can't get to it in this turn
                if(fstation_detail[0][STATION_DISTANCE] >= b -> maximum_move){
                    final_command[COMMAND_MOVE] = b -> maximum_move;
                }
                //If I can
                else{
                    final_command[COMMAND_MOVE] = fstation_detail[0][STATION_DISTANCE];
                }

            }

        }

    }
    else if(direction_flag < 0){
        if(b -> fuel <= bstation_detail[0][STATION_DISTANCE] && bstation_detail[0][STATION_DISTANCE] > b -> turns_left){
            //If my target is further than the closest fuel station
            if(-1 * final_command[COMMAND_MOVE] > bstation_detail[0][STATION_DISTANCE]){

                //Final command -> fuel
                final_command[COMMAND_TYPE] = REFUEL;

                //Final command -> quantity
                final_command[COMMAND_QUANTITY] = b -> fuel_tank_capacity - b -> fuel;

                //Final command -> move
                //If I can't get to it in this turn
                if(bstation_detail[0][STATION_DISTANCE] >= b -> maximum_move){
                    final_command[COMMAND_MOVE] = -1 * b -> maximum_move;
                }
                //If I can
                else{
                    final_command[COMMAND_MOVE] = -1 * bstation_detail[0][STATION_DISTANCE];
                }
            }
        }

    }

    //Emergency refuel
    if(b -> fuel <= abs(nearest_fuel(b))){
        //Final command -> fuel
        final_command[COMMAND_TYPE] = REFUEL;

        //Final command -> quantity
        final_command[COMMAND_QUANTITY] = b -> fuel_tank_capacity - b -> fuel;

        if(nearest_fuel(b) <= 0){
            if(abs(nearest_fuel(b)) >= b -> maximum_move){
                final_command[COMMAND_MOVE] = -1 * b -> maximum_move;
            }
            //If I can
            else{
                final_command[COMMAND_MOVE] = nearest_fuel(b);
            }
        }
        if(nearest_fuel(b) >= 0){
            if(abs(nearest_fuel(b)) >= b -> maximum_move){
                final_command[COMMAND_MOVE] = b -> maximum_move;
            }
            //If I can
            else{
                final_command[COMMAND_MOVE] = nearest_fuel(b);
            }
        }

    }


printf("Here is the final command: \n");
printf("%d %d %d\n",final_command[0],final_command[1],final_command[2]);
printf("fb%d bb%d fs%d bs%d\n",fdistance_lower_limit,bdistance_lower_limit,fseller_detail[fhighest_profit_index][SELLER_DISTANCE],bseller_detail[bhighest_profit_index][SELLER_DISTANCE] ); 

    //--------------------------ACTION--------------------------//
int test_counter = 0;
if(final_command[0] == 0){

   printf("here are the forwards seller: \n");
   while(fseller_detail[test_counter][0] != -1){
    printf("%s %d %d %d %d %d\n",fcommodity_list[fseller_detail[test_counter][0]],fseller_detail[test_counter][1],fseller_detail[test_counter][2],fseller_detail[test_counter][3],fseller_detail[test_counter][4],fseller_detail[test_counter][5]);
   test_counter += 1;
   }  
   printf("\n");

   test_counter = 0;
   printf("Here are the bseller: \n");
   while(bseller_detail[test_counter][0] != -1){
    printf("%s %d %d %d %d %d\n",bcommodity_list[bseller_detail[test_counter][0]],bseller_detail[test_counter][1],bseller_detail[test_counter][2],bseller_detail[test_counter][3],bseller_detail[test_counter][4],bseller_detail[test_counter][5]);
   test_counter += 1;
   }
   printf("\n");

   test_counter = 0;
   printf("Here are the fbuyer: \n");
   while(fbuyer_detail[test_counter][0] != -1){
    printf("%s %d %d %d\n",fcommodity_list[fbuyer_detail[test_counter][0]],fbuyer_detail[test_counter][1],fbuyer_detail[test_counter][2],fbuyer_detail[test_counter][3]);
   test_counter += 1;
   }
   printf("\n");

   test_counter = 0;
   printf("Here are the bbuyer: \n");
   while(bbuyer_detail[test_counter][0] != -1){
    printf("%s %d %d %d\n",bcommodity_list[bbuyer_detail[test_counter][0]],bbuyer_detail[test_counter][1],bbuyer_detail[test_counter][2],bbuyer_detail[test_counter][3]);
   test_counter += 1;
   }
   printf("\n");

   test_counter = 0;
   printf("Here are the fstation: \n");
   while(fstation_detail[test_counter][0] != -1){
    printf("%d %d %d\n",fstation_detail[test_counter][0],fstation_detail[test_counter][1],fstation_detail[test_counter][2]);
   test_counter += 1;
   }
   printf("\n");

   test_counter = 0;
   printf("Here are the bstation: \n");
   while(bstation_detail[test_counter][0] != -1){
    printf("%d %d %d\n",bstation_detail[test_counter][0],bstation_detail[test_counter][1],bstation_detail[test_counter][2]);
   test_counter += 1;
   } 
}

    //If I'm currently at a seller and my command is to buy
    if(b -> location -> type == 1 && final_command[COMMAND_MOVE] == 0 && final_command[COMMAND_TYPE] == BUY){
        *action = 1;
        *n = final_command[COMMAND_QUANTITY];
    }
    //If I'm currently at a buyer and my command is to sell
    else if(b -> location -> type == 2 && final_command[COMMAND_MOVE] == 0 && final_command[COMMAND_TYPE] == SELL){
        *action = 2;
        *n = final_command[COMMAND_QUANTITY];
    }
    //If I'm currently at a fuel station and my command is to refuel
    else if(b -> location -> type == 3 && final_command[COMMAND_MOVE] == 0 && final_command[COMMAND_TYPE] == REFUEL){
        *action = 1;
        *n = final_command[COMMAND_QUANTITY];
    }
    //If I'm not at any of my target locations
    else{
        *action = 0;
        *n = final_command[COMMAND_MOVE];
    }

}

int nearest_fuel(struct bot *b){

    int distance_1 = 0; //The distance to the nearest gas station forwards
    int distance_2 = 0; //The distance to the nearest gas station backwards

    struct location *temp_loc; //This is the dummy struct, I use this so the bot doesn't actually move

    if(b -> location -> type == 3){ //If already on station, return 0;
        return 0;
    }
    else{

        temp_loc = b -> location;
        while(temp_loc -> type != 3){
            temp_loc = temp_loc -> next;
            distance_1 += 1;
        }

        temp_loc = b -> location;
        while(temp_loc -> type != 3){
            temp_loc = temp_loc -> previous;
            distance_2 -= 1;
        }
        
        //Compare the two distances, choose the smaller one
        if(-(distance_2) >= distance_1){
            return distance_1;
        }
        else{
            return distance_2;
        }
    }

}