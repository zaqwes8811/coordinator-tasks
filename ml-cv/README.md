!!!!!!!!! Google ML codestyle and links !!!!!!!!!!!!!

1. "Лекция 2. Методы решения задачи классификации и регрессии" Youtube
2. "Лекция 4. Преобразование признаков" 
3. http://cs231n.github.io/neural-networks-1/

//=========================== global 

https://habrahabr.ru/company/yandex/blog/315138/
"чем больше априорных знаний о данных (алг. обучения) тем лучше работает"

//=====================================================================================
Отбор признаков:
- Интуиция
- классификатор с регулизацией L1 [1]
- pca

Robst:
- L2 - матожидание [1]
- L1 - медиана [1]!!!

Данные
- следить чтобы тренировочные данные не отличались от тестовых - 0.007 -> 0.035 [2]

Overfitting:
!! -"It models the data as two blobs and interprets the few red points inside the green cluster as outliers (noise). 
In practice, this could lead to better generalization on the test set." understood why ANN better SVM

//======================================================================

-1. Интуиция!!!
0. Получаем данны
1. Изучаем их!!! - важно, дисперсии, средние... outliners!!!!
Что подаешь как обучающие выборки влияет на результат
2. Масштабируем min/max, ...
3. Test

Как можно меньше фич с как можно большей информацией


Typical errors:
1. Wrong metrix
2.


Improve:
http://machinelearningmastery.com/improve-machine-learning-results-with-boosting-bagging-and-blending-ensemble-methods-in-weka/
bagging, boosting, blending

Troubles:
imbalance - http://stats.stackexchange.com/questions/21592/optimising-for-precision-recall-curves-under-class-imbalance?rq=1
!!! http://machinelearningmastery.com/tactics-to-combat-imbalanced-classes-in-your-machine-learning-dataset/

Tricks:
add to train only samples with misses

//=====================================================================

From Intel DAAL:
- Data managment
- Analysis - differenct preproc and ...
https://software.intel.com/ru-ru/node/564608
- Training and prediction

From skikit-learn:
- ...

//=====================================================================
2d/3d + optic
Descr + Inf. stat -> ml -> math optimize -> soft+hard


//======================= CV ========================
gpu
http://stackoverflow.com/questions/31599368/strange-error-with-cudareprojectimageto3d-and-opencv-3-0
