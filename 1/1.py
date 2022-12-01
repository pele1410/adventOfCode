#!/usr/bin/env python

"""
Advent of Code
Day 1

--- Part One ---
This list represents the Calories of the food carried by five Elves:

The first Elf is carrying food with 1000, 2000, and 3000 Calories, a total of 6000 Calories.
The second Elf is carrying one food item with 4000 Calories.
The third Elf is carrying food with 5000 and 6000 Calories, a total of 11000 Calories.
The fourth Elf is carrying food with 7000, 8000, and 9000 Calories, a total of 24000 Calories.
The fifth Elf is carrying one food item with 10000 Calories.
In case the Elves get hungry and need extra snacks, they need to know which Elf to ask: they'd like to know how many Calories are being carried by the Elf carrying the most Calories. In the example above, this is 24000 (carried by the fourth Elf).

Find the Elf carrying the most Calories. How many total Calories is that Elf carrying?

--- Part Two ---
By the time you calculate the answer to the Elves' question, they've already realized that the Elf carrying the most Calories of food might eventually run out of snacks.

To avoid this unacceptable situation, the Elves would instead like to know the total Calories carried by the top three Elves carrying the most Calories. That way, even if one of those Elves runs out of snacks, they still have two backups.

In the example above, the top three Elves are the fourth Elf (with 24000 Calories), then the third Elf (with 11000 Calories), then the fifth Elf (with 10000 Calories). The sum of the Calories carried by these three elves is 45000.

Find the top three Elves carrying the most Calories. How many Calories are those Elves carrying in total?

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

    for calories in calories_:
        if len(calories) > 0:
            d[elf_count] += int(calories)
        else:
            elf_count += 1

    return d


def sort_calories(dict_):
    sorted_calories = dict(sorted(dict_.items(), key=lambda x: x[1]))
    return sorted_calories


def find_largest(dict_, num_to_find_):
    tmp = list(dict_.items())
    largest_elves = tmp[-num_to_find_:]
    return dict(largest_elves)


def sum_largest(dict_):
    total_calories = 0
    for _, value in dict_.items():
        total_calories += value

    return total_calories


def main():
    c = read_calories()
    calories_by_elf = split_calories_by_elf(c)
    sorted_calories = sort_calories(calories_by_elf)
    largest_elves = find_largest(sorted_calories, 3)
    print(largest_elves)
    total_calories = sum_largest(largest_elves)
    print(total_calories)


if __name__ == "__main__":
    main()
