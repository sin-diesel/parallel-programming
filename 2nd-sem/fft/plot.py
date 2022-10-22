
import matplotlib.pyplot as plt
import sys


def main():
    x = []
    y = []

    if len(sys.argv) != 2:
        print('ERROR: file not passed as argument')
        return 1

    for line in open(sys.argv[1], 'r'):
        lines = [i for i in line.split()]
        x.append(lines[0])
        y.append(lines[1])
        
    plt.title("FFT execution speed")
    plt.xlabel('Input size')
    plt.ylabel('CPU time')
    plt.xlim([0, 33554432])
    plt.ylim([0, 10e-3])
    plt.grid()  
    plt.scatter(x, y, marker = 'o', c = 'g')    
    plt.show()

if __name__ == '__main__':
    sys.exit(main())