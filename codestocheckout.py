#code 1
import re 
n=input() 
n=re.sub("[aeiouy]*([^aeiouy])[aeiouy]*", r".\1", n.lower()) 
print(n)
#code 2
s = input() 
c = s.lower() 
for i in c:
  if i != 'a' and i != 'e' and i != 'i' and i != 'o' and i != 'u':
    print(("." + i), end='')

