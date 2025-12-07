# Дополнительное задание

## Запуск №1
> Вероятность выпадения NULL - 5%
> Запускаемая программа - factor

Поскольку я не особо обрабатывал ошибки, то мне хватило 7 итераций до первого `Segmentation fault (core dumped)`

## Запуск №2
> Вероятность выпадения NULL - 5%
> Запускаемая программа - factor

Теперь я доработал программу, и спустя 1657 запусков не произошло ни одной ошибки

## Запуск №3
> Вероятность выпадения NULL - 5%
> Запускаемая программа - java

Из-за того, что java требует много памяти даже на самые простые программы, то из-за большого количества аллокаций с первой итерации приложение сразу падало
### Как выглядела ошибка в консоли
> # There is insufficient memory for the Java Runtime Environment to continue.
> # Native memory allocation (malloc) failed to allocate 32 bytes. Error detail: AllocateHeap
> # An error report file with more information is saved as:
> # /home/maxim/CLionProjects/benchs/hs_err_pid29961.log



## Запуск №4
> Вероятность выпадения NULL - 5 / 1000000
> Запускаемая программа - java

Теперь программа выполнила 38 итераций

