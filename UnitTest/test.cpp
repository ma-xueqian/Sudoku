#include "pch.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include "sudoku.h"
#include <string.h>

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}


TEST(TestCase1, TestC) {
    //generate_c(1);
    int num_test_cases = 2;    //测试用例数

    for (int i = 2; i <= num_test_cases+1; i++) {
        // 生成数独终盘
        generate_c(i);

        int row = 0;
        int col = 0;

        // 读取生成的数独终盘
        std::ifstream file(RESPATH);
        std::string line;
        int count = 0;
        while (getline(file, line)) {
            // 检查每行的长度
            EXPECT_TRUE(line.length() == 17 || line.length() == 0);

            // 检查每行是否为合法的数独行
            for (int j = 0; j < line.length(); j += 2) {
                char c = line[j];
                if (c != ' ') {
                    board[row][col] = c;
                    col++;
                }
                // 检查每个字符是否为数字或空格
                EXPECT_TRUE((c >= '1' && c <= '9') || c == ' ');

                row++;
                if (line.length() == 0) {
                    for (int h = 0; h < 9; h++) {
                        for (int k = 0; k < 9; k++) {
                            bool judge = is_valid(h, k, (board[h][k] - '0'));
                            EXPECT_TRUE(judge);
                        }
                    }
                    row = 0;
                    col = 0;
                }
                
            }

            count++;
        }

        // 检查生成的数独终盘的行数是否正确
        EXPECT_TRUE(count == (i * 10 -1));

        file.close();
    }
}

TEST(TestCase3, TestC_fail) {
    try {
        generate_c(-1);
    }
    catch (std::exception& ex) {
        EXPECT_STREQ("输入数据不合法", ex.what());
    }
}


TEST(TestCase4, TestN_fail) {
    try {
        generategame(-1);
    }
    catch (std::exception& ex) {
        EXPECT_STREQ("输入数据不合法", ex.what());
    }
}

TEST(Test_GetNumber, Test_getnumber) {
    int res0 = getnumber("12");
    EXPECT_EQ(res0, 12);

    int res1 = getnumber("32");
    EXPECT_EQ(res1, 32);

    try {
        getnumber("cc");
    }
    catch (std::exception& ex) {
        EXPECT_STREQ("输入数据类型有误", ex.what());
    }
}

TEST(Test_GetDig, Test_getdig) {
    try {
        int a = 1, b = 3;
        getdig("cc",a,b);
    }
    catch (std::exception& ex) {
        EXPECT_STREQ("输入数据类型有误", ex.what());
    }
}


TEST(TestCase2, TestN) {
    // 设置测试用例的数量
    int num_test_cases = 2;

    for (int i = 1; i <= num_test_cases; i++) {
        // 生成数独终盘游戏
        generate_c(i);
        generategame(i, 40, 45);

        // 读取生成的题目文件
        std::ifstream file(QUESPATH);
        std::string line;
        int count = 0;
        int spacenum = 0;
        while (getline(file, line)) {
            // 检查每行的长度
            EXPECT_TRUE(line.length() == 17 || line.length() == 0);

            // 检查每行是否为合法的数独行
            for (int j = 0; j < line.length(); j += 2) {
                char c = line[j];
                if (c == '$') {
                    spacenum++;
                }
                // 检查每个字符是否为数字、空格或$
                EXPECT_TRUE((c >= '1' && c <= '9') || c == ' ' || c == '$');
            }
            if (line.length() == 0) {
                EXPECT_TRUE(spacenum >= 40 && spacenum <= 45);
                spacenum = 0;
            }
            count++;
        }

        // 检查生成的数独终盘游戏的行数是否正确
        EXPECT_TRUE(count == (i * 10 - 1));
        file.close();
    }
}


//read_file
TEST(ReadTest, test_read_file) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = '0';
        }
    }
    std::ifstream infile;
    infile.open("C:/Users/huawei/Desktop/Sample-Test1/Debug/test1.txt", std::ios::in);
    read_file(infile, "test1.txt");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            EXPECT_TRUE(board[i][j] == '$');
        }
    }
}

TEST(ReadTest, test_read_fail) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = '0';
        }
    }
    std::ifstream infile;
    try {
        read_file(infile, "C:/Users/huawei/Desktop/Sample-Test1/Debug/test11.txt");
    }
    catch (std::exception& ex) {
        EXPECT_STREQ("数据输入异常", ex.what());
    }
}

//write_file
TEST(WriteTest, test_write_file) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = '$';
        }
    }
    write_file("C:/Users/huawei/Desktop/Sample-Test1/Debug/test_write.txt");
    std::ifstream infile;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = '0';
        }
    }
    read_file(infile, "C:/Users/huawei/Desktop/Sample-Test1/Debug/test_write.txt");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            EXPECT_TRUE(board[i][j] == '$');
        }
    }
}

//write_fail
TEST(WriteFailTest, test_write_fail) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = '$';
        }
    }
    write_fail("C:/Users/huawei/Desktop/Sample-Test1/Debug/test_write_fail.txt");
    std::ifstream infile;
    infile.open("C:/Users/huawei/Desktop/Sample-Test1/Debug/test_write_fail.txt");
    std::string temps = "";
    std::getline(infile, temps);
    EXPECT_TRUE(temps == "该数独无解！");
}

//is_valid:检测当前的填充方式是否合规
TEST(ValidTest, test_valid_all) {//全相同
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = '1';
        }
    }
    bool answer = is_valid(0, 0, 1);
    EXPECT_TRUE(!answer);
    answer = is_valid(0, 0, 2);
    EXPECT_TRUE(answer);
}

TEST(ValidTest, test_valid_row) {//行相同
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = '1';
        }
    }
    board[0][3] = '2';
    bool answer = is_valid(0, 0, 2);
    EXPECT_TRUE(!answer);
}

TEST(ValidTest, test_valid_col) {//列相同
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = '1';
        }
    }
    board[3][0] = '2';
    bool answer = is_valid(0, 0, 2);
    EXPECT_TRUE(!answer);
}

TEST(ValidTest, test_valid_square) {//九宫格内相同
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = '1';
        }
    }
    board[1][1] = '2';
    bool answer = is_valid(0, 0, 2);
    EXPECT_TRUE(!answer);
}

//people_check
TEST(PeopleCheckTest, test_cannot_check) {//people can't check
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = '$';
        }
    }
    int result = people_check(0, 0);
    EXPECT_TRUE(!result);
}

TEST(PeopleCheckTest, test_row_check) {//people can check：通过行确定填空内容
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = '$';
        }
    }
    for (int j = 1; j < 9; j++) {
        board[0][j] = '0'+ j + 1;
    }
    int result = people_check(0, 0);
    EXPECT_TRUE(result);
}

TEST(PeopleCheckTest, test_col_check) {//people can check：通过列确定填空内容
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = '$';
        }
    }
    for (int j = 1; j < 9; j++) {
        board[j][0] = '0' + j + 1;
    }
    int result = people_check(0, 0);
    EXPECT_TRUE(result);
}

TEST(PeopleCheckTest, test_square_check) {//people can check：通过九宫格确定填空内容
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = '$';
        }
    }
    int a = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if ((i | j) != 0)
                board[i][j] = '0' + a;
            a++;
        }
    }
    int result = people_check(0, 0);
    EXPECT_TRUE(result);
}

TEST(PeopleCheckTest, test_mix_check) {//people can check：通过多种信息混合确定填空内容
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = '$';
        }
    }
    board[0][1] = '2';
    board[1][0] = '3';
    board[2][0] = '4';
    board[1][1] = '5';
    board[2][2] = '6';
    board[0][5] = '7';
    board[0][6] = '8';
    board[8][0] = '9';
    int result = people_check(0, 0);
    EXPECT_TRUE(result);
}

//dfs迭代获取结果
TEST(DfsTest, test_one_answer) {//题目有唯一解
    std::ifstream infile;
    std::ifstream infile3;
    std::fstream outfile;
    outfile.open("C:/Users/huawei/Desktop/Sample-Test1/Debug/sudoku.txt", std::ios::out);
    read_file(infile, "C:/Users/huawei/Desktop/Sample-Test1/Debug/test2.txt");
    int ifsolve = 0;
    dfs(0, ifsolve);
    EXPECT_TRUE(ifsolve == 1);
    bool result = false;
    char board_temp[9][9];
    read_file(infile3, "C:/Users/huawei/Desktop/Sample-Test1/Debug/sudoku.txt");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board_temp[i][j] = board[i][j];
        }
    }
    std::ifstream infile2;
    read_file(infile2, "C:/Users/huawei/Desktop/Sample-Test1/Debug/test2_result.txt");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            EXPECT_TRUE(board_temp[i][j] == board[i][j]);
        }
    }
}

TEST(DfsTest, test_mul_answer) {//题目有多个解
    std::ifstream infile;
    read_file(infile, "C:/Users/huawei/Desktop/Sample-Test1/Debug/test3.txt");
    int ifsolve = 0;
    dfs(0, ifsolve);
    EXPECT_TRUE(ifsolve == 2);
}

TEST(DfsTest, test_no_answer) {//题目无解
    std::ifstream infile;
    read_file(infile, "C:/Users/huawei/Desktop/Sample-Test1/Debug/test4.txt");
    int ifsolve = 0;
    dfs(0, ifsolve);
    EXPECT_TRUE(ifsolve == 0);
}

TEST(MainTest, test_cmd_number) {//参数个数异常
    char** argv;
    argv = new char* [4];
    for (int i = 0; i < 4; i++) {
        argv[i] = new char [5];
    }
    argv[0] = "  ";
    argv[1] = "-n";
    argv[2] = "10"; 
    argv[3] = "-m";
    try {
        test_main(4, argv);
    }
    catch (std::exception& ex) {
        EXPECT_STREQ("命令行参数个数异常", ex.what());
    }
}

TEST(MainTest, test_cmd_wrong) {//参数错误
    char** argv;
    argv = new char* [5];
    for (int i = 0; i < 5; i++) {
        argv[i] = new char[5];
    }
    argv[0] = "  ";
    argv[1] = "-o";
    argv[2] = "10";
    argv[3] = "-m";
    argv[4] = "1";
    try {
        test_main(5, argv);
    }
    catch (std::exception& ex) {
        EXPECT_STREQ("输入了错误的参数！", ex.what());
    }
}

TEST(MainTest, test_cmd_conflict) {//输入异常
    char** argv;
    argv = new char* [5];
    for (int i = 0; i < 5; i++) {
        argv[i] = new char[5];
    }
    argv[0] = "  ";
    argv[1] = "-c";
    argv[2] = "10";
    argv[3] = "-n";
    argv[4] = "1";
    try {
        test_main(5, argv);
    }
    catch (std::exception& ex) {
        EXPECT_STREQ("输入异常！请只要求生成终盘或游戏或解数独！\n", ex.what());
    }
}

TEST(MainTest, test_cmd_loss) {//输入异常
    char** argv;
    argv = new char* [5];
    for (int i = 0; i < 5; i++) {
        argv[i] = new char[5];
    }
    argv[0] = "  ";
    argv[1] = "-r";
    argv[2] = "21~22";
    argv[3] = "-m";
    argv[4] = "1";
    try {
        test_main(5, argv);
    }
    catch (std::exception& ex) {
        EXPECT_STREQ("输入异常！生成游戏时请设置-n参数！\n", ex.what());
    }
}

TEST(MainTest, test_cmd_dig) {//挖空异常
    char** argv;
    argv = new char* [5];
    for (int i = 0; i < 5; i++) {
        argv[i] = new char[5];
    }
    argv[0] = "  ";
    argv[1] = "-r";
    argv[2] = "11~12";
    argv[3] = "-n";
    argv[4] = "1";
    try {
        test_main(5, argv);
    }
    catch (std::exception& ex) {
        EXPECT_STREQ("输入异常！生成游戏时的挖空个数必须在20~55之间！\n", ex.what());
    }
}

TEST(MainTest, test_cmd_right1) {//-c测试
    char** argv;
    argv = new char* [3];
    for (int i = 0; i < 3; i++) {
        argv[i] = new char[5];
    }
    argv[0] = "  ";
    argv[1] = "-c";
    argv[2] = "1";
    test_main(3, argv);
}

TEST(MainTest, test_cmd_right2) {//-s测试
    char** argv;
    argv = new char* [3];
    for (int i = 0; i < 3; i++) {
        argv[i] = new char[60];
    }
    argv[0] = "  ";
    argv[1] = "-s";
    argv[2] = "C:/Users/huawei/Desktop/Sample-Test1/Debug/test2.txt";
    test_main(3, argv);
}

TEST(MainTest, test_cmd_fail1) {//-s测试
    char** argv;
    argv = new char* [3];
    for (int i = 0; i < 3; i++) {
        argv[i] = new char[60];
    }
    argv[0] = "  ";
    argv[1] = "-s";
    argv[2] = "C:/Users/huawei/Desktop/Sample-Test1/Debug/test4.txt";
    test_main(3, argv);
}

TEST(MainTest, test_cmd_right3) {//-n测试
    char** argv;
    argv = new char* [3];
    for (int i = 0; i < 3; i++) {
        argv[i] = new char[5];
    }
    argv[0] = "  ";
    argv[1] = "-n";
    argv[2] = "1";
    test_main(3, argv);
}

TEST(MainTest, test_cmd_right4) {//-n-m测试
    char** argv;
    argv = new char* [5];
    for (int i = 0; i < 5; i++) {
        argv[i] = new char[5];
    }
    argv[0] = "  ";
    argv[1] = "-n";
    argv[2] = "1";
    argv[3] = "-m";
    argv[4] = "2";
    test_main(5, argv);
}

TEST(MainTest, test_cmd_right5) {//-n-m测试
    char** argv;
    argv = new char* [5];
    for (int i = 0; i < 5; i++) {
        argv[i] = new char[5];
    }
    argv[0] = "  ";
    argv[1] = "-n";
    argv[2] = "1";
    argv[3] = "-m";
    argv[4] = "1";
    test_main(5, argv);
}

TEST(MainTest, test_cmd_right6) {//-n-m测试
    char** argv;
    argv = new char* [5];
    for (int i = 0; i < 5; i++) {
        argv[i] = new char[5];
    }
    argv[0] = "  ";
    argv[1] = "-n";
    argv[2] = "1";
    argv[3] = "-m";
    argv[4] = "3";
    test_main(5, argv);
}

TEST(MainTest, test_cmd_right7) {//-n-r测试
    char** argv;
    argv = new char* [5];
    for (int i = 0; i < 5; i++) {
        argv[i] = new char[5];
    }
    argv[0] = "  ";
    argv[1] = "-n";
    argv[2] = "1";
    argv[3] = "-r";
    argv[4] = "21~22";
    test_main(5, argv);
}

TEST(MainTest, test_cmd_right8) {//-n-u测试
    char** argv;
    argv = new char* [4];
    for (int i = 0; i < 4; i++) {
        argv[i] = new char[5];
    }
    argv[0] = "  ";
    argv[1] = "-n";
    argv[2] = "1";
    argv[3] = "-u";
    test_main(4, argv);
}

TEST(MainTest, test_cmd_right9) {//-n-m-u测试
    char** argv;
    argv = new char* [6];
    for (int i = 0; i < 6; i++) {
        argv[i] = new char[5];
    }
    argv[0] = "  ";
    argv[1] = "-n";
    argv[2] = "1";
    argv[3] = "-u";
    argv[4] = "-m";
    argv[5] = "1";
    test_main(6, argv);
}