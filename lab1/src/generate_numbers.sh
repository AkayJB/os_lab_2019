#!/bin/bash

# Генерация 150 случайных чисел
od -An -N600 -i /dev/random | head -n 150 > numbers.txt
echo "Файл numbers.txt создан."