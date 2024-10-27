#!/bin/bash

# Создаём файлы и символические ссылки
for i in $(seq 1 20); do
    # Создать директорию и файл с номером
    mkdir -p "$i"
    echo "$i" > "$i/$i.txt"

    # Создать символическую ссылку на файл
    ln -s "$i/$i.txt" "$i.txt"
done

# Удаляем файлы, не содержащие "4"
for i in $(seq 1 20); do
    if ! grep -q "4" "$i/$i.txt"; then
        rm -f "$i/$i.txt" "$i.txt"
    fi
done

# Изменяем содержимое файла 14/14.txt
echo "eltex" > 14/14.txt
