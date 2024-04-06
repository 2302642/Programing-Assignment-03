/*!****************************************************************************************************************************************************************************************************************************************************************
\file       ocean.cpp
\par        Author:          Najih
\par        Email:           n.hajasheikallaudin
\par        Course:          RSE1202    
\par        Section:         RSE
\par        Tutorial:        Assignment 3
\par        File created on: 27/01/2024
\brief      battle ship game 

\par        File information:
            This file is contains 6 function definitions to called by main in q-driver.
            Functions:
            - Ocean* CreateOcean(int num_boats, int x_size, int y_size);                                                // to create the ocean grid using x and y size and allocate memory for the ocean grid and for the array of boats to be place in ocean grid
            - BoatPlacement PlaceBoat(Ocean& ocean, Boat const& boat);                                                  // to place boats into the ocean grid and updating the boats stats in the ocean struct object.
            - ShotResult TakeShot(Ocean& ocean, Point const& coordinate);                                               // to identify stats of the shots fired in the ocean grid and up date object in structure shotstats in structure ocean
            - ShotStats GetShotStats(Ocean const& ocean);                                                               // to return all the stats(hits, sunk, misses and duplicates) stord in the struct ShotStats in struct ocean 
            - void DestroyOcean(Ocean *theOcean);                                                                       // to delete/free allocated memory for the ocean struct objects
    (given) - void DumpOcean(const Ocean &ocean, int width = 4, bool extraline = false, bool showboats = true);         // to print the physical representation of the grid 
******************************************************************************************************************************************************************************************************************************************************************/
#include "ocean.h"
#include <iostream> // std::cout
#include <iomanip>  // std::setw

namespace HLP2 {
  namespace WarBoats {
//################################################################################################################################################################################################################################################################
    int const BOAT_LENGTH {4};   //!< Length of a boat
    int const HIT_OFFSET  {100}; //!< Add this to the boat ID
//================================================================================================================================================================================================================================================================
//|                                                                         OCEAN CREATION SECTION                                                                                                                                                               |
//================================================================================================================================================================================================================================================================
    Ocean* CreateOcean(int num_boats, int hor_size, int vert_size){                                                     // to create the ocean grid using x and y size and allocate memory for the ocean grid and for the array of boats to be place in ocean grid
      
      Ocean* sea = new Ocean;                                                                                           // allocate memory for  ocean struct object
      sea->grid = new int[hor_size * vert_size];                                                                        // allocated memory for the ocean grid
      for (int i = 0; i < hor_size * vert_size; ++i) { sea->grid[i] = dtOK; }                                           // initialize the grid to dtOK (empty ocean)
      
      sea -> x_size    = hor_size;                                                                                      // set up ocean grid and avaliable boats initializing x_size and storing into x in struct point in struct boat which is in struct ocean
      sea -> y_size    = vert_size;                                                                                     // set up ocean grid and avaliable boats initializing y_size and storing into y in struct point in struct boat which is in struct ocean
      sea -> num_boats = num_boats;                                                                                     // set up ocean grid and avaliable boats initializing num_boats in struct ocean
      sea -> boats   = new Boat[num_boats];                                                                             // allocating memory for the array of boats
      
      sea -> stats.hits       = 0;                                                                                      // initializing the statistics of hits       in struct ShotStats in struct ocean
      sea -> stats.sunk       = 0;                                                                                      // initializing the statistics of sunk       in struct ShotStats in struct ocean
      sea -> stats.misses     = 0;                                                                                      // initializing the statistics of misses     in struct ShotStats in struct ocean
      sea -> stats.duplicates = 0;                                                                                      // initializing the statistics of duplicates in struct ShotStats in struct ocean
      
      for (int i = 0; i < num_boats; ++i) {                                                                             // initializing the stats for the boats array
          sea -> boats[i].hits = 0;
          sea -> boats[i].ID   = 0;
        }
      return sea;                                                                                                       // return the allocated ocean stats objects
    }
//================================================================================================================================================================================================================================================================
//|                                                                         BOAT PLACEMENT SECTION                                                                                                                                                               |
//================================================================================================================================================================================================================================================================
/*
    BoatPlacement PlaceBoat(Ocean& ocean, Boat const& boat) {
      

      if(boat.position.x < 0 || boat.position.y < 0 || boat.position.x >= ocean.x_size || boat.position.y >= ocean.y_size) { return bpREJECTED;}        //check if the boat is outside the ocean


      if (boat.ID > ocean.num_boats){ return BoatPlacement::bpREJECTED; }

      for (int i = 0; i < BOAT_LENGTH; i++) {                                                                                                           // Check if there is already a boat in the location
        int grid_location;
        if (boat.orientation == oHORIZONTAL) {  grid_location = ocean.x_size * (boat.position.y)     + (boat.position.x + i) ; } 
        else {                                  grid_location = ocean.x_size * (boat.position.y + i) +  boat.position.x      ; }
        if (ocean.grid[grid_location] != dtOK) {  return bpREJECTED;  }                                                                                 // reject if the location is already occupied by a boat
      }

      for (int i = 0; i < BOAT_LENGTH; i++) {                                                                                                           // Place the boat in the ocean        
        int grid_location;                                                                                                                              // identify the orientation and location for the boat to be placed
        if (boat.orientation == oHORIZONTAL) {  grid_location = ocean.x_size * (boat.position.y)     + (boat.position.x + i) ; } 
        else {                                  grid_location = ocean.x_size * (boat.position.y + i) +  boat.position.x      ; }
        
       //if (ocean.grid[grid_location] != dtOK || ocean.grid[grid_location] >= HIT_OFFSET) {  return bpREJECTED;  }
        
        ocean.grid[grid_location] = boat.ID;                                                                                                            // Place the boat in the ocean with boat unique ID
      }
      return bpACCEPTED;
    }
*/  
    BoatPlacement PlaceBoat(Ocean& ocean, Boat const& boat) {                                                           // to place boats into the ocean grid and updating the boats stats in the ocean struct object.
      
      if(boat.position.x >= ocean.x_size || boat.position.y >= ocean.y_size) { return bpREJECTED;}                      // check if the boat is outside the ocean

      int start = (ocean.x_size * boat.position.y) + boat.position.x;                                                   // identifying the start location of the boat to be placed
      int EOR = boat.position.y * ocean.x_size + (ocean.x_size - 1);                                                    // EOR (end of row)    (horizontal check) identifying the last posible horizontal space that the boat can cover in the ocean grid
      int EOC = ocean.x_size * ocean.y_size - 1;                                                                        // EOC (end of column) (vertical check) identifying the maxium space of ocean grid (one dimentional array)

      if ((boat.orientation == oHORIZONTAL && (start + (BOAT_LENGTH - 1) > EOR)) || (boat.orientation == oVERTICAL && (start + (ocean.x_size * (BOAT_LENGTH - 1)) > EOC))) {
        return BoatPlacement::bpREJECTED;                                                                               // checking for out of bound of ocean grid
      }

      for (int i = 0; i < BOAT_LENGTH; i++) {
        //if (boat.orientation == Orientation::oHORIZONTAL) { placement = start + i; }
        //else { placement = start + (i * ocean.x_size); }
        int placement = (boat.orientation == oHORIZONTAL) ? start + i : start + (i * ocean.x_size);                     // identifying the orientation of the boat  
        if (ocean.grid[placement] != dtOK) { return bpREJECTED; }                                                       // reject placement of boat in that grid coordinates if that coordinates is occupied
      }      
      
      if (boat.ID > ocean.num_boats) { return bpREJECTED; }
      else{                                                                                                             // identifying and assigning the position and orientation of the boat to the current boat id 
        ocean.boats[boat.ID - 1].ID = boat.ID;                                                                          // identifying and assigning the current boat for this iteration
        ocean.boats[boat.ID - 1].position = boat.position;                                                              // identifying and assigning the current boat  to update boat position status                                                               
        ocean.boats[boat.ID - 1].orientation = boat.orientation;                                                        // identifying and assigning the current boat  to update boat orientation status
      }
      
      for (int i = 0; i < BOAT_LENGTH; i++) {
      //if (boat.orientation == oHORIZONTAL) { placement = start + i; }
      //else { placement = start + (i * ocean.x_size); }
        int placement = (boat.orientation == oHORIZONTAL) ? start + i : start + (i * ocean.x_size);                     // identifying and assigning the position and orientation of the boat current boat id 
        ocean.grid[placement] = boat.ID;                                                                                // placeing the boat in the ocean grid in the specified location
      }
      return BoatPlacement::bpACCEPTED;
    }   
//================================================================================================================================================================================================================================================================
//|                                                                          SHOT RESULT SECTION                                                                                                                                                                 |
//================================================================================================================================================================================================================================================================
/*
    ShotResult TakeShot(Ocean& ocean, const Point& coordinate) {
        
          if (coordinate.x >= ocean.x_size || coordinate.y >= ocean.y_size) { return srILLEGAL; }                                             // check if the shot is outside the ocean grid

          int gridValue = ocean.grid[coordinate.y * ocean.x_size + coordinate.x];

          
          if (gridValue == dtOK) {                                                                                                            // Check for open water position (Miss)
              ocean.stats.misses++;
              ocean.grid[coordinate.y * ocean.x_size + coordinate.x] = dtBLOWNUP;
              return srMISS;
          }
          
          if (gridValue == dtBLOWNUP || (gridValue >= 1 + HIT_OFFSET && gridValue <= 99 + HIT_OFFSET)) {                                      // Check for Duplicate or re-hit
              ocean.stats.duplicates++;
              return srDUPLICATE;
          }
        
          ocean.stats.hits++;                                                                                                                 // Hit on an un-hit boat position
          //int boatID = ocean.boats[gridValue - 1].ID;
          ocean.boats[gridValue - 1].hits++;
          ocean.grid[coordinate.y * ocean.x_size + coordinate.x] += HIT_OFFSET;

          if (ocean.boats[gridValue - 1].hits == BOAT_LENGTH) { ocean.stats.sunk++; return srSUNK; }                                          // Check if the boat has sunk

          return srHIT;
        }
*/
    ShotResult TakeShot(Ocean& ocean, Point const& coordinate) {                                                        // to identify stats of the shots fired in the ocean grid and up date object in structure shotstats in structure ocean
  
    if (coordinate.x >= ocean.x_size || coordinate.y >= ocean.y_size) { return srILLEGAL; }                             // Check if the shot coordinates are within the range of ocean grid else return srILLEGAL

    int location = (coordinate.y * ocean.x_size) + coordinate.x;                                                        // Calculate the location of where the boat is to be in the ocean grid
    for (int num_boats = 0; num_boats < ocean.num_boats; num_boats++) {                                                 // loop through each boats to be in the ocean
      Point position = ocean.boats[num_boats].position;                                                                 // identify the location where the boat is to be in
      int boat_pos = (position.y * ocean.x_size) + position.x;                                                          // identify the current position of the boat                                
         
      for (int j = 0; j < BOAT_LENGTH; j++) {                                                                           // loop through each section of the boat       
        if (location == boat_pos) {
          if (ocean.grid[boat_pos] < HIT_OFFSET) {                                                                      // checking if the shot coordinates matchs boat position and that the boat was not hit at this position previously
            ocean.grid[boat_pos] += HIT_OFFSET;                                                                         // setting that the boat is hit at that coordinates
            ocean.boats[num_boats].hits++;                                                                              // increment boat hits
            ocean.stats.hits++;                                                                                         // updating the total number of hits occured in the ocean grid by updating the stats
        
            if (ocean.boats[num_boats].hits == BOAT_LENGTH) {                                                           // checking if all the sections of the boat is hit, and increment sunk in the ocean grid by updating the stats and return srSUNK
                ocean.stats.sunk++;
                return srSUNK;
            }else { return srHIT; }                                                                                     // else return srHIT
          }
          ocean.stats.duplicates++;                                                                                     // if the boat has already been hit at this position, increment duplicates count and return srDUPLICATE
          return srDUPLICATE;                                                                                
        } 
        else {                                                                                                          // move to the next part of the boat based on its orientation
          if (ocean.boats[num_boats].orientation == oHORIZONTAL) { boat_pos++; }                                        // increment horizontally
          else { boat_pos += ocean.x_size; }                                                                            // increment vertically  
        }
      }
    }

    if (ocean.grid[location] == dtBLOWNUP) {                                                                            // Check if the shot has already hit a blown-up boat
      ocean.stats.duplicates++;                                                                                         // Increment duplicates count
      return srDUPLICATE;                                                                                               // Return srDUPLICATE
    }
   
    ocean.grid[location] = dtBLOWNUP;                                                                                   // if shot did not cause hit, sunk or duplicate and initilze shot for miss and increment miss count and return srMISS 
    ocean.stats.misses++;
    return ShotResult::srMISS;
  }
//================================================================================================================================================================================================================================================================
//|                                                                          SHOTS STATS SECTION                                                                                                                                                                 |
//================================================================================================================================================================================================================================================================
    ShotStats GetShotStats(Ocean const& ocean) { return ocean.stats; }                                                  // to return all the stats(hits, sunk, misses and duplicates) stord in the struct ShotStats in struct ocean 
//================================================================================================================================================================================================================================================================
//|                                                                     OCEAN CLEARING/DELETING SECTION                                                                                                                                                          |
//================================================================================================================================================================================================================================================================
    void DestroyOcean(Ocean *theOcean){                                                                                 // to delete/free allocated memory for the ocean struct objects     
      delete[] theOcean->grid;                                                                                          // deallocate memory allocated for the ocean grid
      delete[] theOcean->boats;                                                                                         // deallocate memory allocated for the array of boats
      delete theOcean;                                                                                                  // deallocate memory allocated for the ocean structure
    }
//================================================================================================================================================================================================================================================================
//|                                                                   OCEAN LAYOUT PRINTING SECTION (given)                                                                                                                                                      |
//================================================================================================================================================================================================================================================================
    void DumpOcean(const HLP2::WarBoats::Ocean &ocean, int field_width, bool extraline, bool showboats) {               // to print the physical representation of the grid 
      for (int y = 0; y < ocean.y_size; y++) {                                                                          // for each row
        for (int x = 0; x < ocean.x_size; x++) {                                                                        // for each column

          int value = ocean.grid[y * ocean.x_size + x];                                                                 // get value at x/y position            
          value = ( (value > 0) && (value < HIT_OFFSET) && (showboats == false) ) ? 0 : value;                          // is it a boat that we need to keep hidden?
          std::cout << std::setw(field_width) << value;
        }
        std::cout << "\n";
        if (extraline) { std::cout << "\n"; }
      }
    }
//################################################################################################################################################################################################################################################################
  } // namespace WarBoats
} // namespace HLP2
