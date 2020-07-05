import random
random.seed(4649)

if __name__ == "__main__":
    path =  '125_max300_random.dat'
    weights =  [ round(random.random() * 299.9 ,1) + 0.1 for i in range(125) ]
    weights.sort()
    with open(path,mode='w') as f:
        for weight in weights:
            file_input = str(weight) + '\n'
            f.write(file_input)


