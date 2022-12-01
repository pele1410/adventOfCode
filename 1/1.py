#!/usr/bin/env python

"""
Advent of Code
Day 1

This list represents the Calories of the food carried by five Elves:

The first Elf is carrying food with 1000, 2000, and 3000 Calories, a total of 6000 Calories.
The second Elf is carrying one food item with 4000 Calories.
The third Elf is carrying food with 5000 and 6000 Calories, a total of 11000 Calories.
The fourth Elf is carrying food with 7000, 8000, and 9000 Calories, a total of 24000 Calories.
The fifth Elf is carrying one food item with 10000 Calories.
In case the Elves get hungry and need extra snacks, they need to know which Elf to ask: they'd like to know how many Calories are being carried by the Elf carrying the most Calories. In the example above, this is 24000 (carried by the fourth Elf).

Find the Elf carrying the most Calories. How many total Calories is that Elf carrying?
"""

import os.path
from collections import defaultdict


def read_calories():
    CALORIE_FILE = "calories.txt"

    if not os.path.isfile(CALORIE_FILE):
        print('File does not exist:', CALORIE_FILE)
        return {}
    else:
        with open(CALORIE_FILE) as f:
            lines = f.read().splitlines()

    return lines


def def_value():
    return 0


def split_calories_by_elf(calories_):
    d = defaultdict(def_value)
    elf_count = 0
    for i in calories_:
        if len(i) > 0:
            d[elf_count] = d[elf_count]+int(i)
        else:
            elf_count = elf_count + 1

    return d


def find_largest(dict_):
    largest_elf = 0
    largest_calories = 0
    for i, (elf, calories) in enumerate(dict_.items()):
        if calories > largest_calories:
            largest_elf = elf
            largest_calories = calories

    return {largest_elf: largest_calories}


def main():
    c = read_calories()
    d = split_calories_by_elf(c)
    largest_elf = find_largest(d)
    print(largest_elf)


if __name__ == "__main__":
    main()
