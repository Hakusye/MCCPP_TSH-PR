import random
random.seed(4649)

if __name__ == "__main__":
    path =  'weight/1000_max10000_random.dat'
    weights =  [ random.randint(1,10000)  for i in range(1000) ]
    weights.sort()
    with open(path,mode='w') as f:
        for weight in weights:
            file_input = str(weight) + '\n'
            f.write(file_input)


