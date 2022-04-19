from random import randint, sample

MAX_PARTICIPANTS = 20
MAX_CONTACTS = 50
MAX_TIME = 60

def gen():
  n = randint(2, MAX_PARTICIPANTS) # participants
  m = randint(1, MAX_CONTACTS) # contacts

  contacts = []
  
  for i in range(m):
    a, b = sample(range(1, n), 2)

    st, en = sample(range(1, MAX_TIME), 2)    
    if en < st:
      st, en = en, st
      
    rate, owlt = 1, 1
    
    contacts.append([a, b, st, en, rate, owlt])
    contacts.append([b, a, st, en, rate, owlt])
    
    
  print('\n'.join(map(lambda c: ' '.join(map(str, c)), contacts)))
  
gen()