# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(CampusFoodDeliverySystem VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Set source files
set(SOURCES
    src/main.cpp
    src/Menu.cpp
    src/Order.cpp
    src/DeliverySystem.cpp
    src/FoodItem.cpp
    src/OrderedItem.cpp
)

# Add executable target
add_executable(food_delivery_system ${SOURCES})
