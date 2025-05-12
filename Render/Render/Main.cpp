#include <SDL.h>
#include <SDL2/SDL_gfxPrimitives.h>  // Для рисования кругов
#include <iostream>
#include <cstdlib>  // Для генерации случайных чисел

// Размеры окна
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Функция инициализации SDL
bool InitSDL(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Ошибка инициализации SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Графический рендеринг",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Ошибка создания окна: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Ошибка создания рендера: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

// Класс для прямоугольника
class Rectangle {
public:
    int x, y, width, height;
    SDL_Color color;

    Rectangle(int x, int y, int width, int height, SDL_Color color)
        : x(x), y(y), width(width), height(height), color(color) {
    }

    void draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_Rect rect = { x, y, width, height };
        SDL_RenderFillRect(renderer, &rect);
    }
};

// Класс для круга
class Circle {
public:
    int x, y, radius;
    SDL_Color color;

    Circle(int x, int y, int radius, SDL_Color color)
        : x(x), y(y), radius(radius), color(color) {
    }

    void draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        filledCircleColor(renderer, x, y, radius, SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), color.r, color.g, color.b));
    }
};

// Класс для линии
class Line {
public:
    int x1, y1, x2, y2;
    SDL_Color color;

    Line(int x1, int y1, int x2, int y2, SDL_Color color)
        : x1(x1), y1(y1), x2(x2), y2(y2), color(color) {
    }

    void draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
};

// Обработка ввода с клавиатуры для перемещения прямоугольника
void handleInput(SDL_Event& event, Rectangle& rect) {
    const int moveSpeed = 10;

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_LEFT:
            rect.x -= moveSpeed;
            break;
        case SDLK_RIGHT:
            rect.x += moveSpeed;
            break;
        case SDLK_UP:
            rect.y -= moveSpeed;
            break;
        case SDLK_DOWN:
            rect.y += moveSpeed;
            break;
        }
    }
}

// Обработка кликов мыши для изменения цвета прямоугольника
void handleMouse(SDL_Event& event, Rectangle& rect) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (x > rect.x && x < rect.x + rect.width && y > rect.y && y < rect.y + rect.height) {
            rect.color = { rand() % 256, rand() % 256, rand() % 256, 255 };  // Случайный цвет
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // Инициализация SDL
    if (!InitSDL(window, renderer)) {
        return 1;
    }

    // Создание графических примитивов
    SDL_Color red = { 255, 0, 0, 255 };
    SDL_Color green = { 0, 255, 0, 255 };
    SDL_Color blue = { 0, 0, 255, 255 };

    Rectangle rect(100, 100, 200, 150, red);
    Circle circle(400, 300, 50, green);
    Line line(50, 50, 300, 300, blue);

    bool running = true;
    SDL_Event event;

    // Главный цикл программы
    while (running) {
        // Обработка событий
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            handleInput(event, rect);  // Обработка клавиш
            handleMouse(event, rect);  // Обработка мыши
        }

        // Рендеринг
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Черный фон
        SDL_RenderClear(renderer);

        // Рисуем примитивы
        rect.draw(renderer);
        circle.draw(renderer);
        line.draw(renderer);

        SDL_RenderPresent(renderer);  // Отображение сцены

        SDL_Delay(16);  // Задержка для ограничения FPS (~60 FPS)
    }

    // Очистка ресурсов
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
