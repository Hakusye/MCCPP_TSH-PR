import random
random.seed(4649)

if __name__ == "__main__":
    path =  '1000_max3000_random.dat'
    weights =  [ round(random.random() * 2999.9 ,1) + 0.1 for i in range(1000) ]
    weights.sort()
    with open(path,mode='w') as f:
        for weight in weights:
            file_input = str(weight) + '\n'
            f.write(file_input)


