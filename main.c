/*
    Author: Soeun Kim
    Date: 3, Mar, 2020
    Description: B+ tree main
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "bptree.h"

#define INPUT_BUFFER 5
#define MINIMUM_MAX_KEY 2
#define MAXIMUM_MAX_KEY 99


extern NODE *root;
char input_buffer[INPUT_BUFFER];


void print_option() {
    char *option[] = {
        "\n==============================\n",
        "1 : Tree 현황 보기\n",
        "2 : Tree에 추가하기\n",
        "3 : Tree에서 삭제하기\n",
        "4 : Data 가져 오기\n",
        "5 : Tree 검색\n",
        "6 : Tree 초기화\n",
        "7 : Sample Tree\n",
        "exit : 프로그램 종료\n",
        "==============================\n",
        "> ",
    };

    for (int i = 0; i < 11; i++)
        printf("%s", option[i]);
}

void initialize() {
    int max_key_count;
    do {
        printf("Node당 최대 키 갯수를 입력해주세요(%d ~ %d): ", MINIMUM_MAX_KEY, MAXIMUM_MAX_KEY);
        scanf("%s", input_buffer);
        max_key_count = atoi(input_buffer);
        initialize_tree(max_key_count);
    } while(max_key_count < MINIMUM_MAX_KEY || max_key_count > MAXIMUM_MAX_KEY);
}

void clear() {
    printf("현재 Tree를 모두 지우시겠습니까? (y/n)\n");
    scanf("%s", input_buffer);
    if (strcasecmp(input_buffer, "y") != 0)
        return;

    clear_tree();
    initialize();
}

int input_key(char *inform) {
    int confirm = 0;
    int result;
    do {
        printf("%s", inform);
        scanf("%s", input_buffer);
        result = atoi(input_buffer);
        if (result == 0 && strcmp(input_buffer, "0") != 0) {
            printf("잘못된 Key 형식입니다. int값을 입력하세요.\n");
            confirm = 0;
        } else {
            confirm = 1;
        }
    } while(!confirm);

    return result;
}

void input() {
    int key, data;
    int confirm = 0;

    key = input_key("\nTree에 추가할 Key를 입력하세요(int): ");

    do {
        printf("저장할 Data를 입력하세요(int): ");
        scanf("%s", input_buffer);
        data = atoi(input_buffer);
        if (data == 0 && strcmp(input_buffer, "0") != 0) {
            printf("잘못된 Data 형식입니다. int값을 입력하세요.\n");
            confirm = 0;
        }  else {
            confirm = 1;
        }
    } while(!confirm);

    insert_data(key, data);
}

void delete() {
    int key = input_key("\n삭제할 Key를 입력하세요(int): ");
    delete_data(key);
}

void data() {
    int key = input_key("\nData를 가져올 Key를 입력하세요(int): ");
    DATA *data = get_data(create_key(key));
    if (data == NULL)
        printf("해당하는 key가 없습니다.\n");
    else
        printf("Key %d 의 data는 %d 입니다.\n", key, *data);
}

void search() {
    int from = input_key("\n범위 검색 시작 Key를 입력하세요(int): ");
    int to = input_key("\n범위 검색 끝 Key를 입력하세요(int): ");
    range_search(from, to);
}

int is_exit(char *input_buffer) {
    if (strcasecmp(input_buffer, "exit") == 0)
        return 1;
    else
        return 0;
}

void example_tree() {
    insert_data(10, 100);
    insert_data(20, 200);
    insert_data(30, 300);
    insert_data(40, 400);
    insert_data(50, 500);
    insert_data(23, 230);
    insert_data(33, 330);
    insert_data(34, 340);
    insert_data(36, 360);
    insert_data(19, 190);
}

int main() {
    printf("B+ Tree를 시작합니다.\n");

    initialize();

    while(1) {
        print_option();
        scanf("%s", input_buffer);

        int command = atoi(input_buffer);
        switch(command) {
            case 1:
                show_tree(root);
                break;
            case 2:
                input();
                break;
            case 3:
                delete();
                break;
            case 4:
                data();
                break;
            case 5:
                search();
                break;
            case 6:
                clear();
                break;
            case 7:
                example_tree();
                break;
            default:
                if (is_exit(input_buffer))
                    return 0;
                printf("올바른 선택이 아닙니다.");
        }
    }

    return -1;
}
