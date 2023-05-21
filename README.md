# Vector
Eksperimentiniai Vector klasės tyrimai

Testavimas:

Test Case 1: Sukurti tuscia vektoriu ir patikrinti elementu skaiciu bei vektoriaus dydi

kostruktorius, size(), capacity()

Test Case 2: Iterpiame naujus elementus ir patikriname vektoriaus dydi

push_back()

Test Case 3: Pasiekti vektoriaus elementus naudojant operator []

operator[]

Test Case 4: Istrinti elementus is vektoriaus ir patikrinti jo dydi

erase(), begin()

Test Case 5: Patikriname reserve() ir shrink_to_fit()

reserve(), shrink_to_fit()


Spartos analize:
|Konteinerio tipas  | 10000      | 100000     | 1000000    | 10000000   | 100000000  |
|------------------ |------------|------------|------------|------------|------------|
|std::vector        | 0.0004412 s| 0.0026377 s| 0.0137801 s| 0.141774 s | 1.21614 s  |
|Custom Vector      | 0.0003114 s| 0.0011343 s| 0.0088765 s| 0.0873297 s| 1.1342 s   |

Perskirstymu skaicius su 100000000:
std::vector: 27
Custom Vector: 26


