#include <stdio.h>
#include <iostream>
#include <array>

void classifyTriangle(std::array<int,3> aSides)
{
   int32_t tEqualSides = 0;
   
   // Fixed: Compare adjacent pairs correctly
   if(aSides[0] == aSides[1])
      tEqualSides++;
   if(aSides[1] == aSides[2])
      tEqualSides++;
   if(aSides[0] == aSides[2])
      tEqualSides++;
   
   std::cout << "Triangle of [" 
             << aSides[0] << " , "
             << aSides[1] << " , "
             << aSides[2] << "] is: ";
   
   switch(tEqualSides)
   {
      case 3:  // All three sides equal
         std::cout << "equilateral";
         break;
      case 1:  // Exactly two sides equal
         std::cout << "isoceles";
         break;
      case 0:  // No sides equal
         std::cout << "scalene";
         break;
      default:
         std::cout << "ERROR!";
         break;
   }
   std::cout << std::endl;
}

int main (int argc, char *argv[]) 
{
   // Test all scenarios:
   
   // Equilateral (all sides equal)
   classifyTriangle(std::array<int, 3>{1, 1, 1});
   classifyTriangle(std::array<int, 3>{5, 5, 5});
   
   // Isoceles (exactly two sides equal)
   classifyTriangle(std::array<int, 3>{1, 1, 2});
   classifyTriangle(std::array<int, 3>{2, 1, 1});
   classifyTriangle(std::array<int, 3>{1, 2, 1});
   classifyTriangle(std::array<int, 3>{3, 5, 3});
   
   // Scalene (all sides different)
   classifyTriangle(std::array<int, 3>{1, 2, 3});
   classifyTriangle(std::array<int, 3>{3, 4, 5});
   classifyTriangle(std::array<int, 3>{2, 3, 4});
   
   return 0;
}