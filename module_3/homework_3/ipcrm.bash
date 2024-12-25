#!/bin/bash

# Запуск программы и захват shmid
shmid=$(./player1 | grep -oP '(?<=shmid: )\d+')

# Проверяем, что shmid получен
if [[ -z "$shmid" ]]; then
    echo "Ошибка: не удалось получить shmid"
    exit 1
fi

echo "Получен shmid: $shmid"

# Удаляем сегмент разделяемой памяти
ipcrm -m "$shmid"

echo "Сегмент с shmid $shmid успешно удалён"
