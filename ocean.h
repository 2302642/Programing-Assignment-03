/*!****************************************************************************************************************************************************************************************************************************************************************
\file       ocean.h
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
////////////////////////////////////////////////////////////////////////////////
#ifndef OCEAN_H
#define OCEAN_H
////////////////////////////////////////////////////////////////////////////////

namespace HLP2 {
  namespace WarBoats {
    struct Ocean; //!< Forward declaration for the Ocean 

    enum Orientation   { oHORIZONTAL, oVERTICAL };
    enum ShotResult    { srHIT, srMISS, srDUPLICATE, srSUNK, srILLEGAL };
    enum DamageType    { dtOK = 0, dtBLOWNUP = -1 };
    enum BoatPlacement { bpACCEPTED, bpREJECTED };

      //! A coordinate in the Ocean
    struct Point {
      int x; //!< x-coordinate (column)
      int y; //!< y-coordinate (row)
    };

      //! A boat in the Ocean
    struct Boat {
      int hits;                 //!< Hits taken so far
      int ID;                   //!< Unique ID 
      Orientation orientation;  //!< Horizontal/Vertical
      Point position;           //!< x-y coordinate (left-top)
    };

      //! Statistics of the "game"
    struct ShotStats {
      int hits;       //!< The number of boat hits
      int misses;     //!< The number of boat misses
      int duplicates; //!< The number of duplicate (misses/hits)
      int sunk;       //!< The number of boats sunk
    };

      // The interface
    Ocean* CreateOcean(int num_boats, int x_size, int y_size);
    void DestroyOcean(Ocean *theOcean);
    ShotResult TakeShot(Ocean& ocean, Point const& coordinate);
    BoatPlacement PlaceBoat(Ocean& ocean, Boat const& boat);
    ShotStats GetShotStats(Ocean const& ocean);
    
    void DumpOcean(const Ocean &ocean, int width = 4, bool extraline = false, bool showboats = true);             // Provided

  } // namespace WarBoats

} // namespace HLP2

namespace HLP2 {
  namespace WarBoats {
      //! The attributes of the ocean
    struct Ocean {
      int *grid;        //!< The 2D ocean 
      Boat *boats;      //!< The dynamic array of boats
      int num_boats;    //!< Number of boats in the ocean
      int x_size;       //!< Ocean size along x-axis
      int y_size;       //!< Ocean size along y-axis
      ShotStats stats;  //!< Status of the attack
    };
    
  } // namespace WarBoats
} // namespace HLP2

#endif // OCEAN_H
////////////////////////////////////////////////////////////////////////////////
