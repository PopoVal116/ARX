#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include"../include/file_handler.h"
#include "../include/sort.h"
#include "../include/binary_search.h"
#include "../include/optimal_tree.h"

using namespace std;

#define MAX_RECORDS 100

enum SortState {
    SORT_NONE,
    SORT_ASC,
    SORT_DESC
};

struct Texture {
    unsigned int id;
    int width;
    int height;
    bool loaded;
};

bool loadTextureFromFile(const char* filename, Texture& texture) {
    
    int channels;
    unsigned char* image_data = stbi_load(filename, &texture.width, &texture.height, &channels,4 ); //загрузка в оп
    
    if (image_data == NULL) {
        cout << "Failed to load image: " << filename << endl;
        texture.loaded = false;
        return false;
    }
    
    glGenTextures(1, &texture.id); //generate 1 texture (выделить память в vram)
    glBindTexture(GL_TEXTURE_2D, texture.id); // Активируем текстуру
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Плавное масштабирование(уменьшение)
    
    glTexImage2D(          // из ram в vram
        GL_TEXTURE_2D,      // тип текстуры
        0,                  // уровень детализации (0 = основной)
        GL_RGBA,            // внутренний формат хранения
        texture.width,      // ширина
        texture.height,     // высота
        0,                  // граница (всегда 0)
        GL_RGBA,            // формат входных данных
        GL_UNSIGNED_BYTE,   // тип данных (байты)
        image_data          // данные пикселей
        );
    
    texture.loaded = true;
    cout << "Loaded texture: " << filename << " (" << texture.width << "x" << texture.height << ")" << endl;
    return true;
}


void sortRecords(Song** song_ptrs, int record_count, SortState state) {
    cout << "SORT: ";
    if (state == SORT_NONE) {
        cout << "NONE" << endl;
    }
    else if (state == SORT_ASC) {
        cout << "ASC (by title)" << endl;
        quick_sort_by_title_asc(song_ptrs, 0, record_count - 1);
    }
    else { 
        cout << "DESC (by title)" << endl;
        quick_sort_by_title_desc(song_ptrs, 0, record_count - 1);
    }
    
}


void DisplayRecordsTable(Song** song_ptrs, int record_count, int& selected_record, bool& show_photo_window) {
    if (ImGui::BeginTable("records_table", 5,
        ImGuiTableFlags_Borders |           // границы
        ImGuiTableFlags_RowBg |              // чередование фона строк
        ImGuiTableFlags_ScrollY, 
        ImVec2(0, 500))) {  
        
        ImGui::TableSetupColumn("Song", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Author", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Album", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Genre", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Duration", ImGuiTableColumnFlags_WidthFixed, 80.0f); 

        // ЗАГОЛОВКИ
        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
   
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Song");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("Author");
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("Album");
        ImGui::TableSetColumnIndex(3);
        ImGui::Text("Genre");
        ImGui::TableSetColumnIndex(4);
        ImGui::Text("Duration");

        // Данные
        for (int i = 0; i < record_count; i++) {
            ImGui::TableNextRow();
            
            // Песня - кнопка во всю ячейку
            ImGui::TableSetColumnIndex(0);
            ImGui::PushID(i); //id кнопки в стек
            float cell_width = ImGui::GetContentRegionAvail().x;
            if (ImGui::Button(song_ptrs[i]->song, ImVec2(cell_width, 0))) {
                selected_record = i;
                show_photo_window = true;
            }
            ImGui::PopID();
            
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", song_ptrs[i]->author);
            
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", song_ptrs[i]->album);
            
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s", song_ptrs[i]->genre);
            
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%s", song_ptrs[i]->duration);
        }
        
        ImGui::EndTable();
    }
}

void run_gui(Song records[], int& record_count) {  
    cout << "GUI started" << endl;

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
        
    bool running = true;
    int selected_record = -1;
    bool show_photo_window = false;
    
    // Переменные:

    // Поиск
    char search_buffer_for_user_input[256] = "";
    char last_search_request[256] = "";  
    bool show_search_result = false;
    Song* search_results[MAX_RECORDS];
    int search_results_count = 0;

     // Поиск 2
    char search_buffer_for_user_input2[256] = "";
    char last_search_request2[256] = "";  
    bool show_search_result2 = false;
    Song* search_results2[MAX_RECORDS];
    int search_results_count2 = 0;
    
    // Сортировка
    SortState current_sort = SORT_NONE;
    
    // Переменные добавления
    bool show_add_window = false;
    char new_author[100] = "";
    char new_song[100] = "";
    char new_album[100] = "";
    char new_genre[50] = "";
    char new_duration[20] = "";

    //Массив указателей
    Song** song_ptrs = new Song*[record_count];
    for (int i = 0; i < record_count; i++) {
        song_ptrs[i] = &records[i];
    }

    
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
        ImGui::Begin("Music Records", nullptr, ImGuiWindowFlags_NoCollapse); //размер авто
        
        if (record_count == 0) {
            ImGui::Text("No records loaded");
        } else {
            // Отдельная кнопка над таблицей
            ImGui::Text("Sort by Title:");
            ImGui::SameLine();

            const char* sort_btn;
            if (current_sort == SORT_NONE) sort_btn = "NONE";
            else if (current_sort == SORT_ASC) sort_btn = "ASC";
            else sort_btn = "DESC";

            if (ImGui::Button(sort_btn)) {
                if (current_sort == SORT_NONE)
                    current_sort = SORT_ASC;
                else if (current_sort == SORT_ASC)
                    current_sort = SORT_DESC;
                else
                    current_sort = SORT_ASC;
                
                sortRecords(song_ptrs, record_count, current_sort);
            }

            ImGui::Separator();
            
            // таблица
            DisplayRecordsTable(song_ptrs, record_count, selected_record, show_photo_window);
            ImGui::Text("Total records: %d", record_count);
            ImGui::Dummy(ImVec2(0, 10));
            ImGui::Separator();


            // ---------------------------ПОИСК----------------------------------------
           
            ImGui::Text("Binary Search:");
            ImGui::InputText("##search", search_buffer_for_user_input, sizeof(search_buffer_for_user_input));
            ImGui::SameLine();
            if (ImGui::Button("Find##binary")) {
                search_results_count = 0;

                if (strlen(search_buffer_for_user_input) > 0) {
                    search_results_count = binary_search_song(records, record_count, search_buffer_for_user_input, search_results, MAX_RECORDS);
                    strcpy(last_search_request, search_buffer_for_user_input);
                    show_search_result = true;
                } else {
                    show_search_result = false;
                }
            }

            
            if (show_search_result && strlen(last_search_request) > 0) {
                if (search_results_count > 0) {
                    ImGui::Text("Search results for '%s':", last_search_request);
        
                    float table_height = (search_results_count +1) * 20.0f; //высота: каждая строка ~20px + заголовок
                    table_height = min(table_height, 300.0f); //чтобы не выходило за окно

                    //таблица с результатами
                    if (ImGui::BeginTable("search_results_table", 5,
                        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY,
                        ImVec2(0, table_height))) {

                      
            
                        ImGui::TableSetupColumn("Song", ImGuiTableColumnFlags_WidthStretch);
                        ImGui::TableSetupColumn("Author", ImGuiTableColumnFlags_WidthStretch);
                        ImGui::TableSetupColumn("Album", ImGuiTableColumnFlags_WidthStretch);
                        ImGui::TableSetupColumn("Genre", ImGuiTableColumnFlags_WidthStretch);
                        ImGui::TableSetupColumn("Duration", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                        
                        // ЗАГОЛОВКИ
                        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
        
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("Song");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("Author");
                        ImGui::TableSetColumnIndex(2);
                        ImGui::Text("Album");
                        ImGui::TableSetColumnIndex(3);
                        ImGui::Text("Genre");
                        ImGui::TableSetColumnIndex(4);
                        ImGui::Text("Duration");

                        //данные

                        for (int i = 0; i < search_results_count; i++) {
                            ImGui::TableNextRow();
                            
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("%s", search_results[i]->song);
                            
                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text("%s", search_results[i]->author);
                            
                            ImGui::TableSetColumnIndex(2);
                            ImGui::Text("%s", search_results[i]->album);
                            
                            ImGui::TableSetColumnIndex(3);
                            ImGui::Text("%s", search_results[i]->genre);
                            
                            ImGui::TableSetColumnIndex(4);
                            ImGui::Text("%s", search_results[i]->duration);
                        }
            
                        ImGui::EndTable();
                    }
        
                ImGui::Text("Total found: %d", search_results_count);
                } else {
                    ImGui::TextColored(ImVec4(1,0,0,1), "No songs found matching '%s'", last_search_request);
                }
            }
            ImGui::Dummy(ImVec2(0, 10));
            ImGui::Separator();

            // -------------------------ПОИСК2----------------------------------------
           
            ImGui::Text("Optimal tree search:");
            ImGui::InputText("##search2", search_buffer_for_user_input2, sizeof(search_buffer_for_user_input2));
            ImGui::SameLine();
            if (ImGui::Button("Find##optimal")) {
                search_results_count2 = 0;

                if (strlen(search_buffer_for_user_input2) > 0) {
                    search_results_count2 = find_song_by_title(records, record_count, search_buffer_for_user_input2, search_results2, MAX_RECORDS);
                    strcpy(last_search_request2, search_buffer_for_user_input2);
                    show_search_result2 = true;
                } else {
                    show_search_result2 = false;
                }
            }

            
            if (show_search_result2 && strlen(last_search_request2) > 0) {
                if (search_results_count2 > 0) {
                    ImGui::Text("Search results for '%s':", last_search_request2);
        
                    float table_height2 = (search_results_count2 + 1) * 20.0f; 
                    table_height2 = min(table_height2, 300.0f); 

                    if (ImGui::BeginTable("search_results_table2", 5,
                        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY,
                        ImVec2(0, table_height2))) {
            
                        ImGui::TableSetupColumn("Song", ImGuiTableColumnFlags_WidthStretch);
                        ImGui::TableSetupColumn("Author", ImGuiTableColumnFlags_WidthStretch);
                        ImGui::TableSetupColumn("Album", ImGuiTableColumnFlags_WidthStretch);
                        ImGui::TableSetupColumn("Genre", ImGuiTableColumnFlags_WidthStretch);
                        ImGui::TableSetupColumn("Duration", ImGuiTableColumnFlags_WidthFixed, 80.0f);

                        // ЗАГОЛОВКИ
                        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
        
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("Song");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("Author");
                        ImGui::TableSetColumnIndex(2);
                        ImGui::Text("Album");
                        ImGui::TableSetColumnIndex(3);
                        ImGui::Text("Genre");
                        ImGui::TableSetColumnIndex(4);
                        ImGui::Text("Duration");

                        //данные
                        
                        for (int i = 0; i < search_results_count2; i++) {
                            ImGui::TableNextRow();
                            
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("%s", search_results2[i]->song);
                            
                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text("%s", search_results2[i]->author);
                            
                            ImGui::TableSetColumnIndex(2);
                            ImGui::Text("%s", search_results2[i]->album);
                            
                            ImGui::TableSetColumnIndex(3);
                            ImGui::Text("%s", search_results2[i]->genre);
                            
                            ImGui::TableSetColumnIndex(4);
                            ImGui::Text("%s", search_results2[i]->duration);
                        }
            
                        ImGui::EndTable();
                    }
        
                ImGui::Text("Total found: %d", search_results_count2);
                } else {
                    ImGui::TextColored(ImVec4(1,0,0,1), "No songs found matching '%s'", last_search_request2);
                }
            }

            
            // --------------------------------------НОВАЯ ПЕСНЯ--------------------------------

            // Кнопка добавления
            ImGui::Dummy(ImVec2(0, 10));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 10));
            if (ImGui::Button("Add New Song", ImVec2(200, 30))) {
                show_add_window = true;
                new_author[0] = '\0';
                new_song[0] = '\0';
                new_album[0] = '\0';
                new_genre[0] = '\0';
                new_duration[0] = '\0';
            }
            
            // Окно добавления
            if (show_add_window) {
                ImGui::Begin("Add New Song", &show_add_window, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
                
                ImGui::Text("Enter record details:");
                ImGui::Separator();
                
                ImGui::InputText("Author", new_author, sizeof(new_author));
                ImGui::InputText("Song", new_song, sizeof(new_song));
                ImGui::InputText("Album", new_album, sizeof(new_album));
                ImGui::InputText("Genre", new_genre, sizeof(new_genre));
                ImGui::InputText("Duration", new_duration, sizeof(new_duration));
                
                ImGui::Separator();
                
                if (ImGui::Button("Save", ImVec2(120, 0))) {
                    Song new_record;
                    
                    strcpy(new_record.author, (strlen(new_author) > 0) ? new_author : "");
                    strcpy(new_record.song, (strlen(new_song) > 0) ? new_song : "");
                    strcpy(new_record.album, (strlen(new_album) > 0) ? new_album : "");
                    strcpy(new_record.genre, (strlen(new_genre) > 0) ? new_genre : "");
                    strcpy(new_record.duration, (strlen(new_duration) > 0) ? new_duration : "");
                    
                    strcpy(new_record.image_path, "../../covers/default.png");
                  
                    
                    if (record_count < MAX_RECORDS) {
                        records[record_count] = new_record;
                        record_count++;

                        // Пересоздаем массив указателей
                        delete[] song_ptrs;
                        song_ptrs = new Song*[record_count];
                        for (int i = 0; i < record_count; i++) {
                            song_ptrs[i] = &records[i];
                        }

                        // Сохраняем обновленную базу в файл
                        if (save_database("../../data/music_db.dat", records, record_count) == record_count) {
                            cout << "Added new record and saved to database" << endl;
                        } else {
                            cout << "Error: Failed to save database" << endl;
                        }
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
        
        // -----------------ВИДЖЕТ С ОБЛОЖКОЙ--------------------------------------
        if (show_photo_window && selected_record >= 0 && selected_record < record_count) {
           
            
            ImGui::Begin("Photo", &show_photo_window, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
            
            ImGui::Text("Song: %s", song_ptrs[selected_record]->song);
            ImGui::Text("Artist: %s", song_ptrs[selected_record]->author);
            ImGui::Separator();

            // Текстура для текущей записи
            static Texture temp_texture;
            static char last_loaded_path[200] = "";
            static bool texture_loaded = false;

            // Если изменился путь к фото(смена песни), перезагружаем текстуру
            if (strcmp(last_loaded_path, song_ptrs[selected_record]->image_path) != 0) {
                if (texture_loaded) {
                    glDeleteTextures(1, &temp_texture.id);
                }
                strcpy(last_loaded_path, song_ptrs[selected_record]->image_path);
                    texture_loaded = loadTextureFromFile(last_loaded_path, temp_texture);
            }
            
            if (texture_loaded) {
                ImGui::Image((void*)(intptr_t)temp_texture.id, ImVec2(400, 400));
                ImGui::SetWindowSize(ImVec2(420, 480), ImGuiCond_Always);
            } else {
                ImGui::TextColored(ImVec4(1,0,0,1), "Failed to load image!");
                ImGui::Text("Path: %s", song_ptrs[selected_record]->image_path);
            }
            
            ImGui::End();
        }
        
        ImGui::Render();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
    delete[] song_ptrs;
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    cout << "GUI finished" << endl;
}


int main() {    
    Song songs[MAX_RECORDS];
    int songs_count = 0;

    const char* db_path = "../../data/music_db.dat";

    int loaded_count = load_database(db_path, songs, MAX_RECORDS);
    
    if (loaded_count > 0) {
        songs_count = loaded_count;
    } else {
        cout << "Error: failed to load database" <<  endl;
    }
    
    run_gui(songs, songs_count);

    return 0;
}
