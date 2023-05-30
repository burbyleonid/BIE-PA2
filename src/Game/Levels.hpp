#include <vector>
#include "Field.hpp"


void generateRandomLevel(std::vector<std::vector<Field>>& levelMap, int width, int height) {
    levelMap.resize(height, std::vector<Field>(width, Field(0)));

    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Generate random content for each cell
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int fieldType = std::rand() % 6 - 2;  // Random type between -2 and 3
            levelMap[row][col] = Field(fieldType);
        }
    }
}

void printLevel(const std::vector<std::vector<Field>>& levelMap) {
    for (const auto& row : levelMap) {
        for (const auto& field : row) {
            std::cout << "#";  // Print the field symbol based on the field type
        }
        std::cout << "\n";
    }
}


// std::vector<std::vector<Field>> level1Map;
// int width = 10;
// int height = 5;
// generateRandomLevel(level1Map, width, height);

// std::cout << "Level 1:\n";
// printLevel(level1Map);

// // Generate and print a random level with size 8x6
// std::vector<std::vector<Field>> level2Map;
// width = 8;
// height = 6;
// generateRandomLevel(level2Map, width, height);

// std::cout << "\nLevel 2:\n";
// printLevel(level2Map);

// // Generate and print a random level with size 12x8
// std::vector<std::vector<Field>> level3Map;
// width = 12;
// height = 8;
// generateRandomLevel(level3Map, width, height);

// std::cout << "\nLevel 3:\n";
// printLevel(level3Map);

