import sys
import numpy

def sgn(weights, data):
    print(data)

def populateVector(vector):
    sparse = ['None'] * 69
    i = 0
    # fill in sparse vector
    for element in vector:
        digit = element.split(':')
        if i < 69:
            while (digit[0] != str(i)):
                i += 1
        sparse[i] =  digit
    return sparse

def populateMatrix(vector):
    print(vector[0])

    mat = numpy.empty(shape = [0, 2])
    for i in range(69):
        if vector[i] == 'None':
            mat = numpy.append(mat, [[i, 0]], axis=0)
        else:
            mat = numpy.append(mat, [[i, 0.18257]], axis = 0)
    return mat

def main():
    file = open('Dataset/phishing.train', 'r')
    line = file.readline()
    vector = line.split(" ")

    weights = []

    trainingY = vector[0]
    vector.pop(0)

    sparse = populateVector(vector)
    matrix = populateMatrix(sparse)
    print(matrix)

    #print(trainingY)
    pass

if __name__ == '__main__':
    sys.exit(main())