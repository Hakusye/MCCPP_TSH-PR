import random
random.seed(4649)

if __name__ == "__main__":
    path =  '40_max1000_random.dat'
    weights =  [ random.randint(1,1000)  for i in range(40) ]
    weights.sort()
    with open(path,mode='w') as f:
        for weight in weights:
            file_input = str(weight) + '\n'
            f.write(file_input)


