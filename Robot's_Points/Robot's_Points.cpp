#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>

// Структура для хранения точки в пространстве
struct Point {
    double x, y, z;
};

// Чтение точек из файла
std::vector<Point> readPoints(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Point p;
        if (iss >> p.x >> p.y >> p.z) {
            points.push_back(p);
        }
    }
    return points;
}

// Чтение плоскостей из файла
std::array<std::array<Point, 3>, 2> readPlanes(const std::string& filename) {
    std::array<std::array<Point, 3>, 2> planes;
    std::ifstream file(filename);
    std::string line;
    int index = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        if (index < 6) {
            int planeIndex = index / 3;
            int pointIndex = index % 3;
            iss >> planes[planeIndex][pointIndex].x
                >> planes[planeIndex][pointIndex].y
                >> planes[planeIndex][pointIndex].z;
        }
        index++;
    }
    return planes;
}

// Вычисление нормали плоскости через три точки
Point computePlaneNormal(const Point& p1, const Point& p2, const Point& p3) {
    Point normal;
    normal.x = (p2.y - p1.y) * (p3.z - p1.z) - (p2.z - p1.z) * (p3.y - p1.y);
    normal.y = (p2.z - p1.z) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.z - p1.z);
    normal.z = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    return normal;
}

// Проверка, находится ли точка перед или за плоскостью
bool isPointInFrontOfPlane(const Point& point, const Point& planePoint, const Point& normal) {
    double d = -(normal.x * planePoint.x + normal.y * planePoint.y + normal.z * planePoint.z);
    double value = normal.x * point.x + normal.y * point.y + normal.z * point.z + d;
    return value > 0; // Точка перед плоскостью, если значение положительно
}

// Фильтрация точек по двум плоскостям
void filterPoints(const std::vector<Point>& points, const std::array<std::array<Point, 3>, 2>& planes,
    std::vector<Point>& goodPoints, std::vector<Point>& badPoints) {
    // Вычисляем нормали плоскостей
    Point normal1 = computePlaneNormal(planes[0][0], planes[0][1], planes[0][2]);
    Point normal2 = computePlaneNormal(planes[1][0], planes[1][1], planes[1][2]);

    for (const auto& point : points) {
        bool inFrontOfPlane1 = isPointInFrontOfPlane(point, planes[0][0], normal1);
        bool inFrontOfPlane2 = isPointInFrontOfPlane(point, planes[1][0], normal2);

        if (inFrontOfPlane1 && inFrontOfPlane2) {
            goodPoints.push_back(point);
        }
        else {
            badPoints.push_back(point);
        }
    }
}

// Запись точек в файл
void writePoints(const std::string& filename, const std::vector<Point>& points) {
    std::ofstream file(filename);
    for (const auto& point : points) {
        file << point.x << " " << point.y << " " << point.z << "\n";
    }
}

int main() {
    setlocale(LC_CTYPE, "rus");
    // Имена файлов передаются через командную строку
    std::string pointsFile;
    std::string planesFile;

    // Ввод с поддержкой русского языка
    std::cout << "Введите имя файла с координатами точек: ";
    std::cin >> pointsFile;
    std::cout << "Введите имя файла с координатами плоскостей: ";
    std::cin >> planesFile;

    // Чтение точек и плоскостей
    auto points = readPoints(pointsFile);
    auto planes = readPlanes(planesFile);

    // Векторы для "хороших" и "ложных" точек
    std::vector<Point> goodPoints, badPoints;

    // Фильтрация точек
    filterPoints(points, planes, goodPoints, badPoints);

    // Запись результатов
    std::string goodFile = pointsFile + "_good.txt";
    std::string badFile = pointsFile + "_wrong.txt";
    writePoints(goodFile, goodPoints);
    writePoints(badFile, badPoints);

    // Вывод результата на русском языке
    std::cout << "Обработка завершена.\nХорошие точки сохранены в " << goodFile
        << "\nЛожные точки сохранены в " << badFile << "\n";

    return 0;
}
