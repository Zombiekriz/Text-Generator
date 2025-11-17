# Text-Generator
Algorithm for predicting the next word using a Markov Chain

USAGE:
The program takes in a number of text files (12 are provided) and tries to predict the next word(s) that comes in a sentence.
The user can provide input but they don't have to.
Open pred.exe on Windows and pred on Linux

SETTINGS:
They can be accessed by inputting "set&" after the program has finished preparing itself. 
1. Randomness - Two numbers (0 to 100) that control how "creative" the output is. The first one matters more however both change a lot.
0% means always choosing the best option (which can often result in an infinite loop)
100% means always choosing the worst possible option (encountered the least amount of times)
For me the ideal values are 40 and 20 but feel free to change them

2. Output type - Could be "S" (sentences) or "W" (words). 
This just changes whether the program outputs a single word or sentence with each newline
When "W" is selected, after each word comes either a "|" or a "/" sign.
The first one indicates that the program found a word that matches something it encountered before.
The second one means it had no idea and just picked randomly.

ADDING FILES:
It is possible to add your own text files.
You have to paste a .txt file into the \text folder and rename it to file(number_of_biggest_file+1)(title).txt
Example: file13breaking_bad.txt

EXTRA:
There is a file 0 which contains a lot of pasted books in it. 
By default it's not included but you can easily change that by renaming it to file13extra_bigfile.txt 
In my experience it doesn't really improve anything and makes the preparing process a lot longer

Author: Zombiekriz
