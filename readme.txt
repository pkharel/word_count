Name: 
Pradosh Kharel

How long did the lab take? 
10 - 15 Hours

Does your lab work as expected? 
Not as well as I'd hoped. My numbers are slightly off. 

Are there any issues?
I think my program has trouble with non standard characters that seem to be
present in the text files.

What assumptions/design choices did you make and why?
I used a struct to store a word and a count because it seemed natural. I restricted
the max length of a string to 100 chars and the max number of words to 100 words in 
the dictionary. This was to ensure no overflow occured since the size of the arrays
are mostly fixed. Also the token used to parse files only contains a limited number
of chars ( .,;?!':") so the presence of other characters would be detrimental and 
throw off the word count. 
