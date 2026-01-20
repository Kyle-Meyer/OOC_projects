#include <stdio.h>
#include <iostream>
#include <array>
void classifyTriangle(std::array<int,3> aSides)
{
   int32_t tEqualSides =  0;
   for(size_t i = 1; i <= 3; i++)
      if(aSides[i-1] == aSides[i])
         tEqualSides++;
   std::cout << "Triangle of [" 
             << aSides[0] << " , "
             << aSides[1] << " , "
             << aSides[2] << "] is: ";
   switch(tEqualSides)
   {
      case 2:
         std::cout << " equilateral";
         break; 
      case 1: 
         std::cout << " isoceles";
         break;
      case 0:
         std::cout << " scalene";
         break;
      default:
         std::cout << " ERROR!";
         break;
   }
   std::cout << std::endl;
}

int main (int argc, char *argv[]) 
{
   classifyTriangle(std::array<int, 3>{1,1,1});
   classifyTriangle(std::array<int, 3>{1,1,2});
   classifyTriangle(std::array<int, 3>{1,2,3});
   return 0;
}
