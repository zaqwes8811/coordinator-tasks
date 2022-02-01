Allows distribute running C/C++ unittests based on Google Test Framework. Предполагается что пара OS/CPU заданы компилятором.

###Мотивация
- Хочется легко добавлять тесты, часто их запускат и не чистить фильтры. Это приводит к росту числа тестов и времени их работы.
Хотелось бы чтобы время работы тестов было уменьшено
- Возможность тестирование кода на разных платформах, но в передалах типа операционной системы и процессора (например, семейство Windows/x86)
- Писать тесты на одной платформе, а на целевой легко запускать. Например, на одной операционной системе стоит кросскомпилятор, 
а целевая система имеет и другую ОС и возможно другой процессор (например - разработка идет
на Windows, а целевая платформа Linux/RaspberyPi/ARM7) и хотелось бы удобно запускать тесты на целевой платформе, т.е. отправить файл на нее, запустить
и собрать результаты
- Работать с одним кластером коммандой
- Легко развертывать систему систему тестирования

###Ideas
- Если тесты независимы, то можно их запускть параллельно. Это накладывает ограничения. Так для тестирования С кода, нужно сбрасывать состояния модуля, для каждного
кейса. Под это определение попадают юнит-тесты, что касается других, пока не совсем ясно
- Run test cases in different processes on cluster and reduce results on main node where installed IDE or test-framework
- One mashine run shards equal cores of own processor
- In one test case programm may be paralleled use OpenMP (can find dummy errors and up probabilbty detection multithreading errors)
- У Google Test, по-моему, довольно захламленный stdout - хотелось очистить 

###How it work
В основу положена технология MapReduce
- Средства запуска мапперов (шарды) всегда существуют и осуществляют опрос своего мастера, который большую часть времени отключен
- Мастер включается время от вермени и раздает работу
- Мапперы выполняет работу 
- Мастер собирает результаты и выводит разработчику

###Covered test levels (см. информацию в конце страницы)
- Small - Yes - возможно полезно использовать in-memory filesystem and db
- Medium - Some
- Large - Maybe

###Dependences
- `gtest/gmock` - it's base for testing
- `Python 2.6` - important - simplest way. In higher version up trouble import std module
- `octopy` - project content some version. Was modified only timeout requests to master-node
- `OpenMP` - может быть полезной - позволяет распараллелить программу в пределах одного теста

### Work platform
- now only Windows

###Know issues
- Для `MinGW` gtest не видит тесты в файлах, отличных от файла с функцией main
- Если шард упала в работе, компьютер с мастером придется перезагрузить - порт будет занят, и изменить его нельзя, т.к.
на него ориентируются рабочие ноды. Ошибки при работе с кластером не обрабатываются (см. man octopy)

###Know inconveniences
- На кластере работают толко юнит-тесты (не обращаются к файлам, не запускают скритпы - только голый exe - файл). Поддержка medium-тестов с зависимыми файлами планируется, но на данный момент этого нет. Для такого запуска нужно паковать все необходимое в архив, слать, распаковывать, что увеличивает задержку при малом числе тестов.
- Если малых тестов мало, то лучше не пользоваться - выполнятся в сумме будут дольше. 
Но если юнит-тестов много, или функциональные долгие, то выгода может проявится
- Need to know near count cores in system. Можно сконфигуритовать запуск программы оценки, а можно грубо задать. Но желательно, чтобы
заданное число работ было равно числу шардов (здесь она равно число ядер в кластере)
- Нужно задавать оценочное время работы теста. Проблема в том, что код может быть с ошибками, и нет различия, если тест просто долгий, 
в нем есть бесконечный цикл или deadlock. Map-функция запускает тест и ждет. Если слишком долго, то она останавливает исполнение, а что накопилось
с пометкой ошибки отправлят на мастер
- Если тесты связаны, лучше перенести их в один. В пределах теста, на данный момент, программа должна запускаться с нуля. Тесты должны быть
полностью независимы. Это важно, в противном случае, резальтаты могут менятся при разных запусках
- Передается только исполняемый файл, поэтому all machines in cluster must have equal operating system, and set libraries needed for run code
- For test C code need add "extern C" in headers and make reset state for clear state on jumps between tests
- For VSEE exist some troubles in configure for running OpenMP

###Some information about testing
- "Effective work with legacy code" - most bad case - no test ever
- Misko (blog, video, articles) - testable code - no tests, but code is potential testable
- Fauler (refactoring) - if test exist we can make code better
- Test Levels from Google [it](http://googletesting.blogspot.ru/2010/12/test-sizes.html) and [it](http://googletesting.blogspot.ru/2010/04/googles-innovation-factory-and-how.html)
- О тестировании многопоточных программ - "Java Concurency on Practice"


###Similar projects
- swarm_client - Chromium project https://code.google.com/p/chromium/
- swarming - https://code.google.com/p/swarming/






