# Stonks Server

A Stock management server written in C. It includes  multithreading, networking and file management.

# Usage

Connect to the server (Stonks_Server.out <Port>) and run: <Order Type> <Company ID> <Number of Stocks> <Price>

Example: 1 6 3 5000
This means: <Sell> stocks from company <6>, <3> stocks at price <5000>

## Command ID's

1: Sell at price
2: Sell at percentage of price
3: Purchase at price
4: Purchase at percentage of price
5: Remove sale order
6: Remove purchase order