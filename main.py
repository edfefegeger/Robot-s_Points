import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def read_points(filename):
    points = []
    with open(filename, 'r') as f:
        for line in f:
            points.append(list(map(float, line.split())))
    return points

def plot_points(good_points, bad_points):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    good_xs, good_ys, good_zs = zip(*good_points)
    bad_xs, bad_ys, bad_zs = zip(*bad_points)
    ax.scatter(good_xs, good_ys, good_zs, c='g', label='Верные точки')
    ax.scatter(bad_xs, bad_ys, bad_zs, c='r', label='Не верные точки')
    ax.set_title('Good and Bad Points')
    ax.legend()
    plt.show()

# Чтение точек
good_points = read_points('points.txt_good.txt')
bad_points = read_points('points.txt_wrong.txt')

# Визуализация
plot_points(good_points, bad_points)
