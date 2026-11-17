#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstring>

// Функция для обработки ошибок GLFW
static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char**) {
    // Инициализация GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        return 1;
    }

    // Создание окна GLFW
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
    // Фиксированный размер окна
    GLFWwindow* window = glfwCreateWindow(500, 400, "Калькулятор", nullptr, nullptr);
    if (window == nullptr) {
        return 1;
    }
    
    // Отключаем возможность изменения размера окна
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Включаем VSync

    // Инициализация Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    //Загрузка шрифта
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/arial.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    
    // Настройка стиля
    ImGui::StyleColorsDark();
    
    // Инициализация бэкендов
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Переменные калькулятора
    float numberA = 0.0f;
    float numberB = 0.0f;
    float result = 0.0f;
    bool hasError = false;
    char errorMessage[256] = "";

    // Главный цикл
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Начало нового кадра ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Создание окна калькулятора
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        
        ImGui::Begin("Calculator", nullptr, 
            ImGuiWindowFlags_NoTitleBar | 
            ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse);

        // Заголовок
        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text("Калькулятор");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::SetWindowFontScale(1.0f);

        // Поля ввода чисел
        ImGui::SetWindowFontScale(1.2f);
        ImGui::InputFloat("Число А", &numberA);
        ImGui::InputFloat("Число Б", &numberB);
        ImGui::Spacing();
        ImGui::Spacing();

        // Кнопки операций
        float buttonWidth = (ImGui::GetContentRegionAvail().x - 30.0f) / 4.0f;
        
        if (ImGui::Button("+", ImVec2(buttonWidth, 40))) {
            result = numberA + numberB;
            hasError = false;
        }
        ImGui::SameLine();
        
        if (ImGui::Button("-", ImVec2(buttonWidth, 40))) {
            result = numberA - numberB;
            hasError = false;
        }
        ImGui::SameLine();
        
        if (ImGui::Button("*", ImVec2(buttonWidth, 40))) {
            result = numberA * numberB;
            hasError = false;
        }
        ImGui::SameLine();
        
        if (ImGui::Button("/", ImVec2(buttonWidth, 40))) {
            if (numberB == 0.0f) {
                hasError = true;
                strcpy(errorMessage, "Ошибка: деление на ноль!");
            } else {
                result = numberA / numberB;
                hasError = false;
            }
        }

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Spacing();

        // Вывод результата
        ImGui::Text("Результат:");
        ImGui::Spacing();
        
        ImGui::SetWindowFontScale(2.0f);
        if (hasError) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", errorMessage);
        } else {
            ImGui::Text("%.4f", result);
        }
        ImGui::SetWindowFontScale(1.0f);

        ImGui::End();

        // Рендеринг
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Очистка
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}