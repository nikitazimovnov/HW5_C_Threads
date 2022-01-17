#include <pthread.h>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

#define SLEEP_TIME 12 // Время отдыха гостя в номере (рекомендуется ставить 8 - 15).
#define FREE 1
#define OCCUPY -1

typedef struct {
    int room_200;
    int room_400;
    int room_600;
}
        hotel_t;

hotel_t hotel = {10, 10, 5};

void *reception(void *param);

int receptionist(int sum, hotel_t *_hotel);

void show_rooms();

void handle_room(int room, int action);

void *reception(void *param) {
    int sum = *(int *) param;
    int room = receptionist(sum, &hotel);
    if (room == -2 || room == -1)
        pthread_exit(nullptr);

    handle_room(room, OCCUPY);
    printf("---check in---\n");
    show_rooms();

    sleep(SLEEP_TIME);

    handle_room(room, FREE);
    printf("---check out---\n");
    show_rooms();

    pthread_exit(nullptr);
}

int receptionist(int sum, hotel_t *_hotel) {
    if (!_hotel)
        return -1;

    if (sum >= 600 && _hotel->room_600 > 0)
        return 6;
    else if (sum >= 600 && _hotel->room_400 > 0)
        return 4;
    else if (sum >= 600 && _hotel->room_200 > 0)
        return 2;
    else if (sum >= 400 && _hotel->room_400 > 0)
        return 4;
    else if (sum >= 400 && _hotel->room_200 > 0)
        return 2;
    else if (sum >= 200 && _hotel->room_200 > 0)
        return 2;
    else
        printf("Not enough money.\n");
        return -2;
}

void handle_room(int room, int action) {
    switch (room) {
        case 2:
            hotel.room_200 += action;
            break;
        case 4:
            hotel.room_400 += action;
            break;
        case 6:
            hotel.room_600 += action;
            break;
        default:
            break;
    }
}

void show_rooms() {
    printf("Guest ID: %lu\n", pthread_self());
    printf("Room 200: %d\t", hotel.room_200);
    printf("Room 400: %d\t", hotel.room_400);
    printf("Room 600: %d\n", hotel.room_600);
}

int main(int argc, char* argv[]) {
    unsigned int start_time = clock();

    if (argc != 2) {
        printf("Incorrect command line.\nPrint -f to use program in handle mode.\nPrint -g to use program in randomising mode.\n");
        return 1;
    }

    if(!strcmp(argv[1], "-f")) {
        int money;

        int client = 0;
        while (scanf("%d", &money) == 1 && money >= 0) {
            client++;
            void *ptr_money = &money;
            auto thread_id = (pthread_t)client;
            pthread_create(&thread_id, nullptr, reception, ptr_money);
        }
    }
    else if (!strcmp(argv[1], "-g")) {
        int clients_count = 0;
        scanf("%d", &clients_count);

        int client = 0;
        int money;
        for (int i = 0; i < clients_count; ++i) {
            client++;
            money = rand() % 3000;
            printf("%d\n", money);
            void *ptr_money = &money;
            auto thread_id = (pthread_t)client;
            pthread_create(&thread_id, nullptr, reception, ptr_money);
        }
    }

    unsigned int end_time = clock();
    printf("\nWorking time: %d\n", end_time - start_time);
}
