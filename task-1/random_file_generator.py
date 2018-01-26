
# coding: utf-8

# In[1]:

import string
import numpy as np
import random


# In[5]:

letters = list(string.ascii_lowercase)
letters += '\n'
numbers = range(0, 100, 1)
array = [letters, numbers]
mapp = [0, 1]
cnt = 0
is_n = False
is_num = False
with open("random.txt", "w") as f:
    for i in range(1000000):
        index = random.choice(mapp)
        if (index == 1):
            if (is_num == False):
                number = random.choice(numbers)
                cnt += number
                f.write(str(number))
                is_num = True
            else:
                pass
        else:
            is_num = False
            symbol = random.choice(letters)
            if (symbol == '\n'):
                if (is_n == False):
                    f.write(symbol)
                    is_n = True
                else:
                    pass
            else:
                is_n = False
                f.write(symbol)
print(cnt)
with open("random_result.txt", "w") as f:
	f.write(str(cnt))

# In[ ]:



