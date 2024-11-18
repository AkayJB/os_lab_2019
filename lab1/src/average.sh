#!/bin/bash

# Подсчитываем количество аргументов
count=$#
echo "Количество аргументов: $count"

# Проверяем, переданы ли аргументы
if [ $count -eq 0 ]; then
    echo "Нет аргументов для подсчета."
    exit 1
fi

# Вычисляем сумму и среднее арифметическое
sum=0
for arg in "$@"; do
    sum=$((sum + arg))
done
average=$(echo "scale=2; $sum / $count" | bc)
echo "Среднее арифметическое: $average"