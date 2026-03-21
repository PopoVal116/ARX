#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

#define MAX_RECORDS 100

// Структура для текстуры
struct Texture {
    unsigned int id;
    int width;
    int height;
    bool loaded;
};

// Структура записи с полем текстуры
struct Record {
    char author[100];
    char song[100];
    char album[100];
    char genre[50];
    char duration[20];
    char image_path[200];
    Texture texture;
};

// Состояния сортировки
enum SortState {
    SORT_NONE,
    SORT_ASC,
    SORT_DESC
};

bool loadTextureFromFile(const char* filename, Texture& texture) {
    int channels;
    unsigned char* image_data = stbi_load(filename, &texture.width, &texture.height, &channels, 4);
    
    if (image_data == NULL) {
        cout << "Failed to load image: " << filename << endl;
        texture.loaded = false;
        return false;
    }
    
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);
    
    texture.loaded = true;
    cout << "Loaded texture: " << filename << " (" << texture.width << "x" << texture.height << ")" << endl;
    return true;
}

void preloadTextures(Record records[], int record_count) {
    for (int i = 0; i < record_count; i++) {
        loadTextureFromFile(records[i].image_path, records[i].texture);
    }
}

// Заглушка сортировки
void sortRecords(Record records[], int record_count, SortState state) {
    cout << "SORT: ";
    if (state == SORT_NONE) cout << "NONE";
    else if (state == SORT_ASC) cout << "ASC";
    else cout << "DESC";
    cout << endl;
    
    // Здесь будет реальная сортировка
}

// Заглушка поиска
void searchRecords(Record records[], int record_count, const char* query) {
    cout << "SEARCH for: " << query << endl;
}

// ===== ФУНКЦИЯ ДЛЯ ОТОБРАЖЕНИЯ ТАБЛИЦЫ =====
void DisplayRecordsTable(Record records[], int record_count, int& selected_record, bool& show_photo_window) {
    if (ImGui::BeginTable("records_table", 5,
        ImGuiTableFlags_Borders |           // границы
        ImGuiTableFlags_RowBg |              // чередование фона строк
        ImGuiTableFlags_Resizable |          // изменение размера колонок
        ImGuiTableFlags_ScrollY, 
        ImVec2(0, 400))) {
        
        ImGui::TableSetupColumn("Song", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Author", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Album", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Genre", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Duration", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        

        
        // Данные
        for (int i = 0; i < record_count; i++) {
            ImGui::TableNextRow();
            
            // Песня – кнопка во всю ячейку
            ImGui::TableSetColumnIndex(0);
            ImGui::PushID(i);
            float cell_width = ImGui::GetContentRegionAvail().x;
            if (ImGui::Button(records[i].song, ImVec2(cell_width, 0))) {
                selected_record = i;
                show_photo_window = true;
            }
            ImGui::PopID();
            
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", records[i].author);
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", records[i].album);
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s", records[i].genre);
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%s", records[i].duration);
        }
        
        ImGui::EndTable();
    }
}

void run_gui(Record records[], int record_count) {  
    cout << "Starting GUI" << endl;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Music list",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1400, 900, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); 
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight();
    
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    preloadTextures(records, record_count);
    
    bool running = true;
    int selected_record = -1;
    bool show_photo_window = false;
    
    // Поиск
    char search_buffer[256] = "";
    char search_result[256] = "";  
    bool show_search_result = false;
    
    // Сортировка
    SortState current_sort = SORT_NONE;
    
    // Переменные для добавления
    bool show_add_window = false;
    char new_author[100] = "";
    char new_song[100] = "";
    char new_album[100] = "";
    char new_genre[50] = "";
    char new_duration[20] = "";
    
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Главное окно
        ImGui::Begin("Music Records", nullptr, ImGuiWindowFlags_NoCollapse);
        
        if (record_count == 0) {
            ImGui::Text("No records loaded!");
        } else {
            // ===== ЗАГОЛОВКИ С КНОПКОЙ СОРТИРОВКИ =====
            // Создаём простую таблицу для заголовка с кнопкой сортировки
            if (ImGui::BeginTable("header_table", 5, ImGuiTableFlags_SizingFixedFit, ImVec2(0, 0))) {
                ImGui::TableSetupColumn("Song", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Author", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Album", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Genre", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Duration", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                
                ImGui::TableNextRow();
                
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Song");
                
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("Author");
                float avail_width = ImGui::GetContentRegionAvail().x;
                ImGui::SameLine(avail_width - 30);
                
                // Кнопка сортировки
                const char* sort_btn;
                if (current_sort == SORT_NONE) sort_btn = "N##sort";
                else if (current_sort == SORT_ASC) sort_btn = "A##sort";
                else sort_btn = "D##sort";
                
                if (ImGui::Button(sort_btn)) {
                    if (current_sort == SORT_NONE)
                        current_sort = SORT_ASC;
                    else if (current_sort == SORT_ASC)
                        current_sort = SORT_DESC;
                    else
                        current_sort = SORT_NONE;
                    
                    sortRecords(records, record_count, current_sort);
                }
                
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("Album");
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("Genre");
                ImGui::TableSetColumnIndex(4);
                ImGui::Text("Duration");
                
                ImGui::EndTable();
            }
            
            // ===== ВЫЗЫВАЕМ ФУНКЦИЮ ДЛЯ ОТОБРАЖЕНИЯ ТАБЛИЦЫ =====
            DisplayRecordsTable(records, record_count, selected_record, show_photo_window);
            
            // ===== ПОИСК ПОД ТАБЛИЦЕЙ =====
            ImGui::Separator();
            ImGui::Text("Search:");
            ImGui::InputText("##search", search_buffer, sizeof(search_buffer));
            ImGui::SameLine();
            if (ImGui::Button("Find")) {
                searchRecords(records, record_count, search_buffer);
                strcpy(search_result, search_buffer);
                show_search_result = true;
            }
            
            if (show_search_result && strlen(search_result) > 0) {
                ImGui::TextColored(ImVec4(0,1,0,1), "Search result for '%s'", search_result);
                ImGui::Text("(Search functionality would filter records here)");
            }
            
            ImGui::Text("Total records: %d", record_count);

            // ===== КНОПКА ДЛЯ ДОБАВЛЕНИЯ =====
            ImGui::Separator();
            if (ImGui::Button("Add New Record", ImVec2(200, 30))) {
                show_add_window = true;
                new_author[0] = '\0';
                new_song[0] = '\0';
                new_album[0] = '\0';
                new_genre[0] = '\0';
                new_duration[0] = '\0';
            }
            
            // ===== ОКНО ДОБАВЛЕНИЯ =====
            if (show_add_window) {
                ImGui::Begin("Add New Record", &show_add_window, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
                
                ImGui::Text("Enter record details:");
                ImGui::Separator();
                
                ImGui::InputText("Author", new_author, sizeof(new_author));
                ImGui::InputText("Song", new_song, sizeof(new_song));
                ImGui::InputText("Album", new_album, sizeof(new_album));
                ImGui::InputText("Genre", new_genre, sizeof(new_genre));
                ImGui::InputText("Duration", new_duration, sizeof(new_duration));
                
                ImGui::Separator();
                
                if (ImGui::Button("Save", ImVec2(120, 0))) {
                    Record new_record;
                    
                    strcpy(new_record.author, (strlen(new_author) > 0) ? new_author : "");
                    strcpy(new_record.song, (strlen(new_song) > 0) ? new_song : "");
                    strcpy(new_record.album, (strlen(new_album) > 0) ? new_album : "");
                    strcpy(new_record.genre, (strlen(new_genre) > 0) ? new_genre : "");
                    strcpy(new_record.duration, (strlen(new_duration) > 0) ? new_duration : "");
                    
                    strcpy(new_record.image_path, "pictures/default.jpg");
                    new_record.texture.loaded = false;
                    
                    if (record_count < MAX_RECORDS) {
                        records[record_count] = new_record;
                        record_count++;
                        cout << "Added new record" << endl;
                    }
                    
                    show_add_window = false;
                }
                
                ImGui::SameLine();
                
                if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                    show_add_window = false;
                }
                
                ImGui::End();
            }
        }
        
        ImGui::End();
        
        // ===== ОКНО С ФОТО =====
        if (show_photo_window && selected_record >= 0 && selected_record < record_count) {
            ImGui::Begin("Photo", &show_photo_window, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
            
            ImGui::Text("Song: %s", records[selected_record].song);
            ImGui::Text("Artist: %s", records[selected_record].author);
            ImGui::Separator();
            
            if (records[selected_record].texture.loaded) {
                float max_size = 400.0f;
                float aspect = (float)records[selected_record].texture.width / records[selected_record].texture.height;
                float w = (aspect > 1.0f) ? max_size : max_size * aspect;
                float h = (aspect > 1.0f) ? max_size / aspect : max_size;
                ImGui::Image((void*)(intptr_t)records[selected_record].texture.id, ImVec2(w, h));
            } else {
                ImGui::TextColored(ImVec4(1,0,0,1), "Failed to load image!");
                ImGui::Text("Path: %s", records[selected_record].image_path);
            }
            
            ImGui::End();
        }
        
        ImGui::Render();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
    
    // Очистка текстур
    for (int i = 0; i < record_count; i++) {
        if (records[i].texture.loaded) {
            glDeleteTextures(1, &records[i].texture.id);
        }
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    cout << "GUI finished" << endl;
}


int main() {    
    Record records[MAX_RECORDS];
    int record_count = 10;
    
    // ... инициализация записей (без изменений) ...
    strcpy(records[0].author, "The Weeknd");
    strcpy(records[0].song, "Blinding Lights");
    strcpy(records[0].album, "After Hours");
    strcpy(records[0].genre, "Synthwave");
    strcpy(records[0].duration, "3:20");
    strcpy(records[0].image_path, "pictures/1.jpg");
    

    
    strcpy(records[1].author, "Dua Lipa");
    strcpy(records[1].song, "Don't Start Now");
    strcpy(records[1].album, "Future Nostalgia");
    strcpy(records[1].genre, "Disco-Pop");
    strcpy(records[1].duration, "3:03");
    strcpy(records[1].image_path, "pictures/1.jpg");
    
    strcpy(records[2].author, "Imagine Dragons");
    strcpy(records[2].song, "Believer");
    strcpy(records[2].album, "Evolve");
    strcpy(records[2].genre, "Alternative Rock");
    strcpy(records[2].duration, "3:24");
    strcpy(records[2].image_path, "pictures/1.jpg");
    
    strcpy(records[3].author, "Billie Eilish");
    strcpy(records[3].song, "bad guy");
    strcpy(records[3].album, "When We All Fall Asleep Where Do We Go?");
    strcpy(records[3].genre, "Electropop");
    strcpy(records[3].duration, "3:14");
    strcpy(records[3].image_path, "pictures/1.jpg");
    
    strcpy(records[4].author, "Eminem");
    strcpy(records[4].song, "Rap God");
    strcpy(records[4].album, "The Marshall Mathers LP 2");
    strcpy(records[4].genre, "Hip-Hop");
    strcpy(records[4].duration, "6:04");
    strcpy(records[4].image_path, "pictures/1.jpg");
    
    strcpy(records[5].author, "Ed Sheeran");
    strcpy(records[5].song, "Shape of You");
    strcpy(records[5].album, "÷ (Divide)");
    strcpy(records[5].genre, "Pop");
    strcpy(records[5].duration, "3:53");
    strcpy(records[5].image_path, "pictures/1.jpg");
    
    strcpy(records[6].author, "Queen");
    strcpy(records[6].song, "Bohemian Rhapsody");
    strcpy(records[6].album, "A Night at the Opera");
    strcpy(records[6].genre, "Rock");
    strcpy(records[6].duration, "5:55");
    strcpy(records[6].image_path, "pictures/1.jpg");
    
    strcpy(records[7].author, "Nirvana");
    strcpy(records[7].song, "Smells Like Teen Spirit");
    strcpy(records[7].album, "Nevermind");
    strcpy(records[7].genre, "Grunge");
    strcpy(records[7].duration, "5:01");
    strcpy(records[7].image_path, "pictures/1.jpg");
    
    strcpy(records[8].author, "Michael Jackson");
    strcpy(records[8].song, "Billie Jean");
    strcpy(records[8].album, "Thriller");
    strcpy(records[8].genre, "Pop");
    strcpy(records[8].duration, "4:54");
    strcpy(records[8].image_path, "pictures/1.jpg");
    
    strcpy(records[9].author, "Daft Punk");
    strcpy(records[9].song, "Get Lucky");
    strcpy(records[9].album, "Random Access Memories");
    strcpy(records[9].genre, "Disco/Funk");
    strcpy(records[9].duration, "4:08");
    strcpy(records[9].image_path, "pictures/1.jpg");
    
    run_gui(records, record_count);
    
    return 0;
}