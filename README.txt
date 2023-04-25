Popa Stefania
336CB

    Am implementat paradigma Map-Reduce, cu ajutorul unei structuri create de mine ("thread_params"),
ce va retine toate informatiile necesare, ce trebuie transmise thread-urilor pentru executia task-urilor.
    Fiecare thread va avea asignat un id in momentul crearii acestuia. Printre informatiile retinute in structura
anterioara, se numara si un mutex si o bariera, cu ajutorul carora ne asiguram ca fiecare thread executa partea
lui de task. 
    Avem si un camp numit "perfectPowers", un dublu pointer la un vector, in care vom retine puterile 
perfecte corespunzatoare fiecarui exponent, pentru fiecare Mapper.

    Citesc mai intai numarul de fisiere de intrare, numele acestora si le stochez intr-o coada numita "fileNames".

    Voi porni exact M+R thread-uri: primele M (cu id de la 0 la noOfMappers-1) fiind de tip Mapper, urmatoarele
R (de la noOfMappers la noOfMappers + noOfReducers) de tip Reducer. 
    Pentru a afla tipul thread-ului vom verifica de fiecare data in ce interval se incadreaza id-ul thread-ului curent.
    
    Mapperii iau din coada fileNames cate un fisier, pana ce acesta se goleste.
    Folosim un mutex pentru momentul in care un Mapper scoate din coada un fisier, pentru a ne asigura ca nu se intampla
ca 2 thrread-uri sa prelucreze acelasi fisier.


    Citesc din fiecare fisier numarul de numere si apoi numerele propriu zise, verificand pentru fiecare daca este 
putere perfecta.
    Pentru a verifica daca un numar este putere perfecta, creez o functie "mapperFunc" in care voi verifica daca un 
numar n este putere perfecta pentru un exponent e (pentru toate valorile lui e de la 2 la noOfReducers + 1). 


    Aceasta verificare o vom face cu ajutorul cautarii binare. Cautam daca exista un numar m de tip int care ridicat la 
puterea e sa fie egal cu n. Cautam valoarea lui m in intervalul (left, right) unde valoarea initiala a lui left este 0, 
iar a lui right este radical de ordinul e din INT32_MAX. 
    In cazul in care  gasim un m , vom introduce numarul n la pozitia corespunzatoare exponentului curent si respectiv a
Mapper-ului curent.

    Folosim o bariera pentru a ne asigura ca toate Mapper-ele au terminat de procesat fisierele si de executat task-urile,
inainte de inceperea Reducer-ilor. 

    Un Reducer va lua puterile perfecte corespunzatoare unui exponent e, din fiecare Mapper de la pozitia exponentului curent
si le va introduce intr-o multime(aceasta stocand doar elementele distincte).
    La sfarsit doar verificam cate elemente are multimea
si scriem raspunsul in fisierul de output corespunzator exponentului curent.
    




