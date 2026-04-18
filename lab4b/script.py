import random
import string

def random_key(length=5):
    return ''.join(random.choices(string.ascii_lowercase, k=length))

keys = set()
target = 1_000_000

while len(keys) < target:
    keys.add(random_key())

with open("keys.txt", "w") as f:
    for key in keys:
        f.write(key + "\n")

