
//  Copyright Ma Xueqian, Sun Yiqi 2023.

#include "pch.h"      // NOLINT
#include "sudoku.h"   // NOLINT
#include <ctime>      // NOLINT
#include <algorithm>  // NOLINT
#include <string>     // NOLINT
#include <stdlib.h>   // NOLINT
#include <fstream>    // NOLINT
#include <iostream>   // NOLINT

extern char board[9][9] = {};
int count_simple;

static char buf[200000000];

// 生成num个数独终盘
void generate_c(int num) {
    if (num <= 0)
        throw std::exception("输入数据不合法");
    int n = num;
    buf[0] = '\0';
    int loc = 0;
    char line[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    srand((unsigned int)time(NULL));
    std::random_shuffle(line, line + 9);
    char formatline[19] = {'1', ' ', '2', ' ', '3', ' ', '4',
        ' ', '5', ' ', '6', ' ', '7', ' ', '8', ' ', '9', '\n', '\0'};
    int off[9] = { 0, 6, 12, 2, 8, 14, 4, 10, 16 };
    char res[10][19];
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 17; j++) {
            res[i][j] = ' ';
        }
        res[i][17] = '\n';
        res[i][18] = '\0';
    }
    res[9][0] = '\n';
    res[9][1] = '\0';

    int pos1[6][3] = { {3, 4, 5}, {3, 5, 4}, {4, 5, 3},
        {4, 3, 5}, {5, 4, 3}, {5, 3, 4} };
    int pos2[6][3] = { {6, 7, 8}, {6, 8, 7}, {7, 6, 8},
        {7, 8, 6}, {8, 6, 7}, {8, 7, 6} };

    FILE* fp = fopen(RESPATH, "w");

    do {
        for (int i = 0; i < 9; i++) {
            formatline[2 * i] = line[i];
        }
        memcpy(res[0], formatline, sizeof(formatline));
        for (int i = 1; i < 9; i++) {
            for (int j = 0; j < 18; j+=2) {
                res[i][j] = formatline[(j + off[i]) % 18];
            }
        }

        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                for (int k = 0; k < 3; k++) {
                    strncpy(buf + loc, res[k], 19);
                    loc += 18;
                }
                for (int k = 0; k < 3; k++) {
                    strncpy(buf + loc, res[pos1[i][k]], 19);
                    loc += 18;
                }
                for (int k = 0; k < 3; k++) {
                    strncpy(buf + loc, res[pos2[j][k]], 19);
                    loc += 18;
                }
                strncpy(buf + loc, "\n", 1);
                loc++;

                if (num == 1) {
                    buf[163 * (n - 1) + 161] = '\0';
                    fputs(buf, fp);
                }

                num--;
                if (num == 0) {
                    fclose(fp);
                    return;
                }
            }
        }
    } while (std::next_permutation(line, line + 9));
}

void generategame(int num, int num1, int num2) {
    if (num <= 0)
        throw std::exception("输入数据不合法");
    FILE* fpQues;
    FILE* fpAnsw;
    char str[200];

    fpAnsw = fopen(RESPATH, "r");
    fpQues = fopen(QUESPATH, "w");

    char ques_board[10][20];
    ques_board[9][0] = '\n';
    ques_board[9][1] = '\0';

    while (num--) {
        str[0] = '\0';
        for (int i = 0; i < 9; i++) {
            fgets(ques_board[i], 20, fpAnsw);
        }
        fgetc(fpAnsw);
        int base[9] = { 0, 6, 12, 54, 60, 66, 108, 114, 120 };
        int plus[9] = { 0, 2, 4, 18, 20, 22, 36, 38, 40 };

        for (int k = 0; k < 9; k++) {
            int tmp[2];
            srand((unsigned int)time(NULL));
            tmp[0] = rand() % 9;  // NOLINT
            srand((unsigned int)time(NULL));
            tmp[1] = rand() % 9;  // NOLINT
            while (tmp[0] == tmp[1]) {
                srand((unsigned int)time(NULL));
                tmp[1] = rand() % 9;  // NOLINT
            }
            for (int t = 0; t < 2; t++) {   // 选2个挖空（每行被选中的概率相等）
                int loc = base[k] + plus[tmp[t]];
                int r = loc / 18;   // 行
                int c = loc % 18;   // 列
                ques_board[r][c] = '$';
            }
        }

        int leave;
        if ( (num1 < 0) && (num2 < 0) ) {
            leave = 7 + rand() % 31;  // NOLINT
        } else {
            int y = rand() % (num2 - num1 + 1);  // NOLINT
            leave = num1 - 18 + y;
        }
        while (leave--) {
            int k = rand() % 81;  // NOLINT
            int r = k / 9;   // 行
            int c = k % 9;   // 列
            c *= 2;
            if (ques_board[r][c] != '$')
                ques_board[r][c] = '$';
            else
                leave++;
        }
        for (int i = 0; i < 10; i++) {
            strncat(str, ques_board[i], 20);
        }
        if (num == 0) {
            str[162] = '\0';
        }
        fputs(str, fpQues);
    }
    fclose(fpAnsw);
    fclose(fpQues);
    return;
}

int getnumber(char* str) {
    int res = 0;
    const char* p;
    for (p = str; *p; p++) {
        int t = 0;
        if (isdigit(*p)) {
            t = *p - '0';
        } else {
            throw std::exception("输入数据类型有误");
        }
        res *= 10;
        res += t;
    }
    return res;
}

void getdig(char* str, int &digup, int &digdown) {  // NOLINT
    int res = 0;
    const char* p;
    for (p = str; *p; p++) {
        int t = 0;
        if (isdigit(*p)) {
            t = *p - '0';
        } else if (*p == '~') {
            digdown = res;
            p++;
            break;
        } else {
            throw std::exception("输入数据类型有误");
        }
        res *= 10;
        res += t;
    }
    res = 0;
    for (; *p; p++) {
        int t = 0;
        if (isdigit(*p)) {
            t = *p - '0';
        } else {
            throw "输入数据类型有误";
        }
        res *= 10;
        res += t;
    }
    digup = res;
}

bool read_file(std::ifstream& infile, char* path) {
    if (!infile.is_open())
        infile.open(path, std::ios::in);
    while (!infile.is_open()) {
        std::cout << "读取文件失败，请重新输入路径:\n";
        std::cin >> path;
        infile.open(path, std::ios::in);
    }
    char temp;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            infile >> temp;
            bool ifread = infile.eof();
            if (ifread) {
                throw std::exception("数据输入异常");
            }
            while (temp != '$' && (temp < '1' || temp>'9')) {
                infile >> temp;
                if (infile.eof()) {
                    return infile.eof();
                }
            }
            board[i][j] = temp;
            std::cout << board[i][j];
        }
        std::cout << '\n';
    }
    bool ifread = infile.eof();
    if (ifread)
        std::cout << "数独读取完毕\n\n";
    return ifread;
}

void write_file(char* path) {
    std::fstream outfile;
    outfile.open(path, std::ios::out | std::ios::app);
    if (!outfile.is_open()) {
        std::cout << "写入文件失败" << std::endl;
        throw "写入文件失败";
        return;
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            outfile << board[i][j];
        }
        outfile << '\n';
    }
    outfile << '\n';
    outfile.close();
}

void write_fail(char* path) {
    std::fstream outfile;
    outfile.open(path, std::ios::out | std::ios::app);
    if (!outfile.is_open()) {
        std::cout << "写入文件失败" << std::endl;
        throw "写入文件失败";
        return;
    }
    outfile << "该数独无解！\n";
    outfile.close();
}

bool is_valid(int row, int col, int num) {
    // 检查行有没有重复的，如果有返回false
    for (int i = 0; i < SIZE; i++) {
        if (num == board[row][i] - '0' && i != col) {
            return false;
        }
    }
    // 检查列有没有重复的，如果有返回false
    for (int i = 0; i < SIZE; i++) {
        if (num == board[i][col] - '0' && i != row) {
            return false;
        }
    }
    // 检查数字所在的方块有没有重复的，如果有返回false
    int startrow = row / 3 * 3;
    int startcol = col / 3 * 3;
    int endrow = startrow + 3;
    int endcol = startcol + 3;
    for (int i = startrow; i < endrow; i++) {
        for (int j = startcol; j < endcol; j++) {
            if (num == board[i][j] - '0' && (i != col || j != row)) {
                return false;
            }
        }
    }
    return true;
}

int people_check(int row, int col) {  // 确定某格子可以按照简单的规则填入
    bool check[10] = { false };
    // 去掉行内容
    for (int i = 0; i < SIZE; i++) {
        if (board[row][i] != '$') {
            check[board[row][i] - '0'] = true;
        }
    }
    // 去掉列内容
    for (int i = 0; i < SIZE; i++) {
        if (board[i][col] != '$') {
            check[board[i][col] - '0'] = true;
        }
    }
    // 去掉数字所在的方块内容
    int startrow = row / 3 * 3;
    int startcol = col / 3 * 3;
    int endrow = startrow + 3;
    int endcol = startcol + 3;
    for (int i = startrow; i < endrow; i++) {
        for (int j = startcol; j < endcol; j++) {
            if (board[i][j] != '$') {
                check[board[i][j] - '0'] = true;
            }
        }
    }
    int count = 0;
    int people_get = 0;
    for (int i = 0; i < 9; i++) {
        if (check[i] == false) {
            count++;
            people_get = i;
        }
    }
    if (count <= 2) {
        return people_get;
    } else {
        return 0;
    }
}

bool dfs(int start, int& ifsolve) {  // NOLINT
    // 从０开始依次遍历81个格子，计算此数独,ifsolve用于计数一共有多少个解
    if (start == 81) {  // start=81，说明已经成功解出数独
        write_file("sudoku.txt");
        ifsolve++;
        return false;
    } else {
        bool ok = false;
        int row = start / 9;  // 根据此时方格的序号，计算出此方格的行和列
        int col = start % 9;
        if (board[row][col] == '$') {
            for (int i = 1; i <= 9; i++) {
                if (is_valid(row, col, i)) {
                    // 从１－９依次放入空格，并判断是否合法
                    board[row][col] = i + '0';
                    // 如果有数字合法，就写入该数字的字符
                    ok = dfs(start + 1, ifsolve);
                    // 判断此方格的下一个方格是否成功写入
                    if (!ok) {
                        // 如果它的下一个方格是不合法的，说明它现在填入的数，不是正确的解，需回溯
                        board[row][col] = '$';  // 回溯
                    }
                }
            }
        } else {
            ok = dfs(start + 1, ifsolve);
        }
        return ok;
    }
}


int main(int argc, char** argv) {
    int full_num = 0;
    char* file_name = nullptr;
    int game_num = 0;
    int level = 0;
    int dig_up = -1;
    int dig_down = -1;
    bool if_u = false;
    for (int i = 1; i < argc; i++) {
        if (argv[i][1] != 'u'&& i + 1 >= argc) {
            throw std::exception("命令行参数个数异常");
        }
        if (argv[i][1] == 'c') {
            full_num = getnumber(argv[i + 1]);
            i++;
        } else if (argv[i][1] == 's') {
            file_name = argv[i + 1];
            i++;
        } else if (argv[i][1] == 'n') {
            game_num = getnumber(argv[i+1]);
            i++;
        } else if (argv[i][1] == 'm') {
            level = getnumber(argv[i + 1]);
            i++;
        } else if (argv[i][1] == 'r') {
            getdig(argv[i + 1], dig_up, dig_down);
            i++;
        } else if (argv[i][1] == 'u') {
            if_u = true;
        } else {
            throw std::exception("输入了错误的参数！");
        }
    }
    // 范围限制
    if ( (full_num && game_num) || (full_num && (file_name != nullptr))
        || (game_num && (file_name != nullptr)))
        throw std::exception("输入异常！请只要求生成终盘或游戏或解数独！\n");
    else if (!game_num && (if_u || level || dig_up != -1))
        throw std::exception("输入异常！生成游戏时请设置-n参数！\n");
    if (dig_up == -1 && dig_down == -1) {
        dig_up = 55;
        dig_down = 20;
    }
    if (dig_up > 55 || dig_down < 20 )
        throw std::exception
        ("输入异常！生成游戏时的挖空个数必须在20~55之间！\n");
    char arg1 = argv[1][1];
    int num = argc;
    if (full_num) {
        int arg2 = getnumber(argv[2]);
        generate_c(arg2);
    }
    if (file_name) {
        std::fstream outfile;
        outfile.open("sudoku.txt", std::ios::out);
        outfile.close();

        std::ifstream infile;
        while (!read_file(infile, argv[2])) {
            int ifsolve = 0;
            std::cout << "计算中...\n";
            dfs(0, ifsolve);
            std::cout << ifsolve << '\n';
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    std::cout << board[i][j];
                }
                std::cout << '\n';
            }
            std::cout << '\n';
            if (!ifsolve) {
                 std::cout << "该数独无解！\n";
                 write_fail("sudiku_result.txt");
            }
        }
        std::cout << "计算完成，输出为sudoku.txt\n";
    }
    if (game_num && !level && !if_u) {
        generate_c(game_num);
        generategame(game_num, dig_down, dig_up);
    }
    if (game_num && !level && if_u) {
        std::fstream outfile;
        outfile.open("one_answer_question.txt", std::ios::out);
        outfile.close();
        while (1) {
            if (game_num == 0) {
                break;
            }
            generate_c(1);
            generategame(1, dig_down, dig_up);
            std::ifstream infile;
            read_file(infile, "question.txt");
            int ifsolve = 0;
            dfs(0, ifsolve);
            if (ifsolve == 1 && game_num) {
                std::ifstream infile;
                infile.open("question.txt", std::ios::in);
                std::fstream outfile;
                outfile.open("one_answer_question.txt", std::ios::app);
                char temp;
                int j = 0;
                while (j < 9) {
                    for (int i = 0; i < SIZE; i++) {
                        infile >> temp;
                        outfile << temp;
                    }
                    outfile << '\n';
                    j++;
                }
                outfile << '\n';
                outfile.close();
                infile.close();
                game_num--;
            }
        }
    }
        if (game_num && level && !if_u) {
            std::fstream outfile;
            outfile.open("level_question.txt", std::ios::out);
            outfile.close();
            while (1) {
                if (game_num == 0) {
                    break;
                }
                int m = 0;
                generate_c(1);
                generategame(1, dig_down, dig_up);
                std::ifstream infile;
                read_file(infile, "question.txt");
                count_simple = 0;
                for (int i = 0; i < SIZE; i++) {
                    for (int j = 0; j < SIZE; j++) {
                        if (people_check(i, j)) {
                            count_simple++;
                        }
                    }
                }
                int ifsolve = 0;
                clock_t start, end;
                start = clock();
                dfs(0, ifsolve);
                end = clock();  // 程序结束用时
                double endtime = (double)(end - start) / CLOCKS_PER_SEC;  // NOLINT
                bool save = false;
                switch (level) {
                case 1:
                    if (count_simple >= 15 || endtime * 1000 <= 40)
                        save = true;
                    break;
                case 3:
                    if (endtime * 1000 >=500)
                        save = true;
                    break;
                case 2:
                    if (count_simple <15 ||
                        endtime * 1000 > 40 && endtime * 1000 < 500)
                        save = true;
                    break;
                }
                if (save) {
                    std::ifstream infile;
                    infile.open("question.txt", std::ios::in);
                    std::fstream outfile;
                    outfile.open("level_question.txt", std::ios::app);
                    char temp;
                    int j = 0;
                    while (j < 9) {
                        for (int i = 0; i < SIZE; i++) {
                            infile >> temp;
                            outfile << temp;
                        }
                        outfile << '\n';
                        j++;
                    }
                    outfile << '\n';
                    outfile.close();
                    infile.close();
                    game_num--;
                }
            }
        }
        if (game_num && level && if_u) {
            std::fstream outfile;
            outfile.open("one_answer_level_question.txt", std::ios::out);
            outfile.close();
            while (1) {
                if (game_num == 0) {
                    break;
                }
                int m = 0;
                generate_c(1);
                generategame(1, dig_down, dig_up);
                std::ifstream infile;
                infile.open("question.txt", std::ios::in);
                read_file(infile, "question.txt");
                count_simple = 0;
                for (int i = 0; i < SIZE; i++) {
                    for (int j = 0; j < SIZE; j++) {
                        if (people_check(i, j)) {
                            count_simple++;
                        }
                    }
                }
                int ifsolve = 0;
                clock_t start, end;
                start = clock();
                dfs(0, ifsolve);
                end = clock();  // 程序结束用时
                double endtime = (double)(end - start) / CLOCKS_PER_SEC;  // NOLINT
                bool save = false;
                switch (level) {
                case 1:
                    if (count_simple >= 15 || endtime * 1000 <= 40)
                        save = true;
                    break;
                case 3:
                    if (endtime * 1000 >= 500)
                        save = true;
                    break;
                case 2:
                    if (count_simple < 15 ||
                        endtime * 1000 > 40 && endtime * 1000 < 500)
                        save = true;
                    break;
                }
                if (save && ifsolve == 1 && game_num) {
                    std::ifstream infile;
                    infile.open("question.txt", std::ios::in);
                    std::fstream outfile;
                    outfile.open("one_answer_level_question.txt",
                        std::ios::app);
                    char temp;
                    int j = 0;
                    while (j < 9) {
                        for (int i = 0; i < SIZE; i++) {
                            infile >> temp;
                            outfile << temp;
                        }
                        outfile << '\n';
                        j++;
                    }
                    outfile << '\n';
                    outfile.close();
                    infile.close();
                    game_num--;
                }
            }
        }
    return 0;
}
