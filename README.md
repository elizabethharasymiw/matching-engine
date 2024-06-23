# Matching  Engine
Command line mock market program to match BUY and SELL offers

## How to Use
- Note: This project does not have any shipped releases yet
- Note: No support on Windows
- Install CMake 3.29.3 or newer
- Run Build Script: ``` ./cmake.sh ```
- Start Program: ``` ./manualtesting ```
- Buy Order Format: ``` BUY (GFD or IOC) (price) (amount) (order ID) ```
- Buy Order Example: ``` BUY GFD 1000 5 order1 ```
- Sell Order Format: ``` SELL (GFD or IOC) (price) (amount) (order ID) ```
- Sell Order Example: ``` SELL IOC 1000 3 order2 ```
- End Program and print remaining unfilled orders: ``` PRINT ```

## Developer Build Instructions
- Note: The test suite needs Catch2 v3 installed to build and run the tests
- Note: The build system needs CMake version 3.29.3 or newer installed to build the project
- Note: No support on Windows
- Build Script: ``` ./cmake.sh ```
- Manual Test Script: ``` ./manualtesting.sh ```
- Run Test Suite Script: ``` ./tests.sh ```

# Acronyms
- GFD == Good for Day
- IOC == Insert Or Cancel
