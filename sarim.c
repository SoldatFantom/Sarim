/*
 * Ce programme a été développé avec l'aide de plusieurs outils et bibliothèques :
 *
 * - SDL2 et SDL_mixer, distribués sous la licence zlib.
 *   Plus d'informations : https://www.libsdl.org/license.php
 *
 * - FFmpeg, une bibliothèque multimédia puissante, sous licence LGPL.
 *   Plus d'informations : https://ffmpeg.org/
 *
 * - Assistance fournie par ChatGPT, un modèle de langage créé par OpenAI,
 *   pour répondre à des questions techniques et fournir des suggestions.
 *   Plus d'informations : https://openai.com/chatgpt
 *
 * - Débogage et création assurés par Soldat Fantôme, avec une aide précieuse pour
 *   identifier et corriger les erreurs du programme.
 *
 * Merci à toutes les personnes, technologies et entités qui ont contribué à ce projet.
 *
 * This program was developed with the help of several tools and libraries:
 *
 * - SDL2 and SDL_mixer, distributed under the zlib license.
 *   More information: https://www.libsdl.org/license.php
 *
 * - FFmpeg, a powerful multimedia library, under the LGPL license.
 *   More information: https://ffmpeg.org/
 *
 * - Assistance provided by ChatGPT, a language model created by OpenAI,
 *   to answer technical questions and offer suggestions.
 *   More information: https://openai.com/chatgpt
 *
 * - Debugging and creation provided by Soldat Fantôme, with valuable help in
 *   identifying and fixing program errors.
 *
 * Thanks to all the people, technologies, and entities who contributed to this project.
 *
 * Este programa fue desarrollado con la ayuda de varias herramientas y bibliotecas:
 *
 * - SDL2 y SDL_mixer, distribuidos bajo la licencia zlib.
 *   Más información: https://www.libsdl.org/license.php
 *
 * - FFmpeg, una poderosa biblioteca multimedia, bajo la licencia LGPL.
 *   Más información: https://ffmpeg.org/
 *
 * - Asistencia proporcionada por ChatGPT, un modelo de lenguaje creado por OpenAI,
 *   para responder a preguntas técnicas y ofrecer sugerencias.
 *   Más información: https://openai.com/chatgpt
 *
 * - Depuración y creación asegurada por Soldat Fantôme, con una ayuda valiosa para
 *   identificar y corregir los errores del programa.
 *
 * Gracias a todas las personas, tecnologías y entidades que contribuyeron a este proyecto.
 *
 * 本程序是通过多种工具和库的帮助开发的：
 *
 * - SDL2 和 SDL_mixer，在 zlib 许可下发布。
 *   更多信息：https://www.libsdl.org/license.php
 *
 * - FFmpeg，一个强大的多媒体库，在 LGPL 许可下发布。
 *   更多信息：https://ffmpeg.org/
 *
 * - 由 OpenAI 创建的语言模型 ChatGPT 提供的协助，
 *   用于回答技术问题并提供建议。
 *   更多信息：https://openai.com/chatgpt
 *
 * - 由 Soldat Fantôme 提供的调试和创作，帮助识别和修复程序错误。
 *
 * 感谢所有为此项目做出贡献的人、技术和实体。
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <libavformat/avformat.h>
#define SKIP_TIME 5 // secondes pour avancer/reculer
#define MAX_FILES 9000 // Nombre maximum de fichiers audio
#include<time.h>
void display_ihelp();
void handle_input();
bool can_execute = true;

// Fonction pour obtenir la durée totale d'un fichier MP3

int get_audio_duration(const char* filename) {
    AVFormatContext *format_ctx = NULL;

    // Initialiser libavformat et ouvrir le fichier audio
    if (avformat_open_input(&format_ctx, filename, NULL, NULL) != 0) {
        printf("Impossible d'ouvrir le fichier %s\n", filename);
        return -1;
    }

    // Lire les informations du fichier
    if (avformat_find_stream_info(format_ctx, NULL) < 0) {
        printf("Impossible de trouver les informations du fichier\n");
        avformat_close_input(&format_ctx);
        return -1;
    }

    // Trouver le flux audio
    AVStream *audio_stream = NULL;
    for (int i = 0; i < format_ctx->nb_streams; i++) {
        if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream = format_ctx->streams[i];
            break;
        }
    }

    if (!audio_stream) {
        printf("Aucun flux audio trouvé\n");
        avformat_close_input(&format_ctx);
        return -1;
    }

    // Calculer la durée en secondes
    int duration_in_seconds = (int)(format_ctx->duration / AV_TIME_BASE);

    // Libérer les ressources
    avformat_close_input(&format_ctx);

    return duration_in_seconds;
}
Mix_Music *music = NULL;
bool is_playing = false;
bool is_paused = false;
char *file_list[MAX_FILES];
int file_count = 0;
int current_index = 0;
int total_length = 0; // Durée totale de la musique en secondes

// Fonction pour jouer la musique
void play_audio(const char *file) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());

    }

    music = Mix_LoadMUS(file);
    if (!music) {
        display_ihelp();
        printw("\n\n ERROR: Failed to load music: \n\n Press Any Key to Quit .... ");


         char input;
    int valid = 0;

    while (!valid) {

        input = getch(); // Utiliser getch() pour récupérer la touche sans l'afficher

        if (input != -1) {
        endwin(); // Fin de ncurses
          exit(0);
        }
    }
    }

    // Vous devez définir la durée totale de la musique ici (par exemple, 240 secondes pour une piste de 4 minutes)
    total_length = get_audio_duration(file); // Remplacez ceci par la durée réelle de la musique

    Mix_PlayMusic(music, -1); // -1 pour jouer en boucle infinie
    is_playing = true;
    is_paused = false; // Réinitialiser l'état de pause
}

// Fonction pour libérer la musique
void stop_audio() {
    if (music) {
        Mix_FreeMusic(music);
        music = NULL;
    }
    Mix_CloseAudio();
    is_playing = false;
    is_paused = false;
}

// Fonction pour avancer ou reculer dans la musique
void skip_audio(int seconds) {

    if (is_playing) {
        int current_position = Mix_GetMusicPosition(music); // Position actuelle en secondes
        int new_position = current_position + seconds;

        // Limiter la position pour qu'elle ne soit pas inférieure à 0
        if (new_position < 0) {
            new_position = 0;
        }

        // Ne pas dépasser la durée totale de la musique


        Mix_SetMusicPosition(new_position);
    }
}

void display_progress(Mix_Music *music, const char* filename,int nopath) {
    int total_length = get_audio_duration(filename);  // Utilise la fonction pour obtenir la durée totale
    int current_position = Mix_GetMusicPosition(music);  // Obtenir la position actuelle

    if (total_length > 0 && current_position >= 0) {
        int remaining_time = total_length - current_position;
        int percentage = (current_position * 100) / total_length;

        clear();

      printw("\n\n           SARIM \n   \n   CREATED BY SOLDAT FANTOM OCTOBER 2024 \n\n");
      printw(" This program was developed with the help of SDL2, SDL_mixer (zlib license) and FFmpeg (LGPL license)\n");
      printw(" Assistance provided by ChatGPT, a language model created by OpenAI.\n");
      printw(" Debugging and development supported by Soldat Fantom, with valuable help in identifying and correcting program errors.\n");
      printw(" Thank you to all the people, technologies, and entities that contributed to this project!\n\n");
      printw(" Available commands:\n");
      printw(" d - open play_list Explorer\n");
      printw(" q - Quit the program\n");
      printw(" p - Pause/Resume the music\n");
      printw(" Right arrow - Fast forward a few seconds\n");
      printw(" Left arrow - Rewind a few seconds\n");
      printw(" Up arrow - Skip to the next track\n");
      printw(" Down arrow - Go back to the previous track\n");
      printw(" + - Increase volume\n");
      printw(" - - Decrease volume\n\n\n");

        printw(" ==========================================================================================================\n\n");
        printw(" Playing:");
        attron(COLOR_PAIR(4));
        if (nopath == 1){
            char *lastSlash = strrchr(filename, '/');
             printw("%s\n\n", lastSlash+1);
        }else{
        printw("%s\n\n", filename);
        }
        attroff(COLOR_PAIR(4));
        printw(" Time Remaining:");
        attron(COLOR_PAIR(3));

        printw(" %d:%02d",remaining_time / 60, remaining_time % 60);
        attroff(COLOR_PAIR(3));
        printw(" / Total: ");
        attron(COLOR_PAIR(3));


           printw("%d:%02d\n\n",total_length / 60, total_length % 60);
          attroff(COLOR_PAIR(3));
        printw(" Progress: [");
        attron(COLOR_PAIR(1));
        int bar_length = 40; // Longueur de la barre de progression
        int filled_length = (percentage * bar_length) / 100;
        for (int i = 0; i < bar_length; i++) {
            if (i < filled_length) {
                printw("=");
            } else {
                printw(" ");
            }
        }
        attroff(COLOR_PAIR(1));
        printw("] (");
        attron(COLOR_PAIR(6));
        printw("%d%%", percentage);
        attroff(COLOR_PAIR(6));
        printw(")");
        printw("\n\n ===========================================================================================================\n");
        refresh();
         if  (file_list[current_index] != 0 && percentage > 99 ){
            current_index++;
            if (current_index >= file_count) {
                current_index = 0; // Revenir au début
            }
            printw("Suivant: %s\n", file_list[current_index]);
            refresh();
            stop_audio();
            play_audio(file_list[current_index]);
            }
    }
}
void dir_playlist(int nopath)
{
    int count = 0;            // Compteur pour parcourir la liste de fichiers
    int current = 0;          // Indice de l'élément actuellement sélectionné dans la playlist
    int v = 1;                // Variable de contrôle pour la boucle principale (v = 0 pour quitter la boucle)

    clear();                  // Effacer l'écran avant de commencer

    while (v != 0) {          // Boucle principale qui continue tant que `v` n'est pas égal à 0

        if (v == 1) {         // Si v vaut 1, continuer l'exploration de la playlist
            SDL_Delay(10);    // Pause de 10 ms pour réduire l'utilisation du CPU
            clear();          // Effacer l'écran pour redessiner l'interface utilisateur

            // Affichage des instructions pour l'utilisateur
            printw(" Playlist Explorer \n\n Press q to return or ENTER For PLay_Selected audio_file\n\n");

            // Boucle pour afficher la liste des fichiers dans la playlist
            while (count <= file_count - 1) {  // Parcours de tous les fichiers dans la playlist
                if (current == count) {        // Si l'élément actuel correspond à l'élément sélectionné
                    attron(COLOR_PAIR(5));     // Activer un style de couleur pour l'élément sélectionné
                    if (nopath == 1){
                       char *lastSlash = strrchr(file_list[count], '/');
                       printw(" %s\n", lastSlash+1);
                    }else{
                        printw(" %s\n", file_list[count]);
                    }
                      // Afficher l'élément sélectionné avec style
                    attroff(COLOR_PAIR(5));    // Désactiver le style de couleur
                } else {
                  if (nopath == 1){
                       char *lastSlash = strrchr(file_list[count], '/');
                       printw(" %s\n", lastSlash+1);
                    }else{
                        printw(" %s\n", file_list[count]);
                    }
                }

                refresh();  // Rafraîchir l'affichage pour refléter les changements
                count++;    // Passer à l'élément suivant
            }

            count = 0;  // Réinitialiser `count` pour les futures boucles

            int ch = getch();  // Obtenir l'entrée utilisateur

            // Si l'utilisateur appuie sur 'q', retourner au fichier audio actuellement sélectionné
            if (ch == 'q') {


                // Boucle pour afficher la progression tant que la lecture est en cours
                while (is_playing) {
                    if (file_list[current_index] != 0) {  // Si le fichier audio n'est pas vide
                        display_progress(music, file_list[current_index],nopath);  // Mettre à jour la progression de la lecture
                    }
                    if (file_list[current_index] != 0) {
                        handle_input(file_list[current_index],nopath);  // Gérer les entrées utilisateur pour contrôler la lecture
                    }
                    SDL_Delay(10);  // Petite pause pour limiter l'utilisation du CPU
                }
            }
            if (ch == 'Q') {


                // Boucle pour afficher la progression tant que la lecture est en cours
                while (is_playing) {
                    if (file_list[current_index] != 0) {  // Si le fichier audio n'est pas vide
                        display_progress(music, file_list[current_index],nopath);  // Mettre à jour la progression de la lecture
                    }
                    if (file_list[current_index] != 0) {
                        handle_input(file_list[current_index],nopath);  // Gérer les entrées utilisateur pour contrôler la lecture
                    }
                    SDL_Delay(10);  // Petite pause pour limiter l'utilisation du CPU
                }
            }

            // Si l'utilisateur appuie sur la touche "Entrée" (ENTER), jouer le fichier sélectionné
            if (ch == '\n') {
                current_index = current;  // Mettre à jour l'index courant avec l'élément sélectionné
                play_audio(file_list[current_index]);  // Jouer l'audio sélectionné

                // Boucle pour gérer la progression de l'audio tant qu'il est en cours de lecture
                while (is_playing) {
                    if (file_list[current_index] != 0) {  // Si le fichier audio est valide
                        display_progress(music, file_list[current_index],nopath);  // Mettre à jour la progression
                    }
                    if (file_list[current_index] != 0) {
                        handle_input(file_list[current_index],nopath);  // Gérer les entrées utilisateur pour contrôler la lecture
                    }
                    SDL_Delay(10);  // Pause de 10 ms pour limiter l'utilisation du CPU
                }
            }

            // Si l'utilisateur appuie sur la touche "flèche haut" (KEY_UP)
            if (ch == KEY_UP) {
                current--;  // Décrémenter `current` pour remonter dans la playlist
                if (current < 0) {  // Si on dépasse le début de la liste
                    current = 0;  // Forcer `current` à rester à 0 (début de la liste)
                }
            }

            // Si l'utilisateur appuie sur la touche "flèche bas" (KEY_DOWN)
            if (ch == KEY_DOWN) {
                current++;  // Incrémenter `current` pour descendre dans la playlist
                if (current > file_count) {  // Si on dépasse la fin de la liste
                    current--;  // Forcer `current` à ne pas dépasser le dernier élément
                }
            }
        }
    }
}

// Fonction pour traiter les entrées de l'utilisateur avec ncurses
void handle_input(char * file,int nopath) {
    int ch = 0;
    ch = getch(); // Obtenir une entrée utilisateur
    switch (ch) {
        case 'q': // Quitter
            stop_audio();
            endwin(); // Fin de ncurses
            exit(0);
        break;
        case 'd': // Quitter
            if  (file_list[current_index] != 0){
                dir_playlist(nopath);
            }
        break;
        case 'p': // Pause
            if (is_playing) {
                if (is_paused) {
                    Mix_ResumeMusic();
                    is_paused = false;
                } else {
                    Mix_PauseMusic();
                    is_paused = true;
                }
            }
            break;
           case 'Q': // Quitter
            stop_audio();
            endwin(); // Fin de ncurses
            exit(0);
        break;
        case 'D': // Quitter
            if  (file_list[current_index] != 0){
                dir_playlist(nopath);
            }
        break;
        case 'P': // Pause
            if (is_playing) {
                if (is_paused) {
                    Mix_ResumeMusic();
                    is_paused = false;
                } else {
                    Mix_PauseMusic();
                    is_paused = true;
                }
            }
            break;
          case ' ': // Pause
            if (is_playing) {
                if (is_paused) {
                    Mix_ResumeMusic();
                    is_paused = false;
                } else {
                    Mix_PauseMusic();
                    is_paused = true;
                }
            }
            break;
        case KEY_RIGHT: // Avancer
            skip_audio(SKIP_TIME);
            break;
        case KEY_LEFT: // Reculer
            skip_audio(-SKIP_TIME);
            break;
        case KEY_UP: // Suivant
            if  (file_list[current_index] != 0){
            current_index++;
            if (current_index >= file_count) {
                current_index = 0; // Revenir au début   ////ici pour ka touche d logique de lecture du fichier playliste
            }
            printw("Suivant: %s\n", file_list[current_index]);
            refresh();
            stop_audio();
            play_audio(file_list[current_index]);
            }
            break;
        case KEY_DOWN : // Précédent
            if  (file_list[current_index] != 0){

            current_index--;
            if (current_index < 0) {
                current_index = file_count - 1; // Revenir à la fin
            }
            printw("Précédent: %s\n", file_list[current_index]);
            refresh();
            stop_audio();
            play_audio(file_list[current_index]);
            }else{
            play_audio(file);
            }
            break;
      case '+':  // Augmenter le volume
            {
                int current_volume = Mix_VolumeMusic(-1);  // Obtenir le volume actuel
                if (current_volume < MIX_MAX_VOLUME) {
                    Mix_VolumeMusic(current_volume + 8);  // Augmenter le volume par 8
                }
            }
            break;
        case '-':  // Diminuer le volume
            {
                int current_volume = Mix_VolumeMusic(-1);  // Obtenir le volume actuel
                if (current_volume > 0) {
                    Mix_VolumeMusic(current_volume - 8);  // Diminuer le volume par 8
                }
            }
            break;
    }
ch =0;
}

// Fonction pour parser le fichier M3U
void parse_m3u(const char *m3u_file) {
    FILE *file = fopen(m3u_file, "r");
    if (!file) {
        fprintf(stderr, "Could not open M3U file: %s\n", m3u_file);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }
        line[strcspn(line, "\n")] = 0; // Retirer le saut de ligne
        if (file_count < MAX_FILES) {
            file_list[file_count] = strdup(line); // Dupliquer la chaîne
            file_count++;
        } else {
            fprintf(stderr, "Maximum number of files exceeded.\n");
        }
    }
    fclose(file);
}

void display_help() {
       printf("Usage: sarim [OPTION] [FILE or DIRECTORY]\n");
    printf("Play audio files in MP3, WAV, or OGG format from a directory, a playlist (.m3u), or an individual file.\n\n");
    printf("Options:\n");
    printf("  --dir <directory>     Recursively scan the directory for .mp3, .wav, and .ogg files and play them.\n");
    printf("                        The playlist will be created dynamically and played in order.\n");
    printf("  --nopath              Suppress the display of file paths in the output. Can be placed before or after\n");
    printf("                        other options and arguments.\n");
    printf("  <file.m3u>            Play a playlist file in M3U format. The playlist should contain\n");
    printf("                        paths to .mp3, .wav, or .ogg files. The files will be played in the order listed.\n");
    printf("  <file.mp3 | file.wav | file.ogg> Play a single .mp3, .wav, or .ogg file directly.\n\n");
    printf("Supported formats:\n");
    printf("  This player currently supports MP3, WAV, and OGG audio files.\n\n");
    printf("Examples:\n");
    printf("  sarim --dir /path/to/music           Play all .mp3, .wav, and .ogg files in the specified directory.\n");
    printf("  sarim --nopath --dir /path/to/music  Same as above but suppress the display of file paths during playback.\n");
    printf("  sarim my_playlist.m3u                Play the .mp3, .wav, and .ogg files listed in the M3U playlist.\n");
    printf("  sarim song.mp3                       Play the specified .mp3 file.\n");
    printf("  sarim sound.wav                      Play the specified .wav file.\n");
    printf("  sarim track.ogg                      Play the specified .ogg file.\n");
    printf("  sarim --nopath song.mp3              Play the specified .mp3 file but suppress the file path display.\n\n");
    printf("Controls during playback:\n");
    printf("  d                 Open play_list Explorer.\n");
    printf("  q                 Quit the player.\n");
    printf("  p                 Pause/Resume the playback.\n");
    printf("  Right Arrow       Skip forward in the track.\n");
    printf("  Left Arrow        Rewind in the track.\n");
    printf("  Up Arrow          Play the next track in the playlist.\n");
    printf("  Down Arrow        Play the previous track in the playlist.\n");
    printf("  +                 Increase the volume.\n");
    printf("  -                 Decrease the volume.\n");

}

void display_ihelp() {
    printw("Usage: sarim [OPTION] [FILE or DIRECTORY]\n");
    printw("Play audio files in MP3, WAV, or OGG format from a directory, a playlist (.m3u), or an individual file.\n\n");
    printw("Options:\n");
    printw("  --dir <directory>     Recursively scan the directory for .mp3, .wav, and .ogg files and play them.\n");
    printw("                        The playlist will be created dynamically and played in order.\n");
    printw("  --nopath              Suppress the display of file paths in the output. Can be placed before or after\n");
    printw("                        other options and arguments.\n");
    printw("  <file.m3u>            Play a playlist file in M3U format. The playlist should contain\n");
    printw("                        paths to .mp3, .wav, or .ogg files. The files will be played in the order listed.\n");
    printw("  <file.mp3 | file.wav | file.ogg> Play a single .mp3, .wav, or .ogg file directly.\n\n");
    printw("Supported formats:\n");
    printw("  This player currently supports MP3, WAV, and OGG audio files.\n\n");
    printw("Examples:\n");
    printw("  sarim --dir /path/to/music           Play all .mp3, .wav, and .ogg files in the specified directory.\n");
    printw("  sarim --nopath --dir /path/to/music  Same as above but suppress the display of file paths during playback.\n");
    printw("  sarim my_playlist.m3u                Play the .mp3, .wav, and .ogg files listed in the M3U playlist.\n");
    printw("  sarim song.mp3                       Play the specified .mp3 file.\n");
    printw("  sarim sound.wav                      Play the specified .wav file.\n");
    printw("  sarim track.ogg                      Play the specified .ogg file.\n");
    printw("  sarim --nopath song.mp3              Play the specified .mp3 file but suppress the file path display.\n\n");
    printw("Controls during playback:\n");
    printw("  d                 Open play_list Explorer.\n");
    printw("  q                 Quit the player.\n");
    printw("  p                 Pause/Resume the playback.\n");
    printw("  Right Arrow       Skip forward in the track.\n");
    printw("  Left Arrow        Rewind in the track.\n");
    printw("  Up Arrow          Play the next track in the playlist.\n");
    printw("  Down Arrow        Play the previous track in the playlist.\n");
    printw("  +                 Increase the volume.\n");
    printw("  -                 Decrease the volume.\n");


}
int main(int argc, char *argv[]) {
    int u = 0;
    int a =0;
    int nopath = 0;
    int v =0;
     if (argc < 2 || strstr(argv[1], "--help") != NULL) {
        display_help();
        return 1;
    }
    if (strstr(argv[1], "--version") != NULL) {
        printf("Sarim Actualy version is V_1.5\n");
        return 0;

    }

     initscr(); // Commencer le mode ncurses
    cbreak(); // Activer le mode cbreak
    noecho(); // Ne pas afficher les caractères saisis
    keypad(stdscr, TRUE); // Activer les touches spéciales
    nodelay(stdscr, TRUE); // Ne pas bloquer l'attente d'entrées
    start_color();       // Activer les couleurs

    // Initialiser les paires de couleurs
    init_pair(1, COLOR_GREEN, COLOR_BLACK);     // Texte vert
    init_pair(2, COLOR_RED, COLOR_BLACK);       // Texte rouge
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);    // Texte jaune
    init_pair(4, COLOR_CYAN, COLOR_BLACK);      // Texte cyan (bleu clair)
    init_pair(5, COLOR_BLACK,COLOR_WHITE);      // Texte blanc
    init_pair(6, COLOR_BLUE, COLOR_BLACK);      // Texte bleu
    clear();
    while (v != 1) {
        a++;



        // Rechercher "--nopath" dans les arguments
        if (strstr(argv[a], "--nopath") != NULL) {
            nopath = 1;

        }
         if (a == argc-1) {  // Vérifier que 'a' ne dépasse pas 'argc'

            v = 1 ; // Sortir de la boucle si 'a' dépasse la plage
        }


    }
a =0;

    while (1){

    a ++;

     if (strstr(argv[a], "--dir") != NULL) {
         u = 1;
         a ++;
        char dir_name [900];

        sprintf(dir_name,"find '%s' -type f  -name '*.mp3' -o -name '*.wav' -o -name '*.ogg' >/tmp/tmp.m3u",argv[a]);
        system(dir_name);
            parse_m3u("/tmp/tmp.m3u");
        if (file_count > 0) {
            current_index = 0; // Commencer par le premier fichier
            printw("Playing: %s\n", file_list[current_index]);
            refresh();
            play_audio(file_list[current_index]);
        } else {
            printw("No audio files found in the playlist.\n");
            refresh();
            getch();
            endwin();
            return 1;
        }

        }else if (strstr(argv[a], ".m3u") != NULL) {
            u = 1;
        parse_m3u(argv[a]);
        if (file_count > 0) {
            current_index = 0; // Commencer par le premier fichier
            printw("Playing: %s\n", file_list[current_index]);
            refresh();
            play_audio(file_list[current_index]);
        } else {
            printw("No audio files found in the playlist.\n");
            refresh();
            getch();
            endwin();
            return 1;
        }
    }

 //si ont depasse le nombre d'option passer ont essaye de lire un audio seule
    if (a == argc-1 && u == 0) {
        play_audio(argv[1]);
    }

    // Gérer l'entrée utilisateur pendant la lecture
   while (is_playing) {
    if  (file_list[current_index] != 0){
        display_progress(music, file_list[current_index],nopath);  // Mettez à jour l'affichage

    }else
    {
        display_progress(music, argv[1],nopath);  // Mettez à jour l'affichage

    }
    if  (file_list[current_index] != 0){
        handle_input(file_list[current_index],nopath);
    }else{
    handle_input(argv[1],nopath);
    }
      // Vérifiez les entrées utilisateur

    SDL_Delay(10);  // Petite pause pour limiter l'utilisation CPU
}

    // Libérer la mémoire
    for (int i = 0; i < file_count; i++) {
        free(file_list[i]);
    }

    }
    endwin(); // Fin de ncurses

    return 0;


    }
